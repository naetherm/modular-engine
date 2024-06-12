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
#include "core/frontend.h"
#include <vector>


//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
struct me_display_o {
  me_rect_o shape;
};

struct me_window_o {
  
};


namespace internal {
std::vector<me_display_o> displays;
}

namespace frontend {

static uint32 linux_get_num_displays() {
  return internal::displays.size();
}

static me_display_o* linux_get_display(uint32 index) {
  return &internal::displays[index];
}

static me_rect_o linux_get_display_shape(const me_display_o* obj) {
  return obj->shape;
}

static void linux_update_displays() {

}

}


static struct me_frontend_api linux_frontend = {
  .get_num_displays = frontend::linux_get_num_displays,
  .get_display = frontend::linux_get_display,
  .get_display_shape = frontend::linux_get_display_shape,
  .update_displays = frontend::linux_update_displays
};

struct me_frontend_api* me_frontend_api = &linux_frontend;