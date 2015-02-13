#ifndef CE_H
#define CE_H

#include "Point.h"
#include <string>
#include <vector>

class CE
{
	mpz_class p, n, a4, a6;
	Point gen;
	
	public:
		CE (string file);
		CE(mpz_class p, mpz_class n, mpz_class a4, mpz_class a6, Point gen);
		Point add(Point P, Point Q, mpz_class lambda);
		bool estSurLaCourbe (Point P);
		Point doublePoint(Point P);
		Point addDiffPoints(Point P, Point Q);
		Point oppose(Point P);
		Point add(Point P, Point Q);
		Point mult(mpz_t lambda, Point P);
		vector<mpz_t> factorise(mpz_t n);
		Point getGen();
		/*mpz_class getP();*/
		mpz_class getN();
	
};

#endif
