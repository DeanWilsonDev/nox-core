#pragma once

#include <string>
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

struct ASTNode {
 public:
  std::string name;
  NodeKind kind;
  std::string file;
  std::string type;
  int line;
  std::string parentName;
  std::string referencedName;
};

}  // namespace Prism
