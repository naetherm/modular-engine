////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2023 RacoonStudios
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
#include "core/assert.h"
#include "core/format.h"
#include <cstdio>
#include <cstdlib>


static void me_assert_logger_assert_error(me_error_o* i, const char* file, uint32 line, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  va_list args2;
  va_copy(args2, args);
  int ret = me_vsnprintf(nullptr, 0, fmt, args2);
  va_end(args2);

  if (ret >= 2048) {
    const size_t buffer_size = (size_t)ret + 1;
    char *buffer = static_cast<char *>(malloc(buffer_size));
    if (buffer) {
      ret = me_vsnprintf(buffer, (int)buffer_size, fmt, args);

      free(buffer);
      va_end(args);
      return;
    }
  }

  char buffer[2048];
  ret = me_vsnprintf(buffer, sizeof(buffer), fmt, args);

  va_end(args);
}

static void me_assert_logger_assert_fatal(me_error_o* i, const char* file, uint32 line, const char* fmt, ...) {

}


static me_error_inst logger_assert = {
  .error = me_assert_logger_assert_error,
  .fatal = me_assert_logger_assert_fatal
};


static struct me_error_api error = {
  .logger_assert = &logger_assert,
  .default_assert = &logger_assert
};


struct me_error_api* me_error_api = &error;