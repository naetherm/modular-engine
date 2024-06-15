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
#include "core/linux/xcb_includes.h"
#include "core/log.h"
#include <algorithm>
#include <vector>
#include <cstring>


//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
struct me_display_o {
  me_rect_o shape;
};

struct me_window_o {
  /** The xcb connection for this window */
  xcb_connection_t* xcb_connection;
  /** Pointer to the screen, this window is placed */
  xcb_screen_t* xcb_screen;
  /** The id of the window */
  xcb_window_t xcb_window;
  /** The ewmh connection */
  xcb_ewmh_connection_t xcb_ewmh;

  /** The shape of the window, this includes x, y, width, height */
  me_rect_o shape;
  /** The state of the window */
  me_window_state_t state;
  /** Boolean flag that helps during closing a window. */
  bool close_requested;
};


namespace internal {
std::vector<me_display_o*> displays;



enum atoms_t {
  WM_DELETE_WINDOW,
  WM_CHANGE_STATE,
  _MOTIF_WM_HINTS,
  CLIPBOARD,
  CLIP_PROP,
  XDND_PROP,
  TARGETS,
  UTF8,
  TEXT,
  STRING,
  PLAIN_UTF8,
  PLAIN,
  URI_LIST,
  XDND_TYPE_LIST,
  XDND_AWARE,
  XDND_SELECTION,
  XDND_ENTER,
  XDND_POSITION,
  XDND_ACTION_COPY,
  XDND_ACTION_MOVE,
  XDND_ACTION_LINK,
  XDND_ACTION_ASK,
  XDND_ACTION_PRIVATE,
  XDND_STATUS,
  XDND_LEAVE,
  XDND_DROP,
  XDND_FINISHED,

  NUM_ATOMS
};

static const char *atoms_names[NUM_ATOMS] = {
  "WM_DELETE_WINDOW",
  "WM_CHANGE_STATE",
  "_MOTIF_WM_HINTS",
  "CLIPBOARD",
  "CLIP_PROP",
  "XDND_PROP",
  "TARGETS",
  "UTF8_STRING",
  "TEXT",
  "STRING",
  "text/plain;charset=utf-8",
  "text/plain",
  "text/uri-list",
  "XdndTypeList",
  "XdndAware",
  "XdndSelection",
  "XdndEnter",
  "XdndPosition",
  "XdndActionCopy",
  "XdndActionMove",
  "XdndActionLink",
  "XdndActionAsk",
  "XdndActionPrivate",
  "XdndStatus",
  "XdndLeave",
  "XdndDrop",
  "XdndFinished"
};



//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
static std::vector<me_window_o*> windows;
static xcb_connection_t* xcb_connection = nullptr;
static xcb_screen_t* xcb_screen = nullptr;
static xcb_generic_error_t *xcb_error = nullptr;
static xcb_generic_event_t *xcb_event = nullptr;
static xcb_atom_t atoms[NUM_ATOMS];


static void initialize_atoms() {
  xcb_intern_atom_cookie_t cookies[NUM_ATOMS];
  for (uint32 i = 0; i < NUM_ATOMS; ++i) {
    cookies[i] = xcb_intern_atom(internal::xcb_connection, 0, strlen(atoms_names[i]), atoms_names[i]);
  }

  for (uint32 i = 0; i < NUM_ATOMS; ++i) {
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(internal::xcb_connection, cookies[i], nullptr);
    if (reply) {
      atoms[i] = reply->atom;
      free(reply);
    } else {
      ME_LOG("Cannot create atom: %hhu\n", xcb_error->error_code);
    }
  }
}

static void initialize_window_system() {
  internal::xcb_connection = xcb_connect(nullptr, nullptr);

  if (xcb_connection_has_error(internal::xcb_connection) != 0) {
    ME_LOG("X Server connection error\n");
  }

  internal::xcb_screen = xcb_setup_roots_iterator(xcb_get_setup(internal::xcb_connection)).data;

  // Create and initialize all atoms
  internal::initialize_atoms();
}

static void destroy_window_system() {
  // Everything should be destroyed up until now, destroy the xcb connection
  if (internal::xcb_connection) {
    xcb_disconnect(internal::xcb_connection);
  }
}


}

namespace frontend {

static uint32 linux_get_num_displays() {
  return internal::displays.size();
}

static me_display_o* linux_get_display(uint32 index) {
  if (internal::displays.size() < index) {
    return internal::displays.at(index);
  }
  return nullptr;
}

static me_rect_o linux_get_display_shape(const me_display_o* obj) {
  return obj->shape;
}

static void linux_update_displays() {
  xcb_connection_t *temp_connection = xcb_connect(nullptr, nullptr);
  if (xcb_connection_has_error(temp_connection) != 0) {
    ME_LOG("[X server] Connection error\n");
    return;
  }
  xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(temp_connection)).data;

  xcb_window_t dummy_window = xcb_generate_id(temp_connection);
  xcb_create_window(temp_connection, 0, dummy_window, screen->root, 0, 0, 1, 1, 0, 0, 0, 0, 0);

  xcb_flush(temp_connection);

  int32 num_displays = 0;
  xcb_randr_get_monitors_cookie_t mon_cookie = xcb_randr_get_monitors(temp_connection, dummy_window, 1);

  uint16 current_width_pixels = screen->width_in_pixels;
  uint16 current_width_mm = screen->width_in_millimeters;

  xcb_randr_get_monitors_reply_t *mon_reply = xcb_randr_get_monitors_reply(temp_connection, mon_cookie, nullptr);
  if (mon_reply == nullptr) {
    ME_LOG("Could not get monitor information from randr\n");
    return;
  }
  xcb_randr_monitor_info_iterator_t mon_it = xcb_randr_get_monitors_monitors_iterator(mon_reply);

  me_display_o *display = new me_display_o({0});
  for (uint32 i = 0; i < mon_reply->nMonitors; ++i) {

    display->shape = (me_rect_o){
      .x = (float)mon_it.data->x,
      .y = (float)mon_it.data->y,
      .w = (float)mon_it.data->width,
      .h = (float)mon_it.data->height
    };

    internal::displays.push_back(display);

    xcb_randr_monitor_info_next(&mon_it);
  }

  xcb_disconnect(temp_connection);
}




static me_window_o* linux_create(const char* title, me_rect_o shape) {
  // If there is no window and the main connection is empty, we have to create it first
  if (internal::windows.empty() && internal::xcb_connection == nullptr) {
    internal::initialize_window_system();
  }

  // Create new window instance
  me_window_o* window = new me_window_o({nullptr});
  internal::windows.push_back(window);
  uint32 screen_num;
  window->xcb_connection = xcb_connect(nullptr, (int *)&screen_num);

  if (xcb_connection_has_error(window->xcb_connection) != 0) {
    ME_LOG("X Server connection error\n");
    return nullptr;
  }

  xcb_screen_iterator_t screen_iter;
  screen_iter = xcb_setup_roots_iterator(xcb_get_setup(window->xcb_connection));
  for (; screen_iter.rem; --screen_num, xcb_screen_next(&screen_iter)) {
    if (screen_num == 0) {
      window->xcb_screen = screen_iter.data;
      break;
    }
  }

  // Create xcb window id
  window->xcb_window = xcb_generate_id(window->xcb_connection);
  window->close_requested = false;

  uint32 value_mask = XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_EVENT_MASK;
  uint32 value_list[3] = { window->xcb_screen->black_pixel, 1, XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW };

  while (shape.x + shape.w >= (float)window->xcb_screen->width_in_pixels) {
    shape.w -= 1.0f;
  }
  while (shape.y + shape.h >= (float)window->xcb_screen->height_in_pixels) {
    shape.h -= 1.0f;
  }

  // Copy updated shape to the window shape
  window->shape = shape;

  xcb_intern_atom_cookie_t *ewmh_cookies = xcb_ewmh_init_atoms(window->xcb_connection, &window->xcb_ewmh);
  if (!xcb_ewmh_init_atoms_replies(&window->xcb_ewmh, ewmh_cookies, nullptr)) {
    ME_LOG("Error during initialization of ewmh atoms\n");
  }

  // Create new window id
  xcb_create_window(
    window->xcb_connection,
    XCB_COPY_FROM_PARENT,
    window->xcb_window,
    window->xcb_screen->root,
    (int16)window->shape.x, (int16)window->shape.y, (uint16)window->shape.w, (uint16)window->shape.h,
    0,
    XCB_WINDOW_CLASS_INPUT_OUTPUT,
    window->xcb_screen->root_visual,
    value_mask,
    value_list);

  // Set the title of the window
  xcb_change_property(window->xcb_connection, XCB_PROP_MODE_REPLACE, window->xcb_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);
  xcb_change_property(window->xcb_connection, XCB_PROP_MODE_REPLACE, window->xcb_window, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING, 8, strlen(title), title);

  xcb_atom_t wm_protocol_list[] = { internal::atoms[internal::WM_DELETE_WINDOW] };

  xcb_icccm_set_wm_protocols(window->xcb_connection, window->xcb_window, window->xcb_ewmh.WM_PROTOCOLS, 1, wm_protocol_list);

  xcb_atom_t window_type[] = { window->xcb_ewmh._NET_WM_WINDOW_TYPE_NORMAL };
  xcb_ewmh_set_wm_window_type(&window->xcb_ewmh, window->xcb_window, 1, window_type);

  xcb_ewmh_get_atoms_reply_t supported_reply;
  uint8_t res = xcb_ewmh_get_supported_reply(&window->xcb_ewmh, xcb_ewmh_get_supported(&window->xcb_ewmh, 0), &supported_reply, nullptr);
  if (res) {
    for (uint32_t i = 0; i < supported_reply.atoms_len; ++i) {
      if (supported_reply.atoms[i] == window->xcb_ewmh._NET_WM_STATE_HIDDEN)
        window->state.support_hidden = true;
      if (supported_reply.atoms[i] == window->xcb_ewmh._NET_WM_STATE_MAXIMIZED_HORZ)
        window->state.support_maximize_horizontal = true;
      if (supported_reply.atoms[i] == window->xcb_ewmh._NET_WM_STATE_MAXIMIZED_VERT)
        window->state.support_maximize_vertical = true;
      if (supported_reply.atoms[i] == window->xcb_ewmh._NET_ACTIVE_WINDOW)
        window->state.support_active_window = true;
      if (supported_reply.atoms[i] == window->xcb_ewmh._NET_WM_MOVERESIZE)
        window->state.support_move_resize = true;
    }
  }

  xcb_map_window(window->xcb_connection, window->xcb_window);

  xcb_flush(window->xcb_connection);

  // Done, return created window
  return window;
}

static void linux_destroy(me_window_o* window) {
  // Remove window from window-"registry"
  xcb_window_t temp_window_id = window->xcb_window;
  xcb_destroy_window(window->xcb_connection, window->xcb_window);
  xcb_disconnect(window->xcb_connection);

  std::vector<me_window_o*>::iterator iter = std::remove_if(
    internal::windows.begin(),
    internal::windows.end(),
    [&](const auto& val){ return val->xcb_window == temp_window_id; }
    );
  internal::windows.erase(iter);

  window->xcb_window = 0;

  // If there is no window left, destroy the window system as well
  if (internal::windows.empty()) {
    internal::destroy_window_system();
  }
}

static void linux_update(me_window_o* window) {
  // Fetch event
  internal::xcb_event = xcb_poll_for_event(window->xcb_connection);

  while (internal::xcb_event) {
    xcb_generic_event_t *next_event = nullptr;
    uint8 event_type = internal::xcb_event->response_type & ~0x80;

    switch (event_type) {
      case XCB_CLIENT_MESSAGE:
        {
          // First case to client message event
          const auto* event = (const xcb_client_message_event_t *)internal::xcb_event;

          if (event->data.data32[0] == internal::atoms[internal::WM_DELETE_WINDOW]) {
            window->close_requested = true;
          } else if (event->type == internal::atoms[internal::XDND_ENTER]) {

          } else if (event->type == internal::atoms[internal::XDND_POSITION]) {

          } else if (event->type == internal::atoms[internal::XDND_DROP]) {

          }

          break;
        }
      case XCB_FOCUS_IN:
        {
          window->state.is_focused = true;
          break;
        }
      case XCB_FOCUS_OUT:
        {
          window->state.is_focused = false;
          break;
        }
      case XCB_CONFIGURE_NOTIFY:
        {
          const auto* event = (const xcb_configure_notify_event_t*)internal::xcb_event;

          xcb_set_input_focus(window->xcb_connection, XCB_INPUT_FOCUS_POINTER_ROOT, window->xcb_window, XCB_TIME_CURRENT_TIME);
          // Just be sure here and set to focused
          window->state.is_focused = true;

          window->shape = (me_rect_o){ .x = (float)event->x, .y = (float)event->y, .w = (float)event->width, .h = (float)event->height };

          xcb_window_t root_window = XCB_WINDOW_NONE;
          // Get the parent window, if there is any
          {
            xcb_query_tree_cookie_t query_tree_cookie = xcb_query_tree(window->xcb_connection, window->xcb_window);
            xcb_query_tree_reply_t *query_reply = xcb_query_tree_reply(window->xcb_connection, query_tree_cookie, nullptr);
            if (query_reply) {
              root_window = query_reply->root;
              free(query_reply);
            }
          }

          if (root_window != XCB_WINDOW_NONE) {
            xcb_translate_coordinates_cookie_t translate_cookie = xcb_translate_coordinates(window->xcb_connection, window->xcb_window, root_window, 0, 0);
            xcb_translate_coordinates_reply_t* translate_reply = xcb_translate_coordinates_reply(window->xcb_connection, translate_cookie, nullptr);
            if (translate_reply) {
              window->shape.x = translate_reply->dst_x;
              window->shape.y = translate_reply->dst_y;
              free(translate_reply);
            }
          }
          break;
        }
      case XCB_LEAVE_NOTIFY:
      {
        window->state.is_under_mouse = false;
        break;
      }
      case XCB_ENTER_NOTIFY:
      {
        window->state.is_under_mouse = true;
        break;
      }
      case XCB_MOTION_NOTIFY:
        {
          const auto* event = (const xcb_motion_notify_event_t *)internal::xcb_event;

          break;
        }
      case XCB_BUTTON_PRESS:
        {
          const auto* event = (const xcb_button_press_event_t*)internal::xcb_event;

          if (event->detail == XCB_BUTTON_INDEX_1) {

          }

          break;
        }
      case XCB_BUTTON_RELEASE:
        {
          const auto* event = (const xcb_button_release_event_t*)internal::xcb_event;

          /// TODO(naetherm): We will require input management
          break;
        }
      case XCB_KEY_PRESS:
      case XCB_KEY_RELEASE:
        {
          if (event_type == XCB_KEY_RELEASE) {
            next_event = xcb_poll_for_queued_event(window->xcb_connection);
            if (next_event && (next_event->response_type & ~0x80) == XCB_KEY_PRESS) {
              auto* key_event = (xcb_key_press_event_t*)internal::xcb_event;
              auto* next_key_event = (xcb_key_press_event_t*)next_event;
              if (next_key_event->time == key_event->time && next_key_event->detail == key_event->detail) {
                internal::xcb_event->response_type = XCB_KEY_PRESS;
                free(next_event);
                next_event = nullptr;
              }
            }
          }
          break;
        }
      default:
        break;
    }

    free(internal::xcb_event);
    if (next_event) {
      internal::xcb_event = next_event;
      next_event = nullptr;
    } else {
      internal::xcb_event = xcb_poll_for_event(window->xcb_connection);
    }
  }

  // Update the internal state of the window
  {
    xcb_get_property_cookie_t cookie = xcb_ewmh_get_wm_state(&window->xcb_ewmh, window->xcb_window);
    xcb_ewmh_get_atoms_reply_t reply;
    bool maximized_h = false;
    bool maximized_v = false;
    window->state.is_hidden = false;
    window->state.is_minimized = false;
    if (xcb_ewmh_get_wm_state_reply(&window->xcb_ewmh, cookie, &reply, nullptr)) {
      for (uint32_t i = 0; i < reply.atoms_len; ++i) {
        if (reply.atoms[i] == window->xcb_ewmh._NET_WM_STATE_MAXIMIZED_HORZ) {
          maximized_h = true;
          window->state.was_maximized_before = false;
        }

        if (reply.atoms[i] == window->xcb_ewmh._NET_WM_STATE_MAXIMIZED_VERT) {
          maximized_v = true;
          window->state.was_maximized_before = false;
        }

        if (reply.atoms[i] == window->xcb_ewmh._NET_WM_STATE_HIDDEN) {
          window->state.is_hidden = true;
          window->state.is_minimized = true;
        }
      }
    }

    window->state.is_maximized = maximized_h && maximized_v;

    window->state.was_maximized_before = window->state.is_minimized && window->state.is_maximized;
  }
}


int32 linux_get_num_of_windows() {
  return internal::windows.size();
}

bool linux_requested_closing(me_window_o* window) {
  return window->close_requested;
}

me_generic_window_data_t linux_generic_window_data(me_window_o* window) {
  me_generic_window_data_t generic_data;
  generic_data.linux.connection = (uint64*)window->xcb_connection;
  generic_data.linux.window = (uint64)window->xcb_window;
  return generic_data;
}

void linux_set_title(me_window_o* window, const char* title) {
  xcb_change_property(window->xcb_connection, XCB_PROP_MODE_REPLACE, window->xcb_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);
}

void linux_set_shape(me_window_o* window, me_rect_o shape) {
  window->shape = shape;

  uint32 win_rect[] = { (uint32)shape.x, (uint32)shape.y, (uint32)shape.w, (uint32)shape.h };
  xcb_configure_window(window->xcb_connection, window->xcb_window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, win_rect);
  xcb_flush(window->xcb_connection);

}

me_rect_o linux_get_shape(me_window_o* window) {
  return window->shape;
}

me_window_state_t linux_get_state(me_window_o* window) {
  return window->state;
}

}


static struct me_frontend_api linux_frontend = {
  .get_num_displays = frontend::linux_get_num_displays,
  .get_display = frontend::linux_get_display,
  .get_display_shape = frontend::linux_get_display_shape,
  .update_displays = frontend::linux_update_displays,
  .create = frontend::linux_create,
  .destroy = frontend::linux_destroy,
  .update = frontend::linux_update,
  .get_num_of_windows = frontend::linux_get_num_of_windows,
  .requested_closing = frontend::linux_requested_closing,
  .generic_window_data = frontend::linux_generic_window_data,
  .set_title = frontend::linux_set_title,
  .set_shape = frontend::linux_set_shape,
  .get_shape = frontend::linux_get_shape,
  .get_state = frontend::linux_get_state
};

struct me_frontend_api* me_frontend_api = &linux_frontend;