#include "RichardsonLucyMocks.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <limits>
  
#include "RichardsonLucyFixture.hh"
#include "deconvolution/RichardsonLucy.hh" 
  
// make the use of CPPUNIT_ASSERT_DOUBLES_EQUAL a bit more user friendly
#define CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(a,b) ( CPPUNIT_ASSERT_DOUBLES_EQUAL(a, b, 8.0D * std::max(std::abs(a), std::abs(b)) * std::numeric_limits<double>::epsilon()) )
  
namespace deconvolution {

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestRichardsonLucy, registryName("FastIo"));
CPPUNIT_TEST_SUITE_REGISTRATION( TestRichardsonLucy);

void TestRichardsonLucy::setUp()
{
}

void TestRichardsonLucy::tearDown()
{
}

void TestRichardsonLucy::testConstructor()
{
    const unsigned FLANK = 1;
    const double PSF[] = {0, 1,2,3,4,5,6,7,8};
    Magick::Point IMAGE;
    const deconvolution::RichardsonLucy richardsonLucy(FLANK, PSF, IMAGE);
}

void TestRichardsonLucy::testInvariant()
{
    const unsigned FLANK = 0;
    const double PSF[] = {1};
    Magick::Point IMAGE;
    const deconvolution::RichardsonLucy richardsonLucy(FLANK, PSF, IMAGE);
    richardsonLucy.step();
}

} // namespace deconvolution

