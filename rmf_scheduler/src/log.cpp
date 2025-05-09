// Copyright 2023 ROS Industrial Consortium Asia Pacific
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdarg>
#include <cstdio>
#include "rmf_scheduler/log.hpp"
#include "rmf_scheduler/default_log_handler.hpp"

namespace rmf_scheduler
{

namespace log
{

class Logger
{
public:
  Logger()
  {
    log_level_ = LogLevel::INFO;
    log_handler_.reset(new DefaultLogHandler());
  }

  ~Logger()
  {
    if (log_handler_) {
      log_handler_.reset();
    }
  }

  void registerLogHandler(std::unique_ptr<LogHandler> loghandler)
  {
    log_handler_ = std::move(loghandler);
  }

  void unregisterLogHandler()
  {
    log_handler_.reset(new DefaultLogHandler());
  }

  void log(const char * file, int line, LogLevel level, const char * txt)
  {
    if (!log_handler_) {
      log_handler_.reset(new DefaultLogHandler());
    }

    log_handler_->log(file, line, level, txt);
  }

  void setLogLevel(LogLevel level)
  {
    log_level_ = level;
  }

  LogLevel getLogLevel()
  {
    return log_level_;
  }

private:
  std::unique_ptr<LogHandler> log_handler_;
  LogLevel log_level_;
};
Logger g_logger;

void registerLogHandler(std::unique_ptr<LogHandler> loghandler)
{
  g_logger.registerLogHandler(std::move(loghandler));
}

void unregisterLogHandler()
{
  g_logger.unregisterLogHandler();
}

void setLogLevel(LogLevel level)
{
  g_logger.setLogLevel(level);
}

void log(const char * file, int line, LogLevel level, const char * fmt, ...)
{
  if (level >= g_logger.getLogLevel()) {
    size_t buffer_size = 1024;
    std::unique_ptr<char[]> buffer;
    buffer.reset(new char[buffer_size]);

    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);

    size_t characters = 1 + std::vsnprintf(buffer.get(), buffer_size, fmt, args);

    if (characters >= buffer_size) {
      buffer_size = characters + 1;
      buffer.reset(new char[buffer_size]);
      std::vsnprintf(buffer.get(), buffer_size, fmt, args_copy);
    }

    va_end(args);
    va_end(args_copy);

    g_logger.log(file, line, level, buffer.get());
  }
}

}  // namespace log

}  // namespace rmf_scheduler
