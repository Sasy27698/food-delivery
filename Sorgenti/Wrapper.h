#include <stdio.h>      //Libreria Standard di Input/Output
#include <stdlib.h>     //Libreria Standard
#include <string.h>     //Libreria per le Stringhe
#include <unistd.h>     //Libreria Standard di Unix
#include <sys/types.h>  //Libreria per i types
#include <sys/socket.h> //Libreria per gestire i Socket
#include <sys/time.h>   //Libreria per gestire i tempi
#include <arpa/inet.h>  //Libreria per convertire gli indirizzi IP
#include <errno.h>      //Libreria per gli errori di sistema
#include <signal.h>     //Libreria per i segnali
#include <syslog.h>     //Libreria per le funzioni di sistema syslog
#include <pthread.h>    //Libreria per la gestione dei thread

//Create a new socket of type TYPE in domain DOMAIN, using protocol PROTOCOL.
//If PROTOCOL is zero, one is chosen automatically.
int Socket(int family,int type,int protocol)
{
        int n;

                if((n = socket(family,type,protocol)) < 0)
                {
                        perror("Socket Error");
                        exit(1);
                }

        return n;

}
//Open a connection on socket FD to peer at ADDR (which LEN bytes long).
//For connectionless socket types, just the default address to send to
//and the only address from which to accept trasmissions.
//Return 0 on success, -1 for errors.
int Connect(int sockfd,struct sockaddr *servaddr,socklen_t addrlen)
{

        int n;

                if((n = connect(sockfd,servaddr,addrlen)) < 0)
                {
                        perror("Connect Error");
                        exit(1);
                }

        return n;

}
//Give the socket FD the local address ADDR (which is LEN bytes long).
int Bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen)
{

        int n;

                if((n = bind(sockfd,addr,addrlen)) < 0)
                {
                        perror("Bind Error");
                        exit(1);
                }

        return n;

}
//listen() marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept
//On success, zero is returned. On error, -1 is returned, and errno is set appropriately. 
int Listen(int sockfd,int listen_fd)
{
        int n;
    
                if((n = listen(sockfd,listen_fd)) < 0)
                {
                        perror("Listen Error");
                        exit(1);
                }

        return n;
}
//Prepare to accept connections on socket FD.
//When a connection arrives, open a new socket to communicate with it,
//set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
//peer and *ADDR_len to the address's actual length, and return the new socket's descriptor, or -1 for errors.
int Accept(int sockfd,struct sockaddr *clientaddr,socklen_t *addr_dim)
{
        int n;

                if((n = accept(sockfd,clientaddr,addr_dim)) < 0)
                {
                        perror("Accept Error");
                        exit(1);
                }

        return n;
}
//Convert from presentation format of an Internet number in buffer starting at CP to the binary network format and store
//result for interface type AF in buffer starting at BUF.
int Inet_pton(int af,const char *src, void *dst)
{
        int n;
        if ((n = inet_pton(af,src,dst)) < 0)
        {
            perror("PTON ERROR");
            exit(-1);
        }
  return n;
}
//select() allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation
int Select(int maxfdp, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
        int n;
            if((n = select(maxfdp,readfds,writefds,exceptfds,timeout))<0)
            {
                perror("Errore nella select!");
                exit(1);
            }
return n;
}
//Creo attraverso una fork un processo figlio
pid_t Fork()
{
        pid_t pid;

                    if((pid = fork()) < 0)
                    {
                            perror("Fork Error");
                            exit(-1);
                    }

        return pid;
}
//Scrive count byte senza poter essere interrotto da una system call
ssize_t FullWrite(int fd,const void *buf,size_t count)
{
        size_t  nleft;
        ssize_t nwritten;
        nleft = count;
        while(nleft > 0)
        {
                    if((nwritten = write(fd,buf,nleft)) < 0)
                    {
                            if(errno == EINTR)
                            {
                            continue;
                            }
                            else
                            {
                            exit(nwritten);
                            }
                    }
        nleft -= nwritten;
        buf += nwritten;
        }
        return(nleft);
}
//Legge count byte senza poter essere interrotto da una system call
ssize_t FullRead(int fd,void *buf,size_t count)
{
        size_t  nleft;
        ssize_t nread;
        nleft = count;
        while(nleft > 0)
        {   
            if((nread = read(fd,buf,nleft))<0)
            {
                if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                exit(nread);
                }
            }
            else if(nread == 0)
            {
            break;
            }
            nleft -= nread;
            buf += nread;
            }
        buf=0;
        return(nleft);
}
