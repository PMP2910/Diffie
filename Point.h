#ifndef POINT_H
#define POINT_H
#include <gmpxx.h>

using namespace std;

class Point
{
	mpz_class _x, _y;
	bool _z;
	
	public:
		Point();
		Point(mpz_class x, mpz_class y);
		Point(int x, int y);
		Point(const mpz_t x, const mpz_t y);
		~Point(void);
		mpz_class getX(void);
		mpz_class getY(void);
		Point add(Point P, Point Q, mpz_class l);
		void setZ(bool infini);
		bool getZ();
};

#endif
