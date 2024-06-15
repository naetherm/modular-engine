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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/core_types.h"


typedef struct me_error_obj me_error_o;

/**
 * @struct
 * me_error_inst
 */
typedef struct me_error_inst {

  me_error_o* instance;

  void (*error)(me_error_o* i, const char* file, uint32 line, const char* fmt, ...);

  void (*fatal)(me_error_o* i, const char* file, uint32 line, const char* fmt, ...);
} me_error_inst;


/**
 * @struct
 * me_error_api
 */
struct me_error_api {
  me_error_inst* logger_assert;

  me_error_inst* default_assert;
};

// TODO(naetherm): Only in debug and release, but not production?

#define ME_STATIC_ASSERT(x) static_assert(x)

#define ME_ASSERT(test, fmt, ...) ((test) || (me_error_api->default_assert->error(me_error_api->default_assert->instance, __FILE__, __LINE__, "" fmt "", ##__VA_ARGS__), false))

#define ME_ERROR(fmt, ...) me_error_api->default_assert->error(me_error_api->default_assert->instance, __FILE__, __LINE__, "" fmt "", ##__VA_ARGS__)

#define ME_FATAL(fmt, ...) me_error_api->default_assert->fatal(me_error_api->default_assert->instance, __FILE__, __LINE__, "" fmt "", ##__VA_ARGS__)

#define me_assert_api_version ME_VERSION(ME_VERSION_MAJOR, ME_VERSION_MINOR, ME_VERSION_PATCH)

extern struct me_error_api* me_error_api;