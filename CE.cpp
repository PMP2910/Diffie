#include "CE.h"
#include <iostream>
#include <fstream>

using namespace std;

static Point Inf(0,0);

CE::CE(string file)
{
	ifstream fichier(file.c_str()); //on ouvre le fichier en lecture
	
	 
	if(fichier) // si l'ouverture réussi
	{
		string contenu; //déclaration d'une chaîne qui contiendra la ligne
		getline(fichier, contenu); //on met dans "contenu" la ligne
		
		string delimiter = "=";
		size_t pos = 0;
		string token;

		//===================Récupération de p======================
		
		
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}

		mpz_class p(contenu);
		this->p = p;
		
		getline(fichier, contenu); //on met dans "contenu" la ligne
		//==================Récupération de n===================
		pos = 0;
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}

		mpz_class n(contenu);
		this->n = n;
		
		getline(fichier, contenu); //on met dans "contenu" la ligne	
		//=================Récupération de a4=====================
		pos = 0;
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}

		mpz_class a4(contenu);
		this->a4 = a4;
		
		getline(fichier, contenu); //on met dans "contenu" la ligne
		//================Récupération de a6======================
		pos = 0;
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}

		mpz_class a6(contenu);
		this->a6 = a6;
		
		getline(fichier, contenu); //on met dans "contenu" la ligne
		//================RIEN======================
		pos = 0;
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}
		
		getline(fichier, contenu); //on met dans "contenu" la ligne
		//================RIEN======================
		pos = 0;
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}
		
		getline(fichier, contenu); //on met dans "contenu" la ligne
		//================Récupération de gx======================
		pos = 0;
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}

		mpz_class gx(contenu);
		
		getline(fichier, contenu); //on met dans "contenu" la ligne
		//================Récupération de gy======================
		pos = 0;
		while ((pos = contenu.find(delimiter)) != string::npos) {
			contenu.erase(0, pos + delimiter.length());
		}

		mpz_class gy(contenu);
		
		//===============On crée le générateur====================
		Point g(gx, gy);
		this->gen = g;
		
		fichier.close(); // on ferme le fichier
		
	}
	else //sinon
		cerr<< "Impossible d'ouvrir le fichier !"<<endl;
}

CE::CE(mpz_class p, mpz_class n, mpz_class a4, mpz_class a6, Point gen)
{
	this->p = p;
	this->n = n;
	this->a4 = a4;
	this->a6 = a6;
	this->gen = gen;
}

Point CE::add(Point P, Point Q, mpz_class lambda)
{
	mpz_class tmp, xr, yr;
	
	mpz_set (xr.get_mpz_t(), lambda.get_mpz_t());//xr = lambda
	mpz_set (yr.get_mpz_t(), lambda.get_mpz_t()); //yr = lambda
	
	//on calcule xr
	mpz_mul (xr.get_mpz_t(), xr.get_mpz_t(),xr.get_mpz_t()); //lambda^2
	mpz_sub (xr.get_mpz_t(), xr.get_mpz_t(), (P.getX()).get_mpz_t()); //lambda^2 -xp
	mpz_sub (xr.get_mpz_t(), xr.get_mpz_t(), (Q.getX()).get_mpz_t()); //lambda^2 -xp -xq
	mpz_mod (xr.get_mpz_t(), xr.get_mpz_t(), (this->p).get_mpz_t()); // xr modulo p
	
	//on calcule yr
	mpz_neg (tmp.get_mpz_t(), lambda.get_mpz_t());//-lambda
	mpz_mul (tmp.get_mpz_t(), tmp.get_mpz_t(), xr.get_mpz_t()); //-lambda * xr 
	mpz_mul (yr.get_mpz_t(), lambda.get_mpz_t(), (P.getX()).get_mpz_t()); //lambda * xp
																	
	mpz_add (yr.get_mpz_t(), yr.get_mpz_t(), tmp.get_mpz_t()); // (-lambda * xr) + (lambda * xp)
	mpz_sub (yr.get_mpz_t(), yr.get_mpz_t(), (P.getY()).get_mpz_t()); // (-lambda * xr) + (lambda * xp) - yp															
	mpz_mod (yr.get_mpz_t(), yr.get_mpz_t(), (this->p).get_mpz_t()); // yr modulo p
	
	
	return Point(xr, yr);
}

Point CE::doublePoint(Point P)
{
	mpz_class lambda, tmp;
	mpz_class deux(2), trois(3);
	
	
	if (P.getZ())
	{
		Point dP;
		return dP;
	}
	
	//On calcule lambda
	
	//Le numérateur
	mpz_set (lambda.get_mpz_t(), P.getX().get_mpz_t()); //xp
	mpz_mul (lambda.get_mpz_t(), lambda.get_mpz_t(), lambda.get_mpz_t());//xp²
	mpz_mul (lambda.get_mpz_t(), lambda.get_mpz_t(), trois.get_mpz_t());//3*xp²
	mpz_add (lambda.get_mpz_t(), lambda.get_mpz_t(), (this->a4).get_mpz_t());//3*xp² + a4
	
	//Le dénominateur
	mpz_mul (tmp.get_mpz_t(), deux.get_mpz_t(), P.getY().get_mpz_t()); //tmp contient le dénominateur
	
	//afin de faire la division on calcule l'inverse du dénominateur
	mpz_invert (tmp.get_mpz_t(), tmp.get_mpz_t(), (this->p).get_mpz_t());
	
	//Division
	mpz_mul (lambda.get_mpz_t(), lambda.get_mpz_t(), tmp.get_mpz_t());
	//calcul du modulo
	mpz_mod (lambda.get_mpz_t(), lambda.get_mpz_t(), (this->p).get_mpz_t());
	
	
	return add(P, P, lambda);
}


Point CE::addDiffPoints(Point P, Point Q)
{
	mpz_class lambda, tmp;
	
	//calcule de lambda
	mpz_set (lambda.get_mpz_t(), P.getY().get_mpz_t()); //yp
	mpz_sub (lambda.get_mpz_t(), lambda.get_mpz_t(), Q.getY().get_mpz_t()); //yp - yq
	
	//calcul du dénominateur
	mpz_set (tmp.get_mpz_t(), P.getX().get_mpz_t()); //xp
	mpz_sub (tmp.get_mpz_t(), tmp.get_mpz_t(), Q.getX().get_mpz_t()); //xp - xq
	
	//afin de faire la division on calcule l'inverse du dénominateur
	mpz_invert (tmp.get_mpz_t(), tmp.get_mpz_t(), (this->p).get_mpz_t());
	
	//division
	mpz_mul (lambda.get_mpz_t(), lambda.get_mpz_t(), tmp.get_mpz_t());
	//calcul du modulo
	mpz_mod (lambda.get_mpz_t(), lambda.get_mpz_t(), (this->p).get_mpz_t());
	
	return add(P, Q, lambda);	
	
}

Point CE::add(Point P, Point Q)
{
	Point tmp;
	Point res;
	
	if(P.getZ())
	{
		res = Point(Q.getX(),Q.getY());
		res.setZ(Q.getZ());
		return res;
	}
	
	if(Q.getZ())
	{
		res = Point(P.getX(),P.getY());
		res.setZ(P.getZ());
		return res;
	}
	
	else
	{
		if (mpz_cmp(P.getX().get_mpz_t(),Q.getX().get_mpz_t())==0 &&
			mpz_cmp(P.getY().get_mpz_t(),Q.getY().get_mpz_t())==0)
			return doublePoint(P);
		else
			return addDiffPoints(P, Q);
	}
}

Point CE::oppose(Point P)
{
	mpz_class tmp;
	mpz_neg(tmp.get_mpz_t(), P.getY().get_mpz_t());
	mpz_mod (tmp.get_mpz_t(), tmp.get_mpz_t(), (this->p).get_mpz_t());
	Point oppose (P.getX(), tmp);
	
	return oppose;
}
	


bool CE::estSurLaCourbe (Point P)
{
	mpz_class l, r, tmp, deux(2);
	
	mpz_powm (tmp.get_mpz_t(), P.getX().get_mpz_t(), deux.get_mpz_t(), (this->p).get_mpz_t());
	mpz_add(r.get_mpz_t(), (this->a4).get_mpz_t(), tmp.get_mpz_t());
	mpz_mul(r.get_mpz_t(), r.get_mpz_t(), P.getX().get_mpz_t());
	mpz_add(r.get_mpz_t(), r.get_mpz_t(), (this->a6).get_mpz_t());
	mpz_mod (r.get_mpz_t(), r.get_mpz_t(), (this->p).get_mpz_t());
	
	mpz_powm (l.get_mpz_t(), P.getY().get_mpz_t(), deux.get_mpz_t(), (this->p).get_mpz_t());
	
	return mpz_cmp(l.get_mpz_t(),r.get_mpz_t())==0;
}

/*vector<mpz_t> CE::factorise(mpz_t n) 
{
	// on cree un vecteur de mpz_t
	vector<mpz_t> facteurs;
	mpz_t c;
	mpz_set_ui (c, 2);
	while (mpz_cmp_ui (n, 1))
	{
		mpz_t tmp;
		mpz_mul (tmp, c, c);
		if (tmp <= n)
		{
			mpz_mod (tmp, n, c);
			while (tmp == 0)
			{
				facteurs.push_back(c);
				mpz_div (n, n, c);
				mpz_mod (tmp, n, c);
			}
		}
	}
	if (mpz_cmp_ui (n, 1))
		facteurs.push_back(n);

	return facteurs;
}*/

/*int CE::ordre (Point P)
{
	int ordre;
	
	
	
	
	
	return ordre;
}*/


Point CE::mult(mpz_t lambda, Point P)
{
	size_t n = mpz_sizeinbase (lambda, 2);
	Point Q (0, 0);
	Q.setZ(true);
	
	for (int i = n-1; i>=0 ; i--)
	{
		Q = add(Q,Q);
		if (mpz_tstbit(lambda,i))
			Q = add (Q, P);
	}
	
	return Q;
}

Point CE::getGen()
{
	
	return gen;
}

mpz_class CE::getN()
{
	return n;
}

/*int main(void)
{
	Point R, T;
	mpz_class x(2);

	CE ce("w256-001.gp");
	
	R = ce.add(ce.getGen(), ce.getGen());*/
	
	/*cout <<ce.getGen().getX().get_mpz_t()<<endl;*/
	/*Point R(ce.getGen().getX(), ce.getGen().getY());*/
	
	/*T = ce.mult(x.get_mpz_t(), ce.getGen());
	
	cout <<"R = ("<<R.getX()<<", "<<R.getY()<<")"<<endl;
	cout <<"T = ("<<T.getX()<<", "<<T.getY()<<")"<<endl;
		
	return 0;
}*/


