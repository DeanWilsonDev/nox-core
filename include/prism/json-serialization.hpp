#include <nlohmann/json.hpp>
#include "ast-node.hpp"
#include "nlohmann/detail/macro_scope.hpp"

NLOHMANN_JSON_SERIALIZE_ENUM(Prism::NodeKind, Prism::nodeKindNames)
