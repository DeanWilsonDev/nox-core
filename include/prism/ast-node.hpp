#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace Prism {

enum NodeKind { Project, Module, Namespace, Class, Struct, ParentClass, Include, Function, Field };

static const std::unordered_map<std::string, NodeKind> kindMap = {
    {"Namespace", NodeKind::Namespace},
    {"ClassDecl", NodeKind::Class},
    {"C++ base class specifier", NodeKind::ParentClass},
    {"CXXMethod", NodeKind::Function},
    {"inclusion directive", NodeKind::Include},
    {"FieldDecl", NodeKind::Field},
};

inline constexpr std::pair<NodeKind, std::string_view> nodeKindNames[] = {
    {NodeKind::Project, "Project"},
    {NodeKind::Module, "Module"},
    {NodeKind::Namespace, "Namespace"},
    {NodeKind::Class, "Class"},
    {NodeKind::Struct, "Struct"},
    {NodeKind::ParentClass, "ParentClass"},
    {NodeKind::Include, "Include"},
    {NodeKind::Function, "Function"},
    {NodeKind::Field, "Field"},
};

std::string_view ToStringFromNodeKind(NodeKind kind){
    for(const auto& [enumValue, name]: nodeKindNames){
        if(enumValue == kind){
            return name;
        }
    }
    return "unknown";
}

struct ASTNode {
 public:
  int id;
  std::string name;
  std::string type;
  NodeKind kind;
  std::string file;
  int line;
  int column;
  std::string physicalParent;
  std::string logicalParent;
  std::string referencedName;
};

}  // namespace Prism
