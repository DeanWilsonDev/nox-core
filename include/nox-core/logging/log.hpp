#pragma once

#include <firefly/log.hpp>
#include <memory>

namespace NoxCore {
namespace Logging {

#ifdef NOX_CORE_PLATFORM_MACOS
#define NOX_CORE_API __attribute__((visibility("default")))
#elif NOX_CORE_PLATFORM_WINDOWS
#define NOX_CORE_API __declspec(dllexport)
#else
#define NOX_CORE_API
#endif

using Logger = Firefly::Logger;

class Log {
 public:
  NOX_CORE_API static void Init(bool enableClientDebugLogging = true);
  NOX_CORE_API static void Init(std::string fileName, bool enableClientDebugLogging = true);
};
}  // namespace Logging
}  // namespace NoxCore
