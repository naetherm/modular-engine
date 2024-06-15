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
#include "core/plugin.h"
#include "core/api_registry.h"
#include "core/os.h"
#include "core/string_hash.h"
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace internal {

struct p_plugin_o {
  char* path;
  char* name;
  
  me_dynlib_o dynlib;
  me_plugin_load_func* load_func;
};

std::vector<p_plugin_o*> plugins;

}

namespace me_plugin {

static uint64 load(const char* path) {
  internal::p_plugin_o* plugin = new internal::p_plugin_o();
  
  plugin->dynlib = me_os_api->dynlib->open(path);
  plugin->load_func = (me_plugin_load_func*)me_os_api->dynlib->get_symbol(plugin->dynlib, "me_plugin_loader");
  
  plugin->load_func(me_api_registry_api, true);
  
  internal::plugins.push_back(plugin);
  
  return internal::plugins.size() - 1;
}

static void unload(uint64 plugin_id) {
  internal::p_plugin_o* plugin = internal::plugins.at(plugin_id);
  
  
  plugin->load_func(me_api_registry_api, false);
  plugin->load_func = nullptr;
  
  me_os_api->dynlib->close(plugin->dynlib);
 
  internal::plugins.at(plugin_id) = nullptr;
  
  delete plugin;
}

static void clear() {
  for (auto* plugin: internal::plugins) {
    //if (plugin) {
      delete plugin;
    //}
  }
  
  internal::plugins.clear();
}

}


static struct me_plugin_api plugin = {
  .load = me_plugin::load,
  .unload = me_plugin::unload
};

struct me_plugin_api* me_plugin_api = &plugin;