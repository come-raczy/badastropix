#include <iostream>
#include <sstream>
#include <string>
#include <limits>

#include "Moffat2DFixture.hh"
#include "models/Moffat2D.hh"

// make the use of CPPUNIT_ASSERT_DOUBLES_EQUAL a bit more user friendly
#define CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(a,b) ( CPPUNIT_ASSERT_DOUBLES_EQUAL(a, b, 8.0D * std::max(std::abs(a), std::abs(b)) * std::numeric_limits<double>::epsilon()) )

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestMoffat2D, registryName("FastIo"));
CPPUNIT_TEST_SUITE_REGISTRATION( TestMoffat2D);

#define M_PI_D 3.14159265358979323846D
#define M_PI_F 3.14159265358979323846

const double BACKGROUND = 0.10D;
const double AMPLITUDE = 12.0D;
const double BETA = 4.1D;
const double X0 = 5.0D;
const double Y0 = 4.0D;
const double SX = 3.0D;
const double SY = 2.0D;
//const double THETA = M_PI_D / 3.0D;
const double THETA = 0.0D;

void TestMoffat2D::setUp()
{
}

void TestMoffat2D::tearDown()
{
}

void TestMoffat2D::testDefaultConstructor()
{
    const models::Moffat2D moffat2D;
    CPPUNIT_ASSERT_EQUAL(0.0D, moffat2D.getBackground());
    CPPUNIT_ASSERT_EQUAL(1.0D, moffat2D.getAmplitude());
    CPPUNIT_ASSERT_EQUAL(4.0D, moffat2D.getBeta());
    CPPUNIT_ASSERT_EQUAL(0.0D, moffat2D.getX0());
    CPPUNIT_ASSERT_EQUAL(0.0D, moffat2D.getY0());
    CPPUNIT_ASSERT_EQUAL(1.0D, moffat2D.getSx());
    CPPUNIT_ASSERT_EQUAL(1.0D, moffat2D.getSy());
    CPPUNIT_ASSERT_EQUAL(0.0D, moffat2D.getTheta());
}

void TestMoffat2D::testConstructor()
{
    const models::Moffat2D moffat2D(BACKGROUND, AMPLITUDE, BETA, X0, Y0, SX, SY, THETA);
    CPPUNIT_ASSERT_EQUAL(BACKGROUND, moffat2D.getBackground());
    CPPUNIT_ASSERT_EQUAL(AMPLITUDE, moffat2D.getAmplitude());
    CPPUNIT_ASSERT_EQUAL(BETA, moffat2D.getBeta());
    CPPUNIT_ASSERT_EQUAL(X0, moffat2D.getX0());
    CPPUNIT_ASSERT_EQUAL(Y0, moffat2D.getY0());
    CPPUNIT_ASSERT_EQUAL(SX, moffat2D.getSx());
    CPPUNIT_ASSERT_EQUAL(SY, moffat2D.getSy());
    CPPUNIT_ASSERT_EQUAL(THETA, moffat2D.getTheta());
}

void TestMoffat2D::testFwhm()
{
    const models::Moffat2D moffat2D_1(BACKGROUND, AMPLITUDE, 1.0, X0, Y0, SX, SY, THETA);
    const models::Moffat2D moffat2D_2(BACKGROUND, AMPLITUDE, 2.0, X0, Y0, SX, SY, THETA);
    CPPUNIT_ASSERT_EQUAL(2.0D*SX, moffat2D_1.getFwhmX());
    CPPUNIT_ASSERT_EQUAL(2.0D*SY, moffat2D_1.getFwhmY());
    const double scale = sqrt(sqrt(2.0D) - 1);
    CPPUNIT_ASSERT_EQUAL(2.0D*SX*scale, moffat2D_2.getFwhmX());
    CPPUNIT_ASSERT_EQUAL(2.0D*SY*scale, moffat2D_2.getFwhmY());
}

void TestMoffat2D::testEvaluation()
{
    {
        // model without any rotation
        const models::Moffat2D moffat2D_1(BACKGROUND, AMPLITUDE, BETA, X0, Y0, SX, SY, 0);
        CPPUNIT_ASSERT_EQUAL(AMPLITUDE + BACKGROUND, moffat2D_1(X0, Y0));
        const double DX = 0.5D * moffat2D_1.getFwhmX();
        const double DY = 0.5D * moffat2D_1.getFwhmY();
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(DX/SX, DY/SY);
        // N * PI/2
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX, Y0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX, Y0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0, Y0 + DY));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0, Y0 - DY));
        // N * PI/6
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX/2.0D, Y0 + DY * sqrt(3.0D)/2.0D));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX/2.0D, Y0 + DY * sqrt(3.0D)/2.0D));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX/2.0D, Y0 - DY * sqrt(3.0D)/2.0D));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX/2.0D, Y0 - DY * sqrt(3.0D)/2.0D));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX * sqrt(3.0D)/2.0D, Y0 + DY/2.0D));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX * sqrt(3.0D)/2.0D, Y0 + DY/2.0D));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX * sqrt(3.0D)/2.0D, Y0 - DY/2.0D));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX * sqrt(3.0D)/2.0D, Y0 - DY/2.0D));
    }
    {
        // model with PI/6 rotation
        const models::Moffat2D moffat2D_1(BACKGROUND, AMPLITUDE, BETA, X0, Y0, SX, SY, M_PI_D / 6.0D);
        CPPUNIT_ASSERT_EQUAL(AMPLITUDE + BACKGROUND, moffat2D_1(X0, Y0));
        const double DX = 0.5D * moffat2D_1.getFwhmX();
        const double DY = 0.5D * moffat2D_1.getFwhmY();
        const double COS = cos(M_PI_D / 6.0D);
        const double SIN = sin(M_PI_D / 6.0D);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(DX/SX, DY/SY);
        // N * PI/2
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX * COS, Y0 + DX * SIN));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DY * SIN, Y0 + DY * COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX * COS, Y0 - DX * SIN));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DY * SIN, Y0 - DY * COS));
        // N * PI/6
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX/2.0D*COS - DY * sqrt(3.0D)/2.0D*SIN, Y0 + DX/2.0D*SIN + DY * sqrt(3.0D)/2.0D*COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX/2.0D*COS - DY * sqrt(3.0D)/2.0D*SIN, Y0 - DX/2.0D*SIN + DY * sqrt(3.0D)/2.0D*COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX/2.0D*COS + DY * sqrt(3.0D)/2.0D*SIN, Y0 + DX/2.0D*SIN - DY * sqrt(3.0D)/2.0D*COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX/2.0D*COS + DY * sqrt(3.0D)/2.0D*SIN, Y0 - DX/2.0D*SIN - DY * sqrt(3.0D)/2.0D*COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX * sqrt(3.0D)/2.0D*COS - DY/2.0D*SIN, Y0 + DX * sqrt(3.0D)/2.0D*SIN + DY/2.0D*COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX * sqrt(3.0D)/2.0D*COS - DY/2.0D*SIN, Y0 - DX * sqrt(3.0D)/2.0D*SIN + DY/2.0D*COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 + DX * sqrt(3.0D)/2.0D*COS + DY/2.0D*SIN, Y0 + DX * sqrt(3.0D)/2.0D*SIN - DY/2.0D*COS));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_DEFAULT(AMPLITUDE / 2.0D + BACKGROUND, moffat2D_1(X0 - DX * sqrt(3.0D)/2.0D*COS + DY/2.0D*SIN, Y0 - DX * sqrt(3.0D)/2.0D*SIN - DY/2.0D*COS));
    }
}


