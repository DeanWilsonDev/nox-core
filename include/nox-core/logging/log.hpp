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

// Log Macros
#define LOG_TRACE(...) Firefly::Logging::LOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) Firefly::Logging::LOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) Firefly::Logging::LOG_INFO(__VA_ARGS__)
#define LOG_WARNING(...) Firefly::Logging::LOG_WARNING(__VA_ARGS__)
#define LOG_ERROR(...) Firefly::Logging::LOG_ERROR(__VA_ARGS__)
#define LOG_FATAL(...) Firefly::Logging::LOG_FATAL(__VA_ARGS__)

}  // namespace NoxCore
