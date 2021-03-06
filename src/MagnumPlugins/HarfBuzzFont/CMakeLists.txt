#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013, 2014, 2015
#             Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

find_package(Magnum REQUIRED Text)
find_package(Freetype REQUIRED)
find_package(HarfBuzz REQUIRED)

# Avoid warning spam with old-style casts
include_directories(SYSTEM ${FREETYPE_INCLUDE_DIRS} ${HARFBUZZ_INCLUDE_DIRS})

set(HarfBuzzFont_SRCS
    HarfBuzzFont.cpp)

set(HarfBuzzFont_HEADERS
    HarfBuzzFont.h)

# Objects shared between plugin and test library
add_library(HarfBuzzFontObjects OBJECT
    ${HarfBuzzFont_SRCS}
    ${HarfBuzzFont_HEADERS})
if(NOT BUILD_STATIC OR BUILD_STATIC_PIC)
    set_target_properties(HarfBuzzFontObjects PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# HarfBuzzFont plugin
add_plugin(HarfBuzzFont ${MAGNUM_PLUGINS_FONT_DEBUG_INSTALL_DIR} ${MAGNUM_PLUGINS_FONT_RELEASE_INSTALL_DIR}
    HarfBuzzFont.conf
    $<TARGET_OBJECTS:HarfBuzzFontObjects>
    pluginRegistration.cpp)
if(BUILD_STATIC_PIC)
    set_target_properties(HarfBuzzFont PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

target_link_libraries(HarfBuzzFont
    ${MAGNUM_LIBRARIES}
    ${MAGNUM_TEXT_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${HARFBUZZ_LIBRARIES})
if(CORRADE_TARGET_WINDOWS)
    target_link_libraries(HarfBuzzFont FreeTypeFont)
endif()

install(FILES ${HarfBuzzFont_HEADERS} DESTINATION ${MAGNUM_PLUGINS_INCLUDE_INSTALL_DIR}/HarfBuzzFont)

if(BUILD_GL_TESTS)
    add_library(MagnumHarfBuzzFontTestLib STATIC $<TARGET_OBJECTS:HarfBuzzFontObjects>)
    target_link_libraries(MagnumHarfBuzzFontTestLib
        ${MAGNUM_LIBRARIES}
        ${MAGNUM_TEXT_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${HARFBUZZ_LIBRARIES}
        MagnumFreeTypeFontTestLib)
    add_subdirectory(Test)
endif()
