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
#include "core/string_hash.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace me_string_hash {

static constexpr uint32 FNV1a_INITIAL_HASH_32 = 0xcbf29ce4u;
static constexpr uint32 FNV1a_MAGIC_PRIME_32 = 0x1000193u;

static uint32 fnv_hash(const char* str) {
  // 32-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
  uint32 hash = FNV1a_INITIAL_HASH_32;
  for (; '\0' != *str; ++str) {
    hash = (hash ^ *str) * FNV1a_MAGIC_PRIME_32;
  }
  return hash;
}

}


static struct me_string_hash_api string_hash = {
  .hash = me_string_hash::fnv_hash
};

struct me_string_hash_api* me_string_hash_api = &string_hash;