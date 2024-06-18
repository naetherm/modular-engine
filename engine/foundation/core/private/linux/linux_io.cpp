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
#include "core/io.h"
#include <dirent.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace io {

static me_file_stats_o linux_get_stats(const char* path) {
  return (me_file_stats_o){};
}

static me_file_o linux_open_file(const char* path, int32 mode) {

  int file_handle = 0;
  if (mode == me_file_mode_t::ME_FILE_MODE_READ) {
    file_handle = ::open(path, O_RDONLY);
  } else if (mode == me_file_mode_t::ME_FILE_MODE_WRITE) {
    file_handle = ::open(path, O_CREAT | O_RDWR | O_TRUNC, 0777);
  } else if (mode == me_file_mode_t::ME_FILE_MODE_APPEND) {
    file_handle = ::open(path, O_CREAT | O_RDWR, 0777);
  }
  return (me_file_o){
    .handle = (uint64)file_handle,
    .is_valid = file_handle != -1
  };
}

static int64 linux_read(me_file_o obj, void* buffer, uint64 readSize) {
  if (obj.is_valid) {
    return (int64)::read(obj.handle, buffer, readSize);
  }

  return -1;
}

static bool linux_write(me_file_o obj, const void* buffer, uint64 writeSize) {
  if (obj.is_valid) {
    return (int64)::write(obj.handle, buffer, writeSize) != 0;
  }

  return false;
}

static uint64 linux_get_size(me_file_o obj) {
  if (obj.is_valid) {
    struct stat stats;
    fstat(obj.handle, &stats);
    return stats.st_size;
  }

  return 0;
}

static void linux_close_file(me_file_o obj) {
  if (obj.is_valid) {
    ::close(obj.handle);
  }
}

}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]


static struct me_io_api linux_io = {
  .get_stats = io::linux_get_stats,
  // filesystem methods
  // file methods
  .open_file = io::linux_open_file,
  .read = io::linux_read,
  .write = io::linux_write,
  .get_size = io::linux_get_size,
  .close_file = io::linux_close_file
};

struct me_io_api* me_io_api = &linux_io;