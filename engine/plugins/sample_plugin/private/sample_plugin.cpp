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
#include "sample_plugin.h"
#include <core/core_types.h>
#include <core/api_registry.h>
#include <core/log.h>
#include <cstdio>


static void sample_plugin_test_call() {
  ME_LOG("static void sample_plugin_test_call()\n");
}


static struct me_foobar_api me_foobar = {
  .test_call = sample_plugin_test_call
};

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
extern "C" void me_plugin_loader(struct me_api_registry_api* api_registry, bool load) {
  if (load) {
    ME_LOG("Loading external sample_plugin");
    api_registry->set(ME_SAMPLE_PLUGIN_FOOBAR_API, &me_foobar);
  } else {
    ME_LOG("Unloading external sample_plugin");
    api_registry->remove(ME_SAMPLE_PLUGIN_FOOBAR_API);
  }
};