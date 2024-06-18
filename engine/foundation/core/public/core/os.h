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


//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
struct me_dynlib_o {
  uint64 handle;
  
  bool is_valid;
};


//[-------------------------------------------------------]
//[ API Interface                                         ]
//[-------------------------------------------------------]
/**
 * @struct
 * me_dynlib_api
 *
 * @brief
 * The dynamic library api interface object.
 */
struct me_dynlib_api {
  /**
   * @brief
   * Opens a library given at path.
   *
   * @param[in] path
   * The path of the shared library to load.
   *
   * @return
   * Instance of me_dynlib_o.
   */
  me_dynlib_o (*open)(const char* path);
  
  /**
   * @brief
   * Closes a previously opened shared library.
   *
   * @param[in] obj
   * The library to close.
   */
  void (*close)(me_dynlib_o obj);
  
  /**
   * @brief
   * Loads an already opened shared library with the same path.
   *
   * @param[in] path
   * The path of the library.
   *
   * @return
   * Instance of me_dynlib_o.h
   */
  me_dynlib_o (*fetch)(const char* path);
  
  /**
   * @brief
   * Loads and returns the given symbol from the library obj.
   *
   * @param[in] obj
   * The library.
   * @param[in] symbol
   * The symbol to load.
   *
   * @return
   * Pointer to the loaded symbol. Can be a nullptr if the symbol is not known.
   */
  void* (*get_symbol)(me_dynlib_o obj, const char* symbol);
};


/**
 * @struct
 * me_system_info_api
 *
 * @brief
 * Provides a base set of system information, e.g. the number of logical cores, libraries pre- and suffix, etc.
 */
struct me_system_info_api {
  /**
   * @brief
   * Returns the number of logical processors.
   *
   * @return
   * Number of logical processors.
   */
  uint32 (*num_logical_processors)();
  
  /**
   * @brief
   * Returns the default shared library prefix on the current platform.
   *
   * @return
   * Shared library prefix.
   */
  const char* (*shared_library_prefix)();
  
  /**
    * @brief
    * Returns the default shared library suffix on the current platform.
    *
    * @return
    * Shared library suffix.
    */
  const char* (*shared_library_suffix)();
};


/**
 * @struct
 * me_os_api
 *
 * @brief
 * Mainly just a collection of different, more topic specific, api interfaces.
 */
struct me_os_api {
  /** Pointer to the dynlib api interface */
  struct me_dynlib_api* dynlib;
  /** Pointer to the system info api interface */
  struct me_system_info_api* system_info;
};


extern struct me_os_api* me_os_api;