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


struct me_format_api {
  int (*printf)(char *buffer, const char *fmt, ...);

  int (*print)(char* buffer, int count, const char* fmt, ...);

  int (*vprintf)(char *buffer, const char *fmt, va_list va);

  int (*vprint)(char* buffer, int count, const char* fmt, va_list vaList);
};
#define me_sprintf(buffer, fmt, ...) me_format_api->printf(buffer, fmt, ##__VA_ARGS__)

#define me_snprintf(buffer, count, fmt, ...) me_format_api->print(buffer, count, fmt, ##__VA_ARGS__)

#define mme_vsprintf(buffer, fmt, va) me_format_api->vprintf(buffer, fmt, va)

#define me_vsnprintf(buffer, count, fmt, va) me_format_api->vprint(buffer, count, fmt, va)

#define me_format_api_version ME_VERSION(ME_VERSION_MAJOR, ME_VERSION_MINOR, ME_VERSION_PATCH)

extern struct me_format_api* me_format_api;