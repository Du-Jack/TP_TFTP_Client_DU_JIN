#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

#include <arpa/inet.h>

void gettftp(char *serverhost, char *filename) {
    
}

void puttftp(char *serverhost, char *filename) {
    
}


int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <serverhost> <filename>\n", argv[0]); //error if number of arguments is not 3.
        exit(EXIT_FAILURE);
    }

    char *serverhost = argv[1];
    char *filename = argv[2];

    puttftp(serverhost, filename);
    gettftp(serverhost,filename);

    return EXIT_SUCCESS;
}