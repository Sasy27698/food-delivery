#include "Wrapper.h"
int main(int argc, char ** argv) 
{
    int sockfd; //descrittore per comunicare con il ristorante
    int n, id_cliente,checkordine,noOrdine;
    int id_rider,sync,check,scelta;
    char temp_name[100];
    printf("Digita il tuo nome:\t");
    fgets(temp_name,100,stdin);
    printf("BENVENUTO:%s\n",temp_name);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //creo il socket, dove prendo in input 3 parametri: 1-> Il dominio AF_INET (Protocollo IPv4).   
    //2->Il tipo di socket, SOCK_STREAM (canale di trasmissione bidirezionale,sequenziale e affidabile). 3-> Il protocollo, in genere quest'ultimo inizializzato
    // a 0 perchè è indicato dal tipo di socket 
    struct sockaddr_in servadd; //Per assegnare una porta ed un indirizzo alla socket si crea quesra struct
    servadd.sin_family = AF_INET;   //Utilizzo la famiglia di protocolli IPv4
  //oppure fare una scanf cosi che il rider possa scegliere direttamente la porta del ristorante e fare una cosa piu carina
  //printf("inserire porta ristorante:");
  //scanf("%d",&porta);
    servadd.sin_port = htons(1026); //Porta 1026 per comunicare con il ristorante2
    inet_pton(AF_INET, "127.0.0.1", & servadd.sin_addr); //Funzione che converte l'indirizzo espresso da testo a binario
    connect(sockfd, (struct sockaddr * ) & servadd, sizeof(servadd)); //Connessione con il server, prende in input 3 parametri:
    //1-> descrittore del file: "clientsocket" che è il file descriptor ottenuto dalla chiamata precedente socket
    //2->Indirizzo IP del server: "&servaddr" che è l'indirizzo del server.
    //3->Dimensione, cioè, la dimensione della struttura che contiene l'indirizzo IP del Server.
    puts("Connessione con il ristorante riuscita!\n");
    srand(time(NULL));
    id_rider=rand()%1000+1;    //CREAZIONE ID RIDER
    printf("RIDER CON ID: %d\n", id_rider);
        while(1){
            printf("Vedo se ci sono ordini disponibili\n");
            sync = 7;
            FullWrite(sockfd, & sync, sizeof(int)); //invia 0 per sincronizzarsi con il ristorante
            FullRead(sockfd,&checkordine,sizeof(int));
            if(checkordine<=0)
            {
            do{
             puts("Nessun ordine disponibile, premere[5] per aggiornare\n");
            scanf("%d",&noOrdine);
            if (noOrdine != 5)
            puts("Immetti il giusto valore!\n");
            }while(noOrdine!=5);
            }
      else
    {
      printf("-");            
      sync = 0;
      FullWrite(sockfd, & sync, sizeof(int)); //invia 0 per sincronizzarsi con il ristorante
      FullRead(sockfd,&check,sizeof(int));            

if(check!=0) //Se check è incrementato, vuol dire che ce un ordine e lo prendo in carico
            {
                do{
                    printf("Ce un ordine in carico, vuoi provare a prenderlo?\n");
                    puts("[1]Si\n");
                    scanf("%d",&scelta);
                    if (scelta != 1)
                    puts("Immetti il giusto valore!\n");
                    }while(scelta!=1);
            FullWrite(sockfd, & id_rider, sizeof(int));
            FullRead(sockfd,&id_cliente,sizeof(int));
            puts("Consegna in corso.......\n");
            sleep(3);
            printf("Ordine consegnato al cliente:%d",id_cliente);
            exit(0);
            }
        else if (check==0) //Altrimenti se è stato decrementato, vuol dire che è gia stato preso in carico da un altro rider
            {
                puts("Ordine gia preso in carico da un altro rider");
                exit(0);
            }
        else
            {
                puts("Ordine gia preso in carico da un altro rider");
                exit(0);
            }
        }
    }
} //fine main
