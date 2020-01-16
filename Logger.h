#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/msvc_sink.h"

#include <memory>

class Logger
{
public:
  static std::shared_ptr<spdlog::logger>& instance();

  Logger(const Logger& other) = delete;
  Logger(const Logger&& other) = delete;
  Logger& operator=(const Logger& other) = delete;
  Logger& operator=(const Logger&& other) = delete;

private:
  Logger();

private:
  std::shared_ptr<spdlog::logger> logger_;
};

