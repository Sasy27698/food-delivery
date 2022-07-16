#include "Wrapper.h"
#define max 100
//**********STRUCT PER LA CREAZIONE E RICEZIONE DEI DATI IMMESSI DAGLI ALTRI FILE.C********
struct cliente {
    char uname[max];
};
typedef struct cliente Cliente;
struct ristorante {
    char name[max];
};
typedef struct ristorante rist;
struct prodotto {
  char nome[max];
};
typedef struct prodotto Prodotto;
struct bibite {
    char nomeb[max];
};
typedef struct bibite Bibite;
//*********************************//
//*********LINKED LIST ESCLUSIVE DEL SERVER ATTE A UNA MAGGIOR COORDINAZIONE****************
struct Server_Ristoranti {         //ESCLUSIVO DEL SERVER
  rist ristorante;
  int descriptor;
  struct Server_Ristoranti * next;
};
typedef struct Server_Ristoranti SERVER_RISTORANTI;
struct listaclienti         //ESCLUSIVO DEL SERVER
{
    Cliente clienti;
    int descriptor;
    struct listaclienti *next;
};
typedef struct listaclienti LISTACLIENTI;
//****************************QUESTA LISTA È GESTITA SOLAMENTE DAL SERVER, SERVE A GESTIRE LE RICHIESTE EFFETTUATE E A MONITORARE I FD
struct lista //Questa struct serve a gestire le richieste effettuare
{            //rispettivamente non solo dai clienti ma anche dai ristoranti tramite i loro FD
    int FDC;
    rist ristorante;
    Prodotto prodotti;
    int FDR;
    int Stato_Operazione;
    struct lista *next;
    struct lista *prec;
};
typedef struct lista LISTA;
//****************************
//**********************************************************
//**********PROTOTIPI DI LISTE VUOTE E DI FUNCTION ATTE A TROVARE I CORRISPETTIVI CLIENTI O RISTORANTI***********
LISTACLIENTI *crea_listaC();
SERVER_RISTORANTI *creaListRistoranti();
SERVER_RISTORANTI *trova_ristoranti(SERVER_RISTORANTI ** testa, int posizione);
LISTA *crea_lista(); 
LISTA * trovarichiesta(LISTA ** head, int fd, int identificativo, int stato);
//****************************
//****************************VARIE FUNZIONI ATTE A AGGIUNGERE O ELIMINARE DETERMINATE OPERAZIONI NELLE LISTE
void ins_testa(rist info_ristoranti,struct lista **p_testa, int fd); //Il server man mano che riceverà i nomi dei ristoranti li inserira in una lista ordinata, fd è il suo identificativo
void aggiungi_Ristoranti(SERVER_RISTORANTI ** testa, rist ris, int fd);
void aggiungiC(LISTACLIENTI ** testa2, Cliente cli, int fd);
void aggiungere_Richiesta(int client, int ristorante);
void eliminazione_ristorante(SERVER_RISTORANTI **p_punt, int id);
void eliminazione_cliente(LISTACLIENTI **p_punt, int id);
void eliminazione_richiesta(LISTA ** punt);
//*****************************//
//********Diverse variabili globali, dichiarate per favorire una migliore leggibilità del codice**********
int n_Ristoranti=0;
int nProdotti;
int nBevande;
int nclienti=0;
//****************************//
LISTA * testa, * p_punt_richiesta, * fine; 
int main(int argc,char **argv)
{
    int i,j,k,idRider,idClient,avviso,sync,avviso2;
    int food_choice,drink_choice,quantita;
    rist IDRistorante;
    Cliente idcliente;
    Prodotto prodotti;
    Bibite bevande;
    testa=crea_lista();
    fine=testa;
    SERVER_RISTORANTI *head, *p_punt;
    LISTACLIENTI *head2,*pp_punt;
    head2=crea_listaC();
    head=creaListRistoranti();
    struct sockaddr_in servadd;     //Per assegnare una porta ed un indirizzo IF alla socket si crea questa variabile strutturaa.
    int listenfd,connfd,sockfd;
    listenfd = Socket(AF_INET,SOCK_STREAM,0); //creo il socket, dove prendo in input 3 parametri: 1-> Il dominio AF_INET (Protocollo IPv4).   
    //2->Il tipo di socket, SOCK_STREAM (canale di trasmissione bidirezionale,sequenziale e affidabile). 3-> Il protocollo, in genere quest'ultimo inizializzato
    // a 0 perchè è indicato dal tipo di socket 
    //in sto caso af_inet non prendo l'altro perche chi diamine ha i permessi, il sock stream se non erro è il canale di
    //trasmissione bidirezionale quindi ottimo per ricevere e tornare (nel caso di errore rivedere sto passaggio) e 0 è implicitamente indicato dal socket
    servadd.sin_family=AF_INET;         //Utilizzo la famiglia di protocolli IPv4
    servadd.sin_addr.s_addr=htonl(INADDR_ANY);  //Utilizziamo "INADDR_ANY" (valore pari a 0.0.0.0) per accettare le comunicazioni da tutti i client, la funzione "htonl" prende un numero a 32 bit e lo converte dal formato della macchina a quello della rete
    servadd.sin_port=htons(1024);           //Utilizziamo la porta di comunicazione 1024, la funzione "htons" prende un numero a 16 bit e lo converte nel formato della macchina a quello della rete
    //rider_disponibili.tv_sec=1; //Da provare dopo
    Bind(listenfd,(struct sockaddr *)&servadd,sizeof(servadd)); //Il bind serve per assegnare un indirizzo al socket e prende
    //3 parametri che sono 1-> il file descriptor utilizzato nella socket precedente, ossia listenfd, 2-> l indirizzo locale e 3-> la dimensione che contiene l'indirizzo
    printf("\nIn attesa di connessioni...\n");
    Listen(listenfd,1024); //mette il server in ascolto e prende due parametri, 1-> sempre il descrittore "listenfd" e la capienza massima di connessioni
    //puts("ciao, sono una stringa per vedere se il server è in ascolto");
    //Inizializzazione campo select
    fd_set rset;
    fd_set allset;
    int client[FD_SETSIZE];
    int maxd=listenfd;
    int maxi=-1;
    for (i=1;i<FD_SETSIZE;i++)
        client[i] =-1;
    client[0] = listenfd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddr_len;
    int connd,sockd,ready;
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);
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
    for (i = 0; i <= maxi; i++) {
      if ((sockd = client[i]) < 0)
        continue;
      if (FD_ISSET(sockd, & rset))
      {
		//Ad ogni connessione, indipendente dal client, riceve un messaggio
		//Questo messaggio contiene il "cosa deve fare", il server
        //Controlla il messaggio in ingresso
        FullRead(sockd, &sync, sizeof(int));
        switch(sync)
        {
        case 0:
            {
                //Riceve la lista dei ristoranti e li aggiunge nella lista
                FullRead(sockd,&IDRistorante,sizeof(rist));
                aggiungi_Ristoranti( & head, IDRistorante, sockd);
                printf("Si è connesso il ristorante:%s\n",IDRistorante.name);
                break;
            }
        case 1:
            {   //Riceve la lista cliente (dove legge anche il loro id) e li aggiunge nella lista, dove successivamente invia la lista dei ristoranti connessi
                puts("Un cliente si e' connesso!\n");
                FullRead(sockd,&idcliente,sizeof(Cliente));
                aggiungiC(&head2,idcliente,sockd);
                printf("Si è connesso il cliente %s\n",idcliente.uname);
                FullWrite(sockd,&n_Ristoranti,sizeof(int));
                if(n_Ristoranti > 0){
                p_punt=head;
                for(i=0;i<n_Ristoranti;i++){
                FullWrite(sockd,&p_punt->ristorante,sizeof(rist));
                if(i!=n_Ristoranti -1)
                  p_punt = p_punt -> next;
                                          } 
                                   }
                puts("Sono stati inviati i ristoranti al cliente!\n");
                break;
            }
        case 2:
            {
                //QUI INVIO LA RICHIESTA DEI PRODOTTI
                puts("Aggiungo richiesta\n");
                FullRead(sockd,&i,sizeof(i));
                p_punt = trova_ristoranti(&head,i);
                aggiungere_Richiesta(sockd, p_punt -> descriptor);//gli passiamo il canale di comunicazione del client e del ristorante
                avviso=8; //numero totalmente a caso per avvisare il ristorante dei prodotti
                FullWrite(p_punt -> descriptor,&avviso,sizeof(int));
                break;
            }
        case 3:
            {   //Qui invio la lista dei prodotti
                FullRead(sockd,&nProdotti,sizeof(int));
                //trova la richiesta che sta in stato 3 e prenditi il file descriptor del client
	            p_punt_richiesta = trovarichiesta(&testa,sockd,1,3);
	            //invia il numero di prodotti
	            FullWrite(p_punt_richiesta -> FDC, & nProdotti, sizeof(int));
	            for (j = 0; j < nProdotti; j++) 
                    {
    	                FullRead(sockd, & prodotti, sizeof(Prodotto));
    	                FullWrite(p_punt_richiesta -> FDC, & prodotti, sizeof(Prodotto));
                    }
                avviso2=6;
                FullWrite(p_punt->descriptor,&avviso2,sizeof(int));
                break;
            }
        case 6:
            { //Qui invio la lista delle bevande
              FullRead(sockd,&nBevande,sizeof(int));
	          FullWrite(p_punt_richiesta -> FDC, & nBevande, sizeof(int));
	          for (k = 0; k < nBevande; k++) 
                {
    	            FullRead(sockd,&bevande,sizeof(Bibite));
    	            FullWrite(p_punt_richiesta->FDC,&bevande,sizeof(Bibite));
                }
	          p_punt_richiesta->Stato_Operazione = 4;
	          //imposta richiesta stato a 4
	          printf("Prodotti del ristorante mostrati al client.\n");
            }
        case 7:
            {
                puts("Lettura ID Rider in corso...\n");
                break;
            }
        case 4:
            {   //Qui il server inoltrerà le scelte effettuate del cliente al ristorante
                FullRead(sockd,&food_choice,sizeof(int));
                FullRead(sockd,&drink_choice,sizeof(int));
                FullRead(sockd,&quantita,sizeof(int));
                int invio_ordine;
                invio_ordine=9;
                // trova la richiesta con stato 4 e prenderà il file descriptor del ristorante
	            p_punt_richiesta = trovarichiesta(&testa,sockd,0,4);
                FullWrite(p_punt_richiesta->FDR,&invio_ordine, sizeof(int));
	            FullWrite(p_punt_richiesta->FDR,&food_choice,sizeof(int));
                FullWrite(p_punt_richiesta->FDR, & drink_choice,sizeof(int));
                FullWrite(p_punt_richiesta->FDR,&quantita,sizeof(int));
	            p_punt_richiesta -> Stato_Operazione = 5;
	            //imposta lo stato a 5
	            printf("Il client ha effettuato l'ordinazione ");
                puts("ed è stata inviata al ristorante.\n\n");
                break;
            }
        case 5:
            {   //In questo case il server inoltrera l'id del rider al cliente ed effettuera la consegna
                puts("Prima di consegnare, prendo l'ID del rider e l'ID del cliente!\n");
                FullRead(sockd,&idRider,sizeof(int));
                //trova la richiesta a stato 5 e prenderà il FD del Cliente
	            p_punt_richiesta = trovarichiesta(&testa,sockd,1,5);
	            FullWrite(p_punt_richiesta->FDC,&idRider,sizeof(int));
	            FullRead(p_punt_richiesta->FDC,&idClient,sizeof(int));
	            FullWrite(sockd,&idClient,sizeof(int));
                printf("Il Rider:%d sta consegnando al cliente:%d........\n",idRider,idClient);
                sleep(3);
                printf("Consegna effettuata!!!!!\n");
                eliminazione_richiesta(&p_punt_richiesta); //elimina la richiesta
                break;
            }
        case 8:
            {
	          //il client si è disconnesso
	          if (close(sockd) == -1)
	            perror("Errore nella close");
	          FD_CLR(sockd, & allset);
	          client[i] = -1;
	          eliminazione_cliente(&head2, sockd);
              puts("Un cliente si è disconnesso\n");
	          break;
            }
	    case 9:
            {
	          //il ristorante si è disconnesso
	          if (close(sockd) == -1)
	            perror("Errore nella close");
	          FD_CLR(sockd, & allset);
	          client[i] = -1;
	          eliminazione_ristorante(&head, sockd);
              puts("Un ristorante si è disconnesso\n");
	          break;
	        }
        default:
                break;
        }
        // se la coda finisce chiudi
        sync = -1;
        if (--ready <= 0)
          break;

        }
    }
    }
}
LISTA *crea_lista()
{   //Crea la lista vuota
    LISTA *testa;
    testa=NULL;
    return testa;
}
void aggiungi_Ristoranti(SERVER_RISTORANTI **testa, rist ris, int fd) { //Inserisce il dato nella testa della lista
  SERVER_RISTORANTI *ptr;
  ptr = calloc(1,sizeof(SERVER_RISTORANTI));
  strcpy(ptr -> ristorante.name, ris.name);
  ptr -> descriptor = fd;
  ptr -> next = *testa;
  *testa = ptr;
  n_Ristoranti++;
}
LISTA * trovarichiesta(LISTA **head, int fd, int identificativo, int stato) { //l'identificativo serve per indicare di chi e' fd, stato indica lo stato dell'ordine
  LISTA *trova;
  trova = *head;

  while ((trova != NULL) && (identificativo == 1)) {
      if (trova -> FDR == fd && trova -> Stato_Operazione == stato) {
        return trova;
    } else {
      if (trova -> FDC == fd && trova -> FDC == stato)
        return trova;
    }

    trova = trova -> next;

    }    
}
void aggiungere_Richiesta(int client, int ristorante) //Se la lista è vuota, aggiungera le richiesta del cliente o ristorante in testa, oppure aggiorna dato dopo dato il nodo
{
    if(testa == NULL)
        {
            LISTA *ptr;
            ptr = calloc(1,sizeof(LISTA));
            ptr -> FDC = client;
            ptr -> FDR = ristorante;
            ptr -> Stato_Operazione = 3;
            ptr -> next = testa;
            ptr -> prec = NULL;
            testa = ptr;
            fine = testa;
        }
    else 
        {
            LISTA *ptr;
            ptr = calloc(1,sizeof(LISTA));
            ptr -> FDC = client;
            ptr-> FDR = ristorante;
            ptr -> Stato_Operazione= 3;
            fine -> next = ptr;
            ptr -> next = NULL;
            ptr -> prec = fine;
            fine = ptr;
        }
}
SERVER_RISTORANTI * trova_ristoranti(SERVER_RISTORANTI **testa, int posizione) 
{ //Funzione per trovare il ristorante richiesto dal cliente
  SERVER_RISTORANTI *trova;
  trova = *testa;
  int i=0;
  while (i < posizione - 1) {
    trova = trova -> next; 
    i++;
  }
  return trova;

}
SERVER_RISTORANTI * creaListRistoranti() 
{ //Creazione lista vuota della lista ristoranti
  SERVER_RISTORANTI *testa;
  testa = NULL;
  return testa;
}
void eliminazione_ristorante(SERVER_RISTORANTI **p_punt, int id) //Eliminazione del ristorante, se si trova in testa, entrerà nel primo if, altrimenti trova i successivi e lo elimina
{
    SERVER_RISTORANTI *ptr,*prec;
    ptr=*p_punt;
    if(ptr-> descriptor == id)
        {
            ptr=(*p_punt)->next;
            *p_punt = ptr;
        }
    else{
    while((ptr->next) != NULL){
    if(ptr->descriptor == id)
        break;
    prec=ptr;
    ptr=ptr->next;
    (*p_punt)->next=ptr->next;
        }
        prec->next=ptr->next;
    }
    n_Ristoranti--;
}
void eliminazione_richiesta(LISTA ** punt) 
{ //Eliminazione della richiesta, se si trova in testa, entrerà nel primo if, altrimenti trova i successivi e lo elimina
   LISTA *ptr;
   if ((*punt) == testa)
        {
            ptr= (*punt)->next;
            free(ptr);
            testa = ptr;
        }
  else 
        {
            (*punt)->prec->next=(*punt)->next;
            (*punt)->next->prec=(*punt)->prec;
            free(punt);
        }
}
LISTACLIENTI *crea_listaC()     //Inizializzo la lista clienti
{
    LISTACLIENTI *testa;
    testa=NULL;
    return testa;
}
void aggiungiC(LISTACLIENTI ** testa2, Cliente cli, int fd) //Aggiungo i clienti alla lista
{
  LISTACLIENTI *ptr;
  ptr = calloc(1,sizeof(LISTACLIENTI));
  strcpy(ptr -> clienti.uname, cli.uname);
  ptr -> descriptor = fd;
  ptr -> next = *testa2;
  * testa2  = ptr;
  nclienti++;
}
void eliminazione_cliente(LISTACLIENTI **p_punt, int id) //Eliminazione dei clienti, se si trova in testa, entrerà nel primo if, altrimenti trova i successivi e lo elimina
{
    LISTACLIENTI *ptr,*prec;
    ptr=*p_punt;
    if(ptr-> descriptor == id)
        {
            ptr=(*p_punt)->next;
            *p_punt = ptr;
        }
    else{
    while((ptr->next) != NULL){
    if(ptr->descriptor == id)
        break;
    prec=ptr;
    ptr=ptr->next;
    (*p_punt)->next=ptr->next;
        }
        prec->next=ptr->next;
    }
    nclienti--;
}
