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



//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct me_linux_window_data_t {
  uint64* connection;
  uint64 window;
} me_linux_window_data_t;

typedef struct me_windows_window_data_t {
  uint64 hwnd;
  uint64 window;
} me_windows_window_data_t;

typedef struct me_generic_window_data_t {
  union {
    me_linux_window_data_t linux;
    me_windows_window_data_t windows;
  };
} me_generic_window_data_t;

/**
 * @struct
 * me_window_state_t
 *
 * @brief
 * Type holding all information which represents the state of a window.
 */
typedef struct me_window_state_t {
  /** True if the window is focus */
  bool is_focused;

  /** True if the window is under the cursor */
  bool is_under_mouse;

  /** True if the window is minimized */
  bool is_minimized;

  /** True if the window is maximized */
  bool is_maximized;

  /** True if the window is hidden */
  bool is_hidden;

  /** True if the window now maximized but had previously another state, e.g. hidden or minimized */
  bool was_maximized_before;

  /** Supports whether the window can be hidden */
  bool support_hidden;

  /** Supports whether the window can be maximize in horizontal direction */
  bool support_maximize_horizontal;

  /** Supports whether the window can be maximize in vertical direction */
  bool support_maximize_vertical;

  /** Supports whether the window can be activated */
  bool support_active_window;

  /** Supports whether the window can be moved and resized */
  bool support_move_resize;
} me_window_state_t;


//[-------------------------------------------------------]
//[ API Interface                                         ]
//[-------------------------------------------------------]
/**
 * @struct
 * me_frontend_api
 *
 * @brief
 * API implementation that holds methods for handling and managing everything related to the frontend.
 * This includes collecting and managing information about the available displays on the current system
 * and everything related to window management.
 */
struct me_frontend_api {
  
  //[-------------------------------------------------------]
  //[ Display                                               ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Returns the number of displays.
   *
   * @return
   * The number of displays.
   */
  uint32 (*get_num_displays)();

  /**
   * @brief
   * Returns the display at index position @p index.
   *
   * @param[in] index
   * The display at index position.
   *
   * @return
   * Pointer to the display.
   */
  me_display_o* (*get_display)(uint32 index);

  /**
   * @brief
   * Returns the shape of the given display instance.
   *
   * @param[in] obj
   * Pointer to the display object.
   *
   * @return
   * Instance of the display shape.
   */
  me_rect_o (*get_display_shape)(const me_display_o* obj);

  /**
   * @brief
   * Updates the display information. This updates all information as this can change over run-time.
   * E.g. the display resolution got changed, etc.
   *
   * This has to be called before any other method to the frontend implementation is called.
   */
  void (*update_displays)();


  //[-------------------------------------------------------]
  //[ Window                                                ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Will create a new window instance. Keep in mind that we will not take care about managing any specific
   * information about windows at all, just provide the mechanism to create a window, though we will give
   * our best to clean up all open windows in the end but it is highly recommended to keep track of all
   * opened windows by yourself and take care about their correct destruction.
   *
   * @param[in] title
   * The title of the window to create
   * @param[in] shape
   * The shape of the window to create.
   *
   * @return
   * Pointer to the newly created window. This can be a nullptr.
   */
  me_window_o* (*create)(const char* title, me_rect_o shape);

  /**
   * @brief
   * Destroys the provided window instance.
   *
   * @param[in] window
   * Pointer to the window to destroy.
   */
  void (*destroy)(me_window_o* window);

  /**
   * @brief
   * Updates the given window.
   *
   * @param[in] window
   * Pointer to the window to update.
   */
  void (*update)(me_window_o* window);

  /**
   * @brief
   * Returns the number of all windows, currently created and not destroyed.
   *
   * @return
   * The number of created windows.
   */
  int32 (*get_num_of_windows)();

  /**
   * @brief
   * Call this method if a window should be closed. We will do it this way to set a flag internally on which
   * you can then react. This way additional structured, given in the overlaying window implementation
   * can be destroyed before the window is finally destroyed.
   *
   * @param[in] window
   * Pointer to the window to request closing.
   */
  bool (*requested_closing)(me_window_o* window);

  /**
   * @brief
   * Returns the generic window data of the given window.
   *
   * @param[in] window
   * Pointer to the window for which to get the generic window data.
   *
   * @return
   * The generic window data.
   */
  me_generic_window_data_t (*generic_window_data)(me_window_o* window);

  /**
   * @brief
   * Sets the title of the given window.
   *
   * @param[in] window
   * Pointer to the window.
   * @param[in] title
   * The title of the window.
   */
  void (*set_title)(me_window_o* window, const char* title);

  /**
   * @brief
   * Sets the shape of the window.
   *
   * @param[in] window
   * Pointer to the window.
   * @param[in] shape
   * The shape of the window.
   */
  void (*set_shape)(me_window_o* window, me_rect_o shape);

  /**
   * @brief
   * Returns the shape of the window.
   *
   * @param[in] window
   * Pointer to the window.
   *
   * @return
   * The shape of the window.
   */
  me_rect_o (*get_shape)(me_window_o* window);

  /**
   * @brief
   * Returns the state of the window.
   *
   * @param[in] window
   * Pointer to the window.
   *
   * @return
   * The state of the window.
   */
  me_window_state_t (*get_state)(me_window_o* window);
};


extern struct me_frontend_api* me_frontend_api;