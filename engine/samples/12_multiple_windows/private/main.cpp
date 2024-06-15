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
#include <core/frontend.h>


struct me_frontend_api;


//[-------------------------------------------------------]
//[ Main entry point                                      ]
//[-------------------------------------------------------]
int main(int argc, char** argv) {
  me_window_o* window1 = me_frontend_api->create("my window", { 0, 0, 800, 600 });
  me_window_o* window2 = me_frontend_api->create("my second window", { 100, 100, 1024, 768 });

  //me_frontend_api->set_shape(window1, {0, 0, 1024, 768});

  while (me_frontend_api->get_num_of_windows() >= 1) {
    if (window1)
      me_frontend_api->update(window1);
    if (window2)
      me_frontend_api->update(window2);

    if (window1 && me_frontend_api->requested_closing(window1)) {
      me_frontend_api->destroy(window1);

      window1 = nullptr;
    }

    if (window2 && me_frontend_api->requested_closing(window2)) {
      me_frontend_api->destroy(window2);

      window2 = nullptr;
    }
  }
  return 0;
}