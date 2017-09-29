#ifndef UNIT_TESTS_MOFFAT_2_D_FIXTURE_HH
#define UNIT_TESTS_MOFFAT_2_D_FIXTURE_HH

#include <cppunit/extensions/HelperMacros.h>

#include <string>

class TestMoffat2D : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( TestMoffat2D );
    CPPUNIT_TEST( testDefaultConstructor );
    CPPUNIT_TEST( testConstructor );
    CPPUNIT_TEST( testFwhm );
    CPPUNIT_TEST( testEvaluation );
    CPPUNIT_TEST_SUITE_END();
private:
public:
    void setUp();
    void tearDown();
    void testDefaultConstructor();
    void testConstructor();
    void testFwhm();
    void testEvaluation();
};

#endif // #ifndef UNIT_TESTS_MOFFAT_2_D_FIXTURE_HH

