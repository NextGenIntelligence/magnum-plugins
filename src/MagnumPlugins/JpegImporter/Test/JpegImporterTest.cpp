/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Directory.h>
#include <Magnum/ColorFormat.h>
#include <Magnum/Trade/ImageData.h>

#include "MagnumPlugins/JpegImporter/JpegImporter.h"

#include "configure.h"

namespace Magnum { namespace Trade { namespace Test {

struct JpegImporterTest: TestSuite::Tester {
    explicit JpegImporterTest();

    void gray();
    void rgb();
};

JpegImporterTest::JpegImporterTest() {
    addTests({&JpegImporterTest::gray,
              &JpegImporterTest::rgb});
}

void JpegImporterTest::gray() {
    JpegImporter importer;
    CORRADE_VERIFY(importer.openFile(Utility::Directory::join(JPEGIMPORTER_TEST_DIR, "gray.jpg")));

    std::optional<Trade::ImageData2D> image = importer.image2D(0);
    CORRADE_VERIFY(image);
    CORRADE_COMPARE(image->size(), Vector2i(3, 2));
    #ifndef MAGNUM_TARGET_GLES2
    CORRADE_COMPARE(image->format(), ColorFormat::Red);
    #else
    CORRADE_COMPARE(image->format(), ColorFormat::Luminance);
    #endif
    CORRADE_COMPARE(image->type(), ColorType::UnsignedByte);
    CORRADE_COMPARE(std::vector<char>(image->data(), image->data()+image->size().product()*image->pixelSize()),
                    (std::vector<char>{'\xff', '\x88', '\x00',
                                       '\x88', '\x00', '\xff'}));
}

void JpegImporterTest::rgb() {
    JpegImporter importer;
    CORRADE_VERIFY(importer.openFile(Utility::Directory::join(JPEGIMPORTER_TEST_DIR, "rgb.jpg")));

    std::optional<Trade::ImageData2D> image = importer.image2D(0);
    CORRADE_VERIFY(image);
    CORRADE_COMPARE(image->size(), Vector2i(3, 2));
    CORRADE_COMPARE(image->format(), ColorFormat::RGB);
    CORRADE_COMPARE(image->type(), ColorType::UnsignedByte);
    /* Data should be similar to the PNG */
    CORRADE_COMPARE(std::vector<char>(image->data(), image->data()+image->size().product()*image->pixelSize()),
                    (std::vector<char>{'\xca', '\xfe', '\x76',
                                       '\xdf', '\xad', '\xb6',
                                       '\xca', '\xfe', '\x76',
                                       '\xe0', '\xad', '\xb6',
                                       '\xc9', '\xff', '\x76',
                                       '\xdf', '\xad', '\xb6'}));
}

}}}

CORRADE_TEST_MAIN(Magnum::Trade::Test::JpegImporterTest)
