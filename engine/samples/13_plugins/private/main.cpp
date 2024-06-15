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
#include <core/core_types.h>
#include <core/log.h>
#include <core/plugin.h>
#include <application/application.h>

struct me_application_o {
  
};


static me_application_o* this_create() {
  return new me_application_o();
}

static void this_destroy(me_application_o* obj) {
  me_plugin_api->clear();
  delete obj;
}

static void this_update(me_application_o* obj) {
  uint64 plugin_id = me_plugin_api->load("./libsample_plugin.so");
  
  me_plugin_api->unload(plugin_id);
}


struct me_application_api app_api = {
  .create = this_create,
  .destroy = this_destroy,
  .update = this_update
};


int main(int argc, char** argv) {
  me_application_o* app = app_api.create();
  
  app_api.update(app);
  
  app_api.destroy(app);
  
  return 0;
}