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
#include "core/api_registry.h"
#include "core/string_hash.h"
#include "core/log.h"
#include <unordered_map>


namespace internal {
std::unordered_map<uint32, void*> apis;
}


namespace api_registry {
  

static void set(const char* name, void* api) {
  uint32 name_hash = me_string_hash_api->hash(name);
  
  ME_LOG("The hash for '%s' is %d", name, name_hash);
  
  if (internal::apis.contains(name_hash)) {
    // Nothing to do here
  }
}

static void set_or_update(const char* name, void* api) {
  uint32 name_hash = me_string_hash_api->hash(name);
  
  if (internal::apis.contains(name_hash)) {
    // Replace the current implementation
    internal::apis[name_hash] = api;
  }
}

static void remove(const char* name) {
  uint32 name_hash = me_string_hash_api->hash(name);
  
  if (internal::apis.contains(name_hash)) {
    internal::apis.erase(internal::apis.find(name_hash));
  }
}

static void* get(const char* name) {
  uint32 name_hash = me_string_hash_api->hash(name);
  if (internal::apis.contains(name_hash)) {
    return internal::apis.at(name_hash);
  }
  return nullptr;
}

}


static struct me_api_registry_api api = {
  .set = api_registry::set,
  .set_or_update = api_registry::set_or_update,
  .remove = api_registry::remove,
  .get = api_registry::get
};

struct me_api_registry_api* me_api_registry_api = &api;