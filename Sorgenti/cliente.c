#include "Wrapper.h"
#define max 100
// Varie struct
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
//***********************Varie function****************
//Function per la registrazione di un cliente
void signup();

// Function per vedere se gia esiste un cliente
void account_check();
// Function per l'accesso del cliente
void login();
int cerca_ristoranti();
void stampaMenu(int clientsocket, int numProdotti);
void stampaBevande(int clientsocket, int numBibite);
//****************************************************
//********Diverse variabili globali, dichiarate per favorire una migliore leggibilità del codice**********
int i,j=0,id,syncro,porzioni;
int id_rider;
int success = 0, x, serv, choice;
char temp_name[max];
Cliente s[max];
int total = 0, food_choice, n;
int scelta_ris, search_choice, confirm;
int ch, food_choice,id_client,drink_choice;
int nProdotti;
//********************************************
int main()
{
    while (1) {
        printf("BENVENUTO AL FOOD DELIVERY DI SALVATORE DE ROSA E MARIO DE LEO!\n");
        printf("**********************************\n*  MATRICOLE:                    *\n*  0124001981->SALVATORE DE ROSA *\n*  0124001945->MARIO DE LEO      *\n**********************************\n");
        printf("IMMETTERE UNA DELLE SEGUENTI OPERAZIONI\n");
        printf("\n\n1)REGISTRAZIONE\n");
        printf("2)ACCESSO\n3)USCITA\n\n");
        printf("Fai la tua scelta!\t");
        scanf("%d", &choice);

        switch (choice) {

        // Per la Function Signup
        case 1: {
            signup();
            break;
        }

        // Per il Login Function
        case 2: {
            login();
            break;
        }
        case 3: {
            exit(1);
        }
        default: {
                printf("\nDigita il giusto valore\n");
                break;
                 }
        }
    }
        
}
// Function che crea un nuovo utente
void signup()
{
    printf("Digita il tuo nome:\t");
    getchar();
    fgets(temp_name,sizeof(temp_name),stdin);
    account_check();
}
// Function che vede se gia esiste un nome utente
void account_check()
{
    for (i = 0; i < 100; i++) {
        // Confronta se ce gia un nome
       if (!(strcmp(temp_name,s[i].uname))) {
            printf("\n\nAccount gia esistente, effettua l'accesso con questo nome o cambia il tuo username!\n\n");
            main();
            break;
        }
    }

    // Se il nome utente non esiste ne crea uno!
    if (i == 100) {
        strcpy(s[j].uname, temp_name);
        j++;
        printf("\n\n\nAccount creato con successo\n\n");
    }
}
// Function per il login
void login()
{
    printf("\n\nLOGIN\n\n");
    printf("\nDigita il tuo nome:\t");
    getchar();
    fgets(temp_name,sizeof(temp_name),stdin);
    for (i = 0; i < 100; i++) {
        // Vede se il nome utente gia esiste
        if (!strcmp(s[i].uname, temp_name)) {
                system("clear");
                printf("\n\nBenvenuto %s\n Ti fornisco due scelte\n1)Cerca ristorante\n2)Esci\n",s[i].uname);
    srand(time(NULL));
    id_client = rand() % 1000 + 1; //Genero l'ID del cliente attraverso un rand
    printf("QUESTO E IL TUO ID: %d\n", id_client);
    printf("Digita la tua scelta:%s\t", s[i].uname);
    scanf("%d", &search_choice);
                // Attraverso uno switch richiama la ricerca del ristorante
                switch (search_choice) {
                case 1: {
                    cerca_ristoranti();
                    break;
                }
                case 2: {
                    exit(1);
                }
                default: {
                    printf("Immetti il giusto valore\n\n");
                    break;
                }
                }
                break;
            }
        else {
            printf("\n\nAccount non esiste, prima registrati!");
            main();
            break;
        }
            }
        }
// Function che implementa le connessioni e ricerca dei ristoranti
int cerca_ristoranti()
{
    rist IDRistorante;
    int numProdotti,numBibite;
    int clientsocket;
	Prodotto prodotti;
    struct sockaddr_in servaddr;    //Per assegnare una porta ed un indirizzo alla socket si crea quesra struct
    clientsocket = Socket(AF_INET,SOCK_STREAM,0); //Creazione della socket, prende in input 3 parametri:1-> Il dominio AF_INET (Protocollo IPv4).   
    //2->Il tipo di socket, SOCK_STREAM (canale di trasmissione bidirezionale,sequenziale e affidabile). 3-> Il protocollo, in genere quest'ultimo inizializzato
    // a 0 perchè è indicato dal tipo di socket 
    servaddr.sin_family=AF_INET;    //Utilizzo la famiglia di protocolli IPv4
    servaddr.sin_port=htons(1024); //Utilizziamo la porta di comunicazione 1024 per la comunicazione con il server
    inet_pton(AF_INET, "127.0.0.1", & servaddr.sin_addr); //Funzione che converte l'indirizzo espresso da testo a binario
    Connect(clientsocket,(struct sockaddr *)&servaddr,sizeof(servaddr)); //Connessione con il server, prende in input 3 parametri:
    //1-> descrittore del file: "clientsocket" che è il file descriptor ottenuto dalla chiamata precedente socket
    //2->Indirizzo IP del server: "&servaddr" che è l'indirizzo del server.
    //3->Dimensione, cioè, la dimensione della struttura che contiene l'indirizzo IP del Server.
    syncro=1;    
    FullWrite(clientsocket, &syncro, sizeof(int)); //invia numero per la sincronizzazione con il server
    FullWrite(clientsocket,&s,sizeof(Cliente));   //Invio "s" che è il nome del cliente
    /*Invio al server la scelta effettuata dall'utente (che in questo caso
    è solo la visualizzazione dei ristoranti */
    FullRead(clientsocket,&n,sizeof(n));
    if(n<=0){
    puts("Nessun ristorante disponibile al momento");
     syncro=8;
        FullWrite(clientsocket,&syncro,sizeof(int));
        exit(1);
   }
   else{
    for(i=0;i<n;i++){
                        FullRead(clientsocket,&IDRistorante,sizeof(rist));
                        printf("%d)Il ristorante connesso e' il seguente:%s\n",i+1,IDRistorante.name);
                    }
    }
        printf("\nPuoi scegliere tra i ristoranti sopra citati utilizzando il loro ID\n\n");
        printf("Fai la tua scelta\t");
        scanf("%d", &scelta_ris);
        syncro=2;
        FullWrite(clientsocket,&syncro,sizeof(int));        //Invio 2 per sincronizzarmi con la richiesta
        FullWrite(clientsocket,&scelta_ris,sizeof(n));      //Invio la scelta del ristorante
        FullRead(clientsocket,&numProdotti,sizeof(int));    //Leggo il numero di prodotti
        stampaMenu(clientsocket,numProdotti);
        printf("Effettua la scelta\n");
        scanf("%d",&food_choice);
        puts("Quante porzioni?\n");
        scanf("%d",&porzioni);
        if (food_choice == 4) {
            syncro=8;
            FullWrite(clientsocket,&syncro,sizeof(int));    //Se non mi piace nessuno dei prodotti, esco
            exit(1);
    }
    else
        puts("***DOPO CHE HAI SCELTO IL PIATTO, ORA SCEGLI COSA VUOI BERE***\n");
        FullRead(clientsocket,&numBibite,sizeof(int));
        stampaBevande(clientsocket,numBibite);
        puts("Digita l'ID corretto!\n");
        scanf("%d",&drink_choice);
        syncro=4;
        FullWrite(clientsocket,&syncro,sizeof(int));
        FullWrite(clientsocket,&food_choice,sizeof(int));
        FullWrite(clientsocket,&porzioni,sizeof(int));
        FullWrite(clientsocket,&drink_choice,sizeof(int));  //Con questi ultimi 4 FullWrite invio le scelte effettuate
        puts("Il tuo ordine e' andato a buon fine!\n");
        puts("Resta in linea finche non ricevi l'ID del Rider!\n");
        while(1)        //Attraverso un ciclo infinito, attendo l'ID del rider
            {
                FullRead(clientsocket,&id_rider,sizeof(int)); 
                printf("Il rider con ID:%d presto inviera il prodotto richiesto\n",id_rider);
                printf("Adesso inviero il tuo id al server!\n");        
                FullWrite(clientsocket,&id_client,sizeof(int));
                puts("\nArrivederci, buona giornata e torni a trovarci presto!\n");
                syncro=8;
                FullWrite(clientsocket,&syncro,sizeof(int));
                close(clientsocket);    //Ricevuta la conferma d'ordine, mi spengo
            }
}
void stampaMenu(int clientsocket, int numProdotti){ //Function che richiama i prodotti del ristorante scelto
	int j;
	Prodotto prodotti;
    printf("\n\t  PIATTI DISPONIBILI\n*****************\n");
    printf("    Nome\n");
    for (j = 0; j < numProdotti; j++) {
      FullRead(clientsocket, & prodotti, sizeof(Prodotto));
      printf("IDProdotto:%d  %s \n", j + 1, prodotti.nome);
    }
}
void stampaBevande(int clientsocket, int numBibite){ //Function che richiama le bevande del ristorante scelto
	int k;
	Bibite bevande;
    printf("\n\t  BIBITE DISPONIBILI\n*****************\n");
    printf("    Nome\n");
    for (k = 0; k < numBibite; k++) {
      FullRead(clientsocket, & bevande, sizeof(Bibite));
      printf("IDProdotto:%d  %s \n", k + 1, bevande.nomeb);
    }
}
