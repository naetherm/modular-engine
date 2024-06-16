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
#include "core/windows/windows_includes.h"
#include <vector>


struct me_window_o {
  /** The windows window handle */
  HWND hwnd;
  /** The style flags passed on the windows CreateWindowEx() method */
  DWORD style;
  /** The shape of the window, this includes x, y, width, height */
  me_rect_o shape;
  /** The state of the window */
  me_window_state_t state;
  /** Boolean flag that helps during closing a window */
  bool close_requested;
};

namespace internal {
std::vector<me_window_o*> windows;
static bool wnd_class_available = false;

static LRESULT CALLBACK wnd_proc_fun(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static void initialize_window_system() {
  if (wnd_class_available) {
    return;
  }

  const HICON icon = LoadIcon(nullptr, IDI_APPLICATION);

  WNDCLASS wnd_class = { };
  wnd_class.hInstance = GetModuleHandle(nullptr);
  wnd_class.hIcon = icon;
  wnd_class.lpszClassName = TEXT("modular-framework");
  wnd_class.lpfnWndProc = wnd_proc_fun;
  wnd_class.style = CS_OWNDC | CS_DBLCLKS;

  ATOM atom = RegisterClass(&wnd_class);

  WNDCLASS wnd_class_child = { };
  wnd_class_child.hInstance = GetModuleHandle(nullptr);
  wnd_class_child.hIcon = nullptr;
  wnd_class_child.lpszClassName = TEXT("modular-framework-child");
  wnd_class_child.lpfnWndProc = wnd_proc_fun;
  wnd_class_child.style = CS_DBLCLKS;

  atom = RegisterClass(&wnd_class_child);

  wnd_class_available = true;
}

static void destroy_window_system() {

}

// static wnd proc method
static LRESULT CALLBACK wnd_proc_fun(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

  me_window_o* window = reinterpret_cast<me_window_o*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  // Breakout, if there is no window (anymore)
  if (window == nullptr) {
    return DefWindowProc(hwnd, umsg, wparam, lparam);
  }
  switch (umsg) {
    case WM_CLOSE:
    {
      window->close_requested = true;
      break;
    }
    case WM_SIZE:
    {
      break;
    }
  }

  // Done here
  return DefWindowProc(hwnd, umsg, wparam, lparam);
}
}


static me_window_o* windows_create_window(const char* title, me_rect_o shape) {
// Create internal window system
  internal::initialize_window_system();

  // Create the internal window
  me_window_o* window = new me_window_o({0});
  window->close_requested = false;

  // Start filling data to internal window representation and windows related window representation
  window->style = WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

  DWORD window_style = WS_EX_ACCEPTFILES;

  RECT windows_shape;
  SetRect(&windows_shape, (int)shape.x, (int)shape.y, (int)(shape.x + shape.w), (int)(shape.y + shape.h));

  window->hwnd = CreateWindowEx(
    window_style,
    TEXT("modular-engine"),
    title,
    window->style,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    GetModuleHandle(nullptr),
    nullptr
  );

  // We have to store this window through the following method, so we can access it later on in the update proc
  SetWindowLongPtr(window->hwnd, GWLP_USERDATA, (LONG_PTR)window);

  // We can first fill the internal shape of the window here, otherwise the data could be wrong
  RECT window_rect;
  GetWindowRect(window->hwnd, &window_rect);
  window->shape = me_rect_o({(float)window_rect.left, (float)window_rect.top, (float)(window_rect.right - window_rect.left), (float)(window_rect.bottom - window_rect.top)});

  // Now the normal procedure to bring up the window
  SetFocus(window->hwnd);
  SetWindowPos(window->hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
  ShowWindow(window->hwnd, SW_SHOW);

  // And commit the commands
  UpdateWindow(window->hwnd);

  // Push to internal window vector
  internal::windows.push_back(window);
return window;
}

static void windows_destroy(me_window_o* window) {
  HWND temp_hwnd = window->hwnd;
  SetWindowLongPtr(window->hwnd, GWLP_USERDATA, 0);
  DestroyWindow(window->hwnd);

  std::vector<me_window_o*>::iterator iter = std::remove_if(
  internal::windows.begin(),
  internal::windows.end(),
  [&](const auto& val){ return val->hwnd == temp_hwnd; }
  );
  internal::windows.erase(iter);

  window->hwnd = nullptr;

  // If there is no window left, destroy the window system as well
  if (internal::windows.empty()) {
    internal::destroy_window_system();
  }
}

static void windows_update(me_window_o* window) {
  /// TODO(naetherm): Finish this, this is, of course, a starting point for now

  MSG window_msg;
  while (PeekMessage(&window_msg, nullptr, 0, 0, PM_REMOVE) != 0) {
    TranslateMessage(&window_msg);
    DispatchMessage(&window_msg);
  }
}

static int32 windows_get_num_of_windows() {
  return (int32)internal::windows.size();
}

static bool windows_requested_closing(me_window_o* window) {
  return window->close_requested;
}

static me_generic_window_data_t windows_generic_window_data(me_window_o* window) {
  me_generic_window_data_t result;
  
  result.windows.hwnd = (uint64)window.hwnd;
  result.windows.window = (uint64)GetModuleHandle(nullptr);

  return result;
}

static void windows_set_title(me_window_o* window, const char* title) {
  SetWindowText(window->hwnd, title);
}

static void windows_set_shape(me_window_o* window, me_rect_o shape) {

}

static me_rect_o windows_get_shape(me_window_o* window) {
  return window->shape;
}

static me_window_state_t windows_get_state(me_window_o* window) {
  return window->state;
}


static struct me_frontend_api windows_frontend = {
  .create = windows_create_window,
  .destroy = windows_destroy,
  .update = windows_update,

  .get_num_of_windows = windows_get_num_of_windows,
  .requested_closing = windows_requested_closing,
  .generic_window_data = windows_generic_window_data,
  .set_title = windows_set_title,
  .set_shape = windows_set_shape,
  .get_shape = windows_get_shape,
  .get_state = windows_get_state
};


struct me_frontend_api* me_frontend_api = &windows_frontend;