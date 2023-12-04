

char* customerAskForBomber(char* msgScan, char* customerAddress){
    // Le serveur a recu un message de type "looking for bomberstudent servers"
    // Le serveur envoie un message de type "hello i'm a bomberstudent server"
    if (strcmp(msgScan, "looking for bomberstudent servers") == 0) {
        char* msgHello = "hello i'm a bomberstudent server";
        printf("Message recu : %s\n", msgScan);
        printf("Envoi du message : %s\n", msgHello);
        post(customerAddress, msgHello);
    }
}

typedef struct {
    char* action;
    char* status;
    char* message;
    char* infoSup;
} Response;

