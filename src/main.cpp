#include <clang-c/CXSourceLocation.h>
#include <clang-c/Index.h>
#include <iostream>
#include "prism/ast-node.hpp"

std::string ToString(CXString value)
{
  std::string str = clang_getCString(value);
  clang_disposeString(value);
  return str;
}

void PrintCursor(CXCursor cursor)
{
  std::string nameStr = ToString(clang_getCursorSpelling(cursor));
  std::string kindStr = ToString(clang_getCursorKindSpelling(cursor.kind));
  std::string typeStr = ToString(clang_getTypeSpelling(clang_getCursorType(cursor)));
  CXSourceLocation location = clang_getCursorLocation(cursor);

  CXFile file;
  unsigned line, column, offset;

  clang_getExpansionLocation(location, &file, &line, &column, &offset);

  std::string fileName = ToString(clang_getFileName(file));

  std::cout << "Found: " << "<" << typeStr << "> " << nameStr << " [" << kindStr << "] "
            << "File: " << fileName << " Line: " << line << " Column: " << column << std::endl;
}

// Prism::NodeKind GetNodeKindFromString(std::string kind)
// {
//
//
//
// }

Prism::ASTNode BuildAstNode(CXCursor cursor)
{
  Prism::ASTNode node;
  node.name = ToString(clang_getCursorSpelling(cursor));
  // node.kind = ToString(clang_getCursorKindSpelling(cursor.kind));
  std::string typeStr = ToString(clang_getTypeSpelling(clang_getCursorType(cursor)));
  CXSourceLocation location = clang_getCursorLocation(cursor);

  CXFile file;
  unsigned line, column, offset;

  clang_getExpansionLocation(location, &file, &line, &column, &offset);

  std::string fileName = ToString(clang_getFileName(file));
  return node;
}

CXChildVisitResult Visitor(CXCursor cursor, CXCursor parent, CXClientData data)
{
  CXSourceLocation location = clang_getCursorLocation(cursor);
  if (!clang_Location_isFromMainFile(location)) {
    return CXChildVisit_Continue;
  }

  CXCursorKind kind = clang_getCursorKind(cursor);

  switch (kind) {
    case CXCursor_Namespace:
      PrintCursor(cursor);
      break;
    case CXCursor_ClassDecl:
      PrintCursor(cursor);
      break;
    case CXCursor_StructDecl:
      PrintCursor(cursor);
      break;
    case CXCursor_FunctionDecl:
      PrintCursor(cursor);
      break;
    case CXCursor_CXXMethod:
      PrintCursor(cursor);
      break;
    case CXCursor_FieldDecl:
      PrintCursor(cursor);
      break;
    case CXCursor_InclusionDirective:
      PrintCursor(cursor);
      break;
    case CXCursor_CXXBaseSpecifier:
      PrintCursor(cursor);
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
      "src/test-input.cpp",
      nullptr,
      0,
      nullptr,
      0,
      CXTranslationUnit_DetailedPreprocessingRecord
  );

  CXCursor rootCursor = clang_getTranslationUnitCursor(unit);

  clang_visitChildren(rootCursor, Visitor, nullptr);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
  return 0;
}
