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
#include "core/allocator.h"
#include <mimalloc.h>


static void* system_alloc(void* old_ptr, uint64 old_size, uint64 new_size, const char* filepath, uint32 line) {
  void* ptr = nullptr;
  
  if (new_size) {
    ptr = mi_realloc(old_ptr, new_size);
  } else {
    mi_free(old_ptr);
  }
  
  return ptr;
}

static me_allocator_inst* create_allocator() {
  me_allocator_inst* inst = new  me_allocator_inst{
    .alloc = system_alloc
  };
  
  return inst;
}

static void destroy_allocator(me_allocator_inst* obj) {
  delete obj;
}

static me_allocator_inst system_allocator = {
  .alloc = system_alloc
};

struct me_allocator_api me_allocator = {
  .system = &system_allocator,
  .create = create_allocator,
  .destroy = destroy_allocator
};

struct me_allocator_api* me_allocator_api = &me_allocator;