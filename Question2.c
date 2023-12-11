#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

void puttftp(char *serverhost, char *filename) {}

//void gettftp(char *serverhost, char *filename) {}

int main(int argc, char *argv[]){
    char *serverhost = argv[1];
    char *filename = argv[2];
    
    if (argc != 3) {
        fprintf(stderr, "ERROR : Usage: %s <serverhost> <filename>\n", argv[0]); //error if number of arguments is not 3.
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints,*res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;      // Use IPv4
    hints.ai_socktype = SOCK_DGRAM; // Datagram socket for TFTP

    int status = getaddrinfo(serverhost, "69", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Extract the address of the server
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, INET_ADDRSTRLEN);
    printf("Server IP: %s\n", ip_str); // Print the IP address

    freeaddrinfo(res); // Free the memory allocated

    puttftp(serverhost, filename);
    //gettftp(serverhost,filename);

    return EXIT_SUCCESS;
}