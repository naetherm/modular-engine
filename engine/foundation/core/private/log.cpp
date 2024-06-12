////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/log.h"
#include "core/format.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>


#define LOG_STATIC_BUFFER_SIZE 2048


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
static void me_logger_system_default_log(struct me_logger_o* logger, me_logging_type loggingType, const char* msg) {
  // Check whether the provided msg does end on \n
  bool hasLineEnding = msg && *msg && msg[strlen(msg) - 1] == '\n';
  printf(hasLineEnding ? "%s" : "%s\n", msg);
  fflush(stdout);
}

static me_logger_i system_default_logger = {
  .log = me_logger_system_default_log
};


namespace internal {

static constexpr uint32 max_loggers = 64;
uint32 num_loggers = 0;

me_logger_i loggers[max_loggers];

static void me_logger_internal_init() {
  if (num_loggers == 0) {
    num_loggers = 1;
    loggers[0] = system_default_logger;
  }
}

}


static void me_log_add_logger(const me_logger_i *logger) {
  internal::me_logger_internal_init();

  // Sanity check

  internal::loggers[internal::num_loggers++] = *logger;
}

static void me_log_remove_logger(const me_logger_i *logger) {
  internal::me_logger_internal_init();

  // TODO(naetherm): Should we use C++ (containers)?
  for (uint32 i = 0; i < internal::num_loggers; ++i) {
    if (memcmp(&internal::loggers[i], logger, sizeof(me_logger_i)) == 0) {
      internal::loggers[i] = internal::loggers[--internal::num_loggers];
      return;
    }
  }
}

static void me_log_print(enum me_logging_type loggingType, const char* msg) {
  internal::me_logger_internal_init();

  for (uint32 i = 0; i < internal::num_loggers; ++i) {
    internal::loggers[i].log(internal::loggers[i].instance, loggingType, msg);
  }
}

static int me_log_printf(enum me_logging_type loggingType, const char* msg, ...) {
  internal::me_logger_internal_init();
  va_list args;
  va_start(args, msg);

  va_list args2;
  va_copy(args2, args);
  int ret = me_vsnprintf(nullptr, 0, msg, args2);
  va_end(args2);

  if (ret < 0)
    return ret;

  if (ret >= LOG_STATIC_BUFFER_SIZE) {
    const size_t buffer_size = (size_t)ret + 1;
    char *buffer = static_cast<char *>(malloc(buffer_size));
    if (buffer) {
      ret = me_vsnprintf(buffer, (int)buffer_size, msg, args);
      me_log_print(loggingType, buffer);

      free(buffer);
      va_end(args);
      return ret;
    }
  }

  char buffer[LOG_STATIC_BUFFER_SIZE];
  ret = me_vsnprintf(buffer, sizeof(buffer), msg, args);
  me_log_print(loggingType, buffer);

  va_end(args);
  return ret;
}


static struct me_logger_api logger = {
  .add_logger = me_log_add_logger,
  .remove_logger = me_log_remove_logger,
  .print = me_log_print,
  .printf = me_log_printf
};


struct me_logger_api* me_logger_api = &logger;