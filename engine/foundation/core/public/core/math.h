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


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
static inline float vec2_len(me_vec2_o& a);

static inline me_vec2_o vec2_add(me_vec2_o& a, me_vec2_o& b);

static inline me_vec2_o vec2_sub(me_vec2_o& a, me_vec2_o& b);

static inline me_vec2_o vec2_mul(me_vec2_o& a, float t);

static inline me_vec2_o vec2_div(me_vec2_o& a, float t);

static inline me_vec2_o vec2_muladd(me_vec2_o& a, me_vec2_o& b, float t);


static inline float vec3_len(me_vec3_o& a);

static inline me_vec3_o vec3_add(me_vec3_o& a, me_vec3_o& b);

static inline me_vec3_o vec3_sub(me_vec3_o& a, me_vec3_o& b);

static inline me_vec3_o vec3_mul(me_vec3_o& a, float t);

static inline me_vec3_o vec3_div(me_vec3_o& a, float t);


#include "core/math.inl"