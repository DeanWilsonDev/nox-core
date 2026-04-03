#include <firefly/log.hpp>
#include "log.hpp"

namespace NoxCore {

namespace Logging {

void Log::init(bool enableClientDebugLogging)
{
  Firefly::Logging::LogRegistry::registerLogger(FIREFLY_DEFAULT_LOGGER, enableClientDebugLogging);
}

void Log::init(std::string fileName, bool enableClientDebugLogging)
{
  Firefly::Logging::LogRegistry::registerLogger(
      FIREFLY_DEFAULT_LOGGER, fileName, enableClientDebugLogging
  );
}
}  // namespace Logging

}  // namespace NoxCore
