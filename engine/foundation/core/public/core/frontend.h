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
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/core_types.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct me_display_o me_display_o;
typedef struct me_window_o me_window_o;


typedef struct me_linux_window_data_t {
  uint64* connection;
  uint64 window;
} me_linux_window_data_t;

typedef struct me_generic_window_data_t {
  union {
    me_linux_window_data_t linux;
  };
} me_generic_window_data_t;

typedef struct me_window_state_t {
  bool is_focused;

  bool is_under_mouse;

  bool is_minimized;

  bool is_maximized;

  bool is_hidden;

  bool was_maximized_before;

  bool support_hidden;

  bool support_maximize_horizontal;

  bool support_maximize_vertical;

  bool support_active_window;

  bool support_move_resize;
} me_window_state_t;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
struct me_frontend_api {
  
  //[-------------------------------------------------------]
  //[ Display                                               ]
  //[-------------------------------------------------------]
  uint32 (*get_num_displays)();

  me_display_o* (*get_display)(uint32 index);

  me_rect_o (*get_display_shape)(const me_display_o* obj);

  void (*update_displays)();


  //[-------------------------------------------------------]
  //[ Window                                                ]
  //[-------------------------------------------------------]
  me_window_o* (*create)(const char* title, me_rect_o shape);

  void (*destroy)(me_window_o* window);

  void (*update)(me_window_o* window);

  int32 (*get_num_of_windows)();

  bool (*requested_closing)(me_window_o* window);

  me_generic_window_data_t (*generic_window_data)(me_window_o* window);

  void (*set_title)(me_window_o* window, const char* title);

  void (*set_shape)(me_window_o* window, me_rect_o shape);

  me_rect_o (*get_shape)(me_window_o* window);

  me_window_state_t (*get_state)(me_window_o* window);
};


extern struct me_frontend_api* me_frontend_api;