#include "Point.h"
#include <iostream>

using namespace std;

Point::Point()
{
	mpz_set_si(_x.get_mpz_t(), 0);
	mpz_set_si(_y.get_mpz_t(), 0);
	_z = false;
}

Point::Point(mpz_class x, mpz_class y)
{
	_x = x;
	_y = y;
	_z = false;
}

Point::Point(int x, int y)
{
	mpz_set_si(_x.get_mpz_t(), x);
	mpz_set_si(_y.get_mpz_t(), y);
	_z = false;
}

Point::Point(const mpz_t x, const mpz_t y)
{
	_x = mpz_class (x);
	_y = mpz_class (y);
	_z = false;
	
}

mpz_class Point::getX()
{
	return _x;
}

mpz_class Point::getY()
{
	return _y;
}

Point::~Point(void)
{
	
}

void Point::setZ(bool infini)
{
	_z = infini;
}

bool Point::getZ()
{
	return _z;
}
