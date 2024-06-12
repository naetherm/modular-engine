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
#include "core/os.h"
#include <dlfcn.h>
#include <unistd.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
namespace os {

static me_dynlib_o linux_open(const char* path) {
  void* handle = dlopen(path, RTLD_LAZY);
  
  return (me_dynlib_o){
    .handle = (uint64)handle,
    .is_valid = handle != nullptr
  };
}

static void linux_close(me_dynlib_o obj) {
  if (obj.is_valid) {
    dlclose((void*)obj.handle);
  }
}

static me_dynlib_o linux_fetch(const char* path) {
  void* handle = dlopen(path, RTLD_NOW | RTLD_NOLOAD);
  
  return (me_dynlib_o){
    .handle = (uint64)handle,
    .is_valid = handle != nullptr
  };
}

static void* linux_get_symbol(me_dynlib_o obj, const char* symbol) {
  if (obj.is_valid) {
    return dlsym((void*)obj.handle, symbol);
  }
  return nullptr;
}


static uint32 linux_num_logical_processors() {
  return sysconf(_SC_NPROCESSORS_ONLN);
}

static const char* linux_shared_library_prefix() {
  return "lib";
}

static const char* linux_shared_library_suffix() {
  return ".so";
}

}

static struct me_dynlib_api dynlib = {
    .open = os::linux_open,
    .close = os::linux_close,
    .fetch = os::linux_fetch,
    .get_symbol = os::linux_get_symbol
};

static struct me_system_info_api system_info = {
    .num_logical_processors = os::linux_num_logical_processors,
    .shared_library_prefix = os::linux_shared_library_prefix,
    .shared_library_suffix = os::linux_shared_library_suffix
};

static struct me_os_api linux_os = {
    .dynlib = &dynlib,
    .system_info = &system_info
};

struct me_os_api* me_os_api = &linux_os;