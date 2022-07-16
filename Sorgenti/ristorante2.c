#include "Wrapper.h"
#define max 100
//**********STRUCT PER LA CREAZIONE E RICEZIONE DEI DATI IMMESSI DAGLI ALTRI FILE.C********
struct ristorante { //struct per settare le informazioni del ristorante
    char name[max];
};
typedef struct ristorante rist;
struct prodotto { //struct per settare le informazioni dei prodotti del ristorante
  char nome[max];
};
typedef struct prodotto Prodotto;
struct bibite {         //Struct per settare le informazioni delle bibite del ristorante
    char nomeb[max];
};
typedef struct bibite Bibite;
struct ordine {     //struct per settare l'ID dell'ordine da parte del cliente
  int stato_Ord;
  struct ordine * next;

}* p_punt, * head, * fine;
typedef struct ordine Ordine;
//*********************************//
Ordine * creaListOrdini();                          //Creazioni lista vuota degli ordini
//*********************************//
//****************************VARIE FUNZIONI ATTE A AGGIUNGERE O ELIMINARE DETERMINATE OPERAZIONI NELLE LISTE
void insl_testa(Ordine ** head, Ordine ordine);
void eliminazione_ordine(Ordine **p_testa);
//*********************************//
//********Diverse variabili globali, dichiarate per favorire una migliore leggibilità del codice**********
char temp_name[max],temp_menu[max],temp_bibite[max];
int numOrdini = 0;
int j;
//********************************//
int main(int argc,char **argv)
{
    int i,m,idRider,server_sync,idClient,quantita_ordine,chiusura,serv;
    int chance,scelta_effettuata=0,numOrdini=0,check,numProdotti,numBibite,sceltaBibita=0;
    rist IDRistorante;
    Ordine ordine;
    head = creaListOrdini();
    numProdotti = 3;
    Prodotto prodotti[numProdotti];
    numBibite = 3;
    Bibite bevande[numBibite];
    printf("Immettere nome ristorante:\n");
    fgets(temp_name,sizeof(temp_name),stdin); //Creiamo il ristorante
    strcpy(IDRistorante.name,temp_name);
    puts("Ora creiamo il menu\n");      //Creiamo i prodotti
    for(i=0;i<numProdotti;i++)
        {
            fgets(temp_menu,sizeof(temp_menu),stdin);
            strcpy(prodotti[i].nome, temp_menu);
            strcpy(prodotti[i].nome, temp_menu);
            strcpy(prodotti[i].nome, temp_menu);
        }
    puts("Ora creiamo le bibite:\n");   //Creiamo le bibite
    for(m=0;m<numBibite;m++)
        {
            fgets(temp_bibite,sizeof(temp_bibite),stdin);
            strcpy(bevande[m].nomeb, temp_bibite);
            strcpy(bevande[m].nomeb, temp_bibite);
            strcpy(bevande[m].nomeb, temp_bibite);
        }
    struct sockaddr_in servadd;                  //Per assegnare una porta ed un indirizzo alla socket si crea quesra struct
    servadd.sin_family=AF_INET;                  //Utilizzo la famiglia di protocolli IPv4
    servadd.sin_addr.s_addr=htonl(INADDR_ANY);  //Utilizziamo "INADDR_ANY" (valore pari a 0.0.0.0) per accettare le comunicazioni da tutti i client, la funzione "htonl" prende un numero a 32 bit e lo converte dal formato della macchina a quello della rete
    servadd.sin_port=htons(1026); //1026 porta per il ristorante-rider
    int listenfd,connfd,sockfd;
    listenfd = Socket(AF_INET,SOCK_STREAM,0); //Creazione della socket, prende in input 3 parametri:1-> Il dominio AF_INET (Protocollo IPv4).   
    //2->Il tipo di socket, SOCK_STREAM (canale di trasmissione bidirezionale,sequenziale e affidabile). 3-> Il protocollo, in genere quest'ultimo inizializzato
    // a 0 perchè è indicato dal tipo di socket 
    //rider_disponibili.tv_sec=1; //Da provare dopo
    Bind(listenfd,(struct sockaddr *)&servadd,sizeof(servadd)); //Il bind serve per assegnare un indirizzo al socket e prende
    //3 parametri che sono 1-> il file descriptor utilizzato nella socket precedente, ossia listenfd, 2-> l indirizzo locale e 3-> la dimensione che contiene l'indirizzo
    printf("\nIn attesa di connessioni...\n");
    Listen(listenfd,1024); //mette il server in ascolto e prende due parametri, 1-> sempre il descrittore "listenfd" e la capienza massima di connessioni
    sockfd = Socket(AF_INET,SOCK_STREAM,0);  //Creazione della socket, prende in input 3 parametri:1-> Il dominio AF_INET (Protocollo IPv4).   
    //2->Il tipo di socket, SOCK_STREAM (canale di trasmissione bidirezionale,sequenziale e affidabile). 3-> Il protocollo, in genere quest'ultimo inizializzato
    // a 0 perchè è indicato dal tipo di socket 
    struct sockaddr_in servaddr;        //Per assegnare una porta ed un indirizzo alla socket si crea quesra struct
    servaddr.sin_family = AF_INET;      //Utilizzo la famiglia di protocolli IPv4
    servaddr.sin_port = htons(1024); //1024 porta per il ristorante-server
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr); //Funzione che converte l'indirizzo espresso da testo a binario
    connect(sockfd, (struct sockaddr * ) & servaddr, sizeof(servaddr)); //Connessione con il server, prende in input 3 parametri:
    //1-> descrittore del file: "clientsocket" che è il file descriptor ottenuto dalla chiamata precedente socket
    //2->Indirizzo IP del server: "&servaddr" che è l'indirizzo del server.
    //3->Dimensione, cioè, la dimensione della struttura che contiene l'indirizzo IP del Server.
    puts("\nConnessione al server...\n");
    serv=sockfd;
    server_sync=0;
    FullWrite(sockfd,&server_sync,sizeof(int));
    FullWrite(sockfd,&IDRistorante,sizeof(rist)); //Invio i dati appena creati al server
    puts("\nSto inviando....\n");
    int servfd;
    //***INIZIALIZZAZIONE CAMPO SELECT***
    serv=servfd;
    fd_set rset;
    fd_set allset;
    int client[FD_SETSIZE];
    int maxd=listenfd;
    int maxi=1;
    if (sockfd>listenfd)
    maxd=sockfd;
    for (i=1;i<FD_SETSIZE;i++)
    client[i] =-1;
    client[0] = listenfd;
    client[1] = sockfd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddr_len;
    int connd;
    int sockd;
    int ready;
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);
    FD_SET(sockfd,&allset);
    //***INIZIO PROTOCOLLO***
    //Posso identificare in due modi che mi vengono in mente per capire far capire al server che sono un ristorante
    //1) uso un send che invia "r" a caso, oppure gli do un identificativo tipo var=1
    while(1)
    {
        rset=allset;
        ready=select(maxd+1,&rset,NULL,NULL,NULL);
        // controlla se il socket di ascolto è leggibile
        if (FD_ISSET(listenfd, & rset)) {
            cliaddr_len = sizeof(cliaddr);
        // invoca la accept
        connd = accept(listenfd, (struct sockaddr * ) & cliaddr, & cliaddr_len);
        // inserisce il socket di connessione in un posto libero di client
        for (i = 0; i < FD_SETSIZE; i++)
            if (client[i] < 0) {
          client[i] = connd;
          break;
        }
      // se non ci sono posti segnala errore
        if (i == FD_SETSIZE)
            perror("troppi client");
        FD_SET(connd, & allset);
        // registra il socket ed aggiorna maxd
        if (connd > maxd)
            maxd = connd;
        if (i > maxi)
            maxi = i;
        if (--ready <= 0)
            continue;
    }
    // controlla tutti i socket di ascolto se sono leggibili
    for (i = 1; i <= maxi; i++) {
      if ((sockd = client[i]) < 0)
        continue;
      if (FD_ISSET(sockd, & rset))
      {
		//Ad ogni connessione, indipendente dal client, riceve un messaggio
		//Questo messaggio contiene il "cosa deve fare", il server
        //Controlla il messaggio in ingresso
        FullRead(sockd, &chance, sizeof(int));
        switch(chance)
        {
        case 8:
            {   //Invio al server i prodotti
                puts("Il server ha bisogno dei miei prodotti!\n");
                server_sync=3;
                FullWrite(sockfd,&server_sync,sizeof(int));
                FullWrite(sockfd,&numProdotti,sizeof(int));
                FullWrite(sockfd,prodotti,sizeof(prodotti));
                puts("\nAspetto la scelta del client....\n");
                break;
            }
        case 6:
            {          //Invio al server le bevande
                puts("Il server ha bisogno delle bevande!\n");
                server_sync=6;
                FullWrite(sockfd,&server_sync,sizeof(int));
                FullWrite(sockfd,&numBibite,sizeof(int));
                FullWrite(sockfd,bevande,sizeof(bevande));
                puts("Aspetto ancora la scelta del cliente....\n");
            }
        case 1:
        //Nel caso si vuole non solo di visualizzare, ma anche di acquistare i piatti disponibili
        {
                puts("\nSto inviando....\n");
                break;
        }
        case 7:
                {
                    if(numOrdini<=0)
                    {
                        printf("Non ci sono ordini per il rider\n");
                    }
                    else
                    printf("\n");
                    FullWrite(sockd,&numOrdini,sizeof(int));
                    break;
}
        case 9:
            {       //Leggo cosa ha scelto il cliente
                FullRead(sockd,&scelta_effettuata,sizeof(int));
                FullRead(sockd,&sceltaBibita,sizeof(int));
                FullRead(sockd,&quantita_ordine,sizeof(int));
                printf("\nIl cliente ha effettuato la scelta, vuole il prodotto con ID:%d\t come bevanda il prodotto con ID:%d\t per %d persone!\n Inoltro la richiesta al rider....\n",scelta_effettuata,quantita_ordine,sceltaBibita);
                insl_testa(&head, ordine);
                numOrdini++;
                p_punt = head;            
                server_sync=5;
                FullWrite(sockfd,&server_sync, sizeof(int));
                break;
            }
        case 0: ////Qui avviene la consegna
            {
                if (p_punt -> stato_Ord == 0) {
                    check++;
                    FullWrite(sockd,&check,sizeof(int));
                    FullRead(sockd,&idRider,sizeof(int)); //Leggo l'ID del rider e lo mando al server
                    printf("Il rider:%d si e connesso e vuole prendere l'incarico di consegnare l'ordine!",idRider);
                    server_sync=7;
                    FullWrite(sockfd,&idRider,sizeof(int));
                    FullWrite(sockfd,&server_sync, sizeof(int));
                    puts("\nHo inviato l'IDRider al server!");
                    FullRead(sockfd, &idClient, sizeof(int));   //Leggo l'ID del cliente
                    FullWrite(sockd, &idClient, sizeof(int));   //Lo mando al rider
                    puts("\nHo inviato l'ID del cliente al rider!");
                    puts("Simulo consegna....\n");
                    sleep(2);
                    printf("Consegna effettuata dal rider: %d al cliente %d.\n", idRider, idClient);
                    numOrdini--;
                    eliminazione_ordine(&head);
                    check--;
                    printf("Ordine effettuato, vuoi continuare la giornata o chiudere baracca?\n");
                    printf("[0]Per chiudere, valore diverso da 0, continui la giornata\n");
                    scanf("%d",&chiusura);
                    if(chiusura==0) //Nel caso il ristorante decide di scollegarsi mandera al server 9 cosi che il server possa eliminarlo dalla lista
                        {
                            server_sync=9;
                            FullWrite(sockfd,&server_sync,sizeof(int));
                            exit(0);
                        }   
                else
                    printf("continuo la mia giornata\n");
                }
                else { //ordine già preso in carico da un altro rider
                        check = 0;
                        FullWrite(sockd, & check, sizeof(int));
                }
            break;
            }
    default:
        break;

        }
        // se la coda finisce chiudi
        chance = -1;
        if (--ready <= 0)
          break;

      }
    }
 }
}
Ordine * creaListOrdini() 
{   //Creo una lista vuota degli ordini
  Ordine * testa;
  testa = NULL;
  return testa;
}

void insl_testa(Ordine **head, Ordine ordine) 
{    //Inserisco l'ordine nella testa
  Ordine *ptr;
  ptr = calloc(1,sizeof(Ordine));
  ptr -> stato_Ord = 0;
  ptr -> next = * head;
  *head = ptr;
}
void eliminazione_ordine(Ordine **p_testa)
{   //Elimino l'ordine dalla lista
    Ordine *ptr;
    ptr=(*p_testa)->next;
    free(ptr);
}
