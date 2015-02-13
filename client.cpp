#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <gmpxx.h>
#include "CE.h"
#include "random.h"
#include <iostream>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{

//-----------------INITIALISATION DES SOCKETS--------------------------- 
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Connexion réussie\n");
//**********************************************************************

//---------------------CALCUL DE B DE BOB ET ENVOIE---------------------    
    //Sélection de la courbe
	CE ce("w256-001.gp");
	
	Point B;
	//Sélection de la clé secrète
	mpz_t b;
	mpz_init(b);
	gmp_randstate_t b_state;
	gmp_randinit_mt (b_state);
	random_seeding(b_state);
	mpz_urandomm(b , b_state ,ce.getN().get_mpz_t());
	gmp_randclear(b_state);
	cout <<"b="<<b<<endl;
	
	B = ce.mult(b, ce.getGen());
	
	cout <<"-----calcul et envoie de B-----"<<endl;
	char buffer[256];
	//envoie de Bx
    bzero(buffer,256);
    mpz_get_str (buffer, 10, B.getX().get_mpz_t());
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    
    //envoie de By
    bzero(buffer,256);
    mpz_get_str (buffer, 10, B.getY().get_mpz_t());
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
//**********************************************************************

//----------------------RECEPTION DE A DE ALICE-------------------------         
    mpz_t x, y;
    
    cout <<"-----reception de A-----"<<endl;
    //reception de Ax
     bzero(buffer,256);
     n = read(sockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Ax: %s\n",buffer);
     mpz_init_set_str(x, buffer, 10);
     
     //reception de Ay
     bzero(buffer,256);
     n = read(sockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Ay: %s\n",buffer);
     mpz_init_set_str(y, buffer, 10);
     
      Point A(x, y);
     cout <<"A = ("<<A.getX()<<", "<<A.getY()<<")"<<endl;
     
//**********************************************************************     
     
    //Ferméture de la socket
    close(sockfd);

//--------------------CALCUL DE LA CLE SECRETE PARTAGEE-----------------        
    Point S;
     S = ce.mult(b, A);
     
    cout<<"\nAffichage de la clé secrète S:"<<endl;
    cout <<"S = ("<<S.getX()<<", "<<S.getY()<<")"<<endl;
//**********************************************************************
    
    return 0;
}
