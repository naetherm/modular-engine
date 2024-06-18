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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/core_types.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
enum me_file_mode_t {
  ME_FILE_MODE_READ = 0,
  ME_FILE_MODE_WRITE,
  ME_FILE_MODE_APPEND
};


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct me_file_stats_o {
  bool is_file;

  bool exists;
};

struct me_file_time_o {
  uint64 time;
};

struct me_file_o {
  uint64 handle;

  bool is_valid;
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

struct me_io_api {

//[-------------------------------------------------------]
//[ Filesystem methods                                    ]
//[-------------------------------------------------------]
  me_file_stats_o (*get_stats)(const char* path);

//[-------------------------------------------------------]
//[ File methods                                          ]
//[-------------------------------------------------------]
  me_file_o (*open_file)(const char* path, int32 mode);
  
  int64 (*read)(me_file_o obj, void* buffer, uint64 readSize);

  bool (*write)(me_file_o obj, const void* buffer, uint64 writeSize);

  uint64 (*get_size)(me_file_o obj);

  void (*close_file)(me_file_o obj);

};


extern struct me_io_api* me_io_api;