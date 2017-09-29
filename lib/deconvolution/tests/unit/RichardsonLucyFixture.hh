#ifndef UNIT_TESTS_RICHARDSON_LUCY_FIXTURE_HH
#define UNIT_TESTS_RICHARDSON_LUCY_FIXTURE_HH

#include <cppunit/extensions/HelperMacros.h>

#include <string>

namespace deconvolution {

class TestRichardsonLucy : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( TestRichardsonLucy );
    CPPUNIT_TEST( testConstructor );
    CPPUNIT_TEST( testInvariant );
    CPPUNIT_TEST_SUITE_END();
private:
public:
    void setUp();
    void tearDown();
    void testConstructor();
    void testInvariant();
};

} // namespace deconvolution

#endif // #ifndef UNIT_TESTS_RICHARDSON_LUCY_FIXTURE_HH


