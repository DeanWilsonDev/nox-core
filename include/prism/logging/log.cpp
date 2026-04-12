#include "log.hpp"
#include <firefly/log-registry.hpp>

namespace Prism {

namespace Logging {

void Log::Init(bool enableClientDebugLogging)
{
  Firefly::LogRegistry::RegisterLogger(FIREFLY_DEFAULT_LOGGER, enableClientDebugLogging);
}

void Log::Init(std::string fileName, bool enableClientDebugLogging)
{
  Firefly::LogRegistry::RegisterLogger(FIREFLY_DEFAULT_LOGGER, fileName, enableClientDebugLogging);
}
}  // namespace Logging
}  // namespace Prism
