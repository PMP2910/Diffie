#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "CE.h"
#include "random.h"
#include <iostream>
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	
	
//-----------------INITIALISATION DES SOCKETS---------------------------
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
    
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
//**********************************************************************

//-----------------RECEPTION DE B DE BOB--------------------------------   
     char buffer[256];
     mpz_t x, y;
     cout<<"----reception de B----"<<endl;
     //reception de Bx
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Bx: %s\n",buffer);
     mpz_init_set_str(x, buffer, 10);
     
     //reception de By
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("By: %s\n",buffer);
     mpz_init_set_str(y, buffer, 10);
     
     Point B(x, y);
     
     cout <<"B = ("<<B.getX()<<", "<<B.getY()<<")"<<endl;
//**********************************************************************

//-----------CALCUL ET ENVOIE DE A DE ALICE-----------------------------     
    
    cout<<"----calcul et envoie de A----"<<endl; 
       //Sélection de la courbe
	CE ce("w256-001.gp");
	
	Point A;
	//Sélection de la clé secrète
	mpz_t a;
	mpz_init(a);
	gmp_randstate_t r_state;
	gmp_randinit_mt (r_state);
	random_seeding(r_state);
	mpz_urandomm(a , r_state ,ce.getN().get_mpz_t());
	gmp_randclear(r_state);
	cout <<"a="<<a<<endl;
	
	 A = ce.mult(a, ce.getGen());
     
    //envoie de Ax
     bzero(buffer,256);
     mpz_get_str (buffer, 10, A.getX().get_mpz_t());
      n = write(newsockfd,buffer,255);
     if (n < 0) error("ERROR writing to socket");
     
     //envoie de Ay
     bzero(buffer,256);
     mpz_get_str (buffer, 10, A.getY().get_mpz_t());
      n = write(newsockfd,buffer,255);
     if (n < 0) error("ERROR writing to socket");
     
//**********************************************************************    
     //Ferméture de sockets   
     close(newsockfd);
     close(sockfd);

//--------------------CALCUL DE LA CLE SECRETE PARTAGEE-----------------     
     Point S;
     S = ce.mult(a, B);
     
     cout<<"\nAffichage de la clé secrète S:"<<endl;
     cout <<"S = ("<<S.getX()<<", "<<S.getY()<<")"<<endl;
//**********************************************************************
     
     return 0; 
}
