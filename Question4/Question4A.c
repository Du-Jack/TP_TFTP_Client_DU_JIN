#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TFTP_OPCODE_RRQ 1

// Structure for the RRQ (Read Request) packet
struct TFTPRequestPacket {
    uint16_t opcode;    // Opcode for RRQ
    char filename[512]; // Name of the file to read
    char mode[10];      // Transfer mode (e.g., "octet")
};

// Function to send a RRQ packet to the server
void sendRRQ(int sockfd, struct sockaddr *server_addr, const char *filename) {
    struct TFTPRequestPacket rrqPacket;

    // Construct the RRQ packet
    rrqPacket.opcode = htons(TFTP_OPCODE_RRQ);
    strncpy(rrqPacket.filename, filename, sizeof(rrqPacket.filename));
    strncpy(rrqPacket.mode, "octet", sizeof(rrqPacket.mode));

    // Send the RRQ packet to the server
    sendto(sockfd, &rrqPacket, sizeof(rrqPacket), 0, server_addr, sizeof(struct sockaddr));
}

// Main function for the gettftp program
void gettftp(char *serverhost, char *filename) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;      // Use IPv4
    hints.ai_socktype = SOCK_DGRAM; // Datagram socket for TFTP

    int status = getaddrinfo(serverhost, "1069", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Extract the address of the server
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, INET_ADDRSTRLEN);

    // Create a socket and verify if it has been created
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Connection error with the server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send RRQ to the server
    sendRRQ(sockfd, (struct sockaddr *)ipv4, filename);

    freeaddrinfo(res); // Free the linked list when done with it
    close(sockfd);     // Close the socket when done
}

int main(int argc, char *argv[]){
    
    if (argc != 3) {
        fprintf(stderr, "ERROR : Usage: %s <serverhost> <filename>\n", argv[0]); //error if number of arguments is not 3.
        exit(EXIT_FAILURE);
    }

    char *serverhost = argv[1];
    char *filename = argv[2];

    gettftp(serverhost, filename);

    return EXIT_SUCCESS;
}