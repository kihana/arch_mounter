#include "Logger.h"

using namespace std;

Logger::Logger()
{
  auto sink = make_shared<spdlog::sinks::msvc_sink_mt>();
  logger_ = make_shared<spdlog::logger>("arch_mounter", sink);
  logger_->set_level(spdlog::level::debug);
  logger_->set_pattern("[%P.%t] [%l] %v");
}

shared_ptr<spdlog::logger>& Logger::instance()
{
  static Logger instance;

  return instance.logger_;
}
