#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2024 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////

set(DBUS_LIBRARIES "dbus-1")

re_associate_package(
  PACKAGE_NAME lz4-1.9.3-rev1-linux
  TARGETS lz4
  PACKAGE_HASH 902ee3879b4506d3544017a95441a1163537ab8c8886bc755891b93340f778d8
)
re_associate_package(
  PACKAGE_NAME mimalloc-2.0.6-rev1-linux
  TARGETS mimalloc
  PACKAGE_HASH d54b3f84f886690a0ffcc7013a971e75994b065e317a94041c66403b90e696a1
)