#include <clang-c/CXSourceLocation.h>
#include <clang-c/Index.h>
#include <iostream>
#include "firefly/log.hpp"
#include "nlohmann/json_fwd.hpp"
#include "prism/ast-node.hpp"
#include "prism/json-serialization.hpp"
#include <filesystem>
#include <unordered_set>

std::string ToString(CXString value)
{
  std::string str = clang_getCString(value);
  clang_disposeString(value);
  return str;
}

nlohmann::json ToJson(const Prism::ASTNode& node)
{
  return nlohmann::json{
      {"id", node.id},
      {"name", node.name},
      {"type", node.type},
      {"kind", Prism::ToStringFromNodeKind(node.kind)},
      {"file", node.file},
      {"line", node.line},
      {"column", node.column},
      {"physical_parent", node.physicalParent},
      {"logical_parent", node.logicalParent},
      {"referenced_name", node.referencedName},
  };
}

// TODO: Configure this somewhere at the top level:
std::string projectName = "Prism";

std::optional<Prism::NodeKind> GetNodeKindFromString(std::string kind)
{
  auto iterator = Prism::kindMap.find(kind);
  if (iterator != Prism::kindMap.end()) {
    return iterator->second;
  }
  LOG_WARNING("Mapping of kind [{}] was not found", kind);
  return std::nullopt;
}

Prism::ASTNode BuildAstNode(CXCursor cursor)
{
  Prism::ASTNode node;
  node.name = ToString(clang_getCursorSpelling(cursor));

  std::string typeStr = ToString(clang_getTypeSpelling(clang_getCursorType(cursor)));
  node.type = typeStr;

  std::string kindStr = ToString(clang_getCursorKindSpelling(cursor.kind));
  auto kind = GetNodeKindFromString(kindStr);
  if (kind.has_value()) {
    node.kind = kind.value();
  }

  CXSourceLocation location = clang_getCursorLocation(cursor);

  CXFile file;
  unsigned line, column, offset;
  clang_getExpansionLocation(location, &file, &line, &column, &offset);
  std::string filePath = ToString(clang_getFileName(file));
  node.line = line;
  node.column = column;

  std::filesystem::path fullPath(
      "/run/media/deanwilson/FEARLESS_SSD/development/prism/src/example/test-input.cpp"
  );
  std::filesystem::path projectRoot("/run/media/deanwilson/FEARLESS_SSD/development/prism/src");
  std::filesystem::path relativePath = std::filesystem::relative(fullPath, projectRoot);

  std::string parentId = projectName;

  std::string fileName = relativePath.filename();
  std::string moduleId = projectName;

  std::string parentDirectory = relativePath.parent_path();

  for (const std::filesystem::path& segment : relativePath) {
    if (segment == fileName) {
      break;
    }

    moduleId = projectName + "::" + segment.string();

    parentId = moduleId;
  }

  node.physicalParent = parentId;
  node.file = fileName;

  nlohmann::json nodeJson = ToJson(node);
  std::cout << nodeJson.dump(2) << std::endl;

  return node;
}

CXChildVisitResult Visitor(CXCursor cursor, CXCursor parent, CXClientData data)
{
  CXSourceLocation location = clang_getCursorLocation(cursor);
  Prism::ASTNode node;
  if (!clang_Location_isFromMainFile(location)) {
    return CXChildVisit_Continue;
  }

  CXCursorKind kind = clang_getCursorKind(cursor);

  switch (kind) {
    case CXCursor_Namespace:
    case CXCursor_ClassDecl:
    case CXCursor_StructDecl:
    case CXCursor_FunctionDecl:
    case CXCursor_CXXMethod:
    case CXCursor_FieldDecl:
    case CXCursor_InclusionDirective:
    case CXCursor_CXXBaseSpecifier:
      node = BuildAstNode(cursor);
      break;
    default:
      break;
  }

  return CXChildVisit_Recurse;
};

int main()
{
  CXIndex index = clang_createIndex(0, 0);

  CXTranslationUnit unit = clang_parseTranslationUnit(
      index,
      "src/example/test-input.cpp",
      nullptr,
      0,
      nullptr,
      0,
      CXTranslationUnit_DetailedPreprocessingRecord
  );

  if (unit == nullptr) {
    return 1;
  }

  CXCursor rootCursor = clang_getTranslationUnitCursor(unit);

  clang_visitChildren(rootCursor, Visitor, nullptr);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
  return 0;
}
