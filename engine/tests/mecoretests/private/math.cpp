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
#include "math.h"
#include <core/math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace me_core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Vec2Tests::Vec2Tests()
: UnitTest("Vec2Tests") {

}

Vec2Tests::~Vec2Tests() {

}

void Vec2Tests::test() {
  {
    me_vec2_o a = {1.0f, 2.0f};
    me_vec2_o b = {3.0f, 4.0f};
    {
      me_vec2_o z = vec2_add(a, b);
      re_expect_eq(z.x, 4.0f);
      re_expect_eq(z.y, 6.0f);
    }
    {
      me_vec2_o z = vec2_sub(a, b);
      re_expect_eq(z.x, -2.0f);
      re_expect_eq(z.y, -2.0f);
    }
    {
      me_vec2_o z = vec2_mul(a, 1.0f);
      re_expect_eq(z.x, 1.0f);
      re_expect_eq(z.y, 2.0f);

      z = vec2_mul(a, 2.0f);
      re_expect_eq(z.x, 2.0f);
      re_expect_eq(z.y, 4.0f);
    }
    {
      me_vec2_o z = vec2_div(a, 1.0f);
      re_expect_eq(z.x, 1.0f);
      re_expect_eq(z.y, 2.0f);

      z = vec2_div(a, 2.0f);
      re_expect_eq(z.x, 0.5f);
      re_expect_eq(z.y, 1.0f);
    }
    {
      me_vec2_o z = vec2_muladd(a, b, 2);
      re_expect_eq(z.x, 7.0f);
      re_expect_eq(z.y, 10.0f);
    }
  }
}

Vec3Tests::Vec3Tests()
: UnitTest("Vec3Tests") {
  
}

Vec3Tests::~Vec3Tests() {
  
}

void Vec3Tests::test() {
  {
    me_vec3_o a = {1.0f, 2.0f, 3.0f};
    me_vec3_o b = {4.0f, 5.0f, 6.0f};
    {
      me_vec3_o z = vec3_add(a, b);
      re_expect_eq(z.x, 5.0f);
      re_expect_eq(z.y, 7.0f);
      re_expect_eq(z.z, 9.0f);
    }
    {
      me_vec3_o z = vec3_sub(a, b);
      re_expect_eq(z.x, -3.0f);
      re_expect_eq(z.y, -3.0f);
      re_expect_eq(z.z, -3.0f);
    }
  }
}

re_unittest_autoregister(Vec2Tests)
re_unittest_autoregister(Vec3Tests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // me_core_tests