#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 516
#define TFTP_OPCODE_WRQ 2
#define TFTP_OPCODE_DATA 3
#define TFTP_OPCODE_ACK 4
#define RRQ_MODE "octet"
#define SEND_ERROR "Error sending WRQ packet"

// Structure for the WRQ (Write Request) packet
struct TFTPWriteRequestPacket {
    uint16_t opcode;    // Opcode for WRQ
    char filename[512]; // Name of the file to write
    char mode[10];      // Transfer mode (e.g., "octet")
};

// Function to send a WRQ packet to the server
void sendWRQ(int sockfd, struct sockaddr *server_addr, const char *filename) {
    char wrq_buffer[MAX_BUFFER_SIZE];
    wrq_buffer[0] = 0x00;  // Opcode (2 bytes)
    wrq_buffer[1] = TFTP_OPCODE_WRQ;
    memcpy(wrq_buffer + 2, filename, strlen(filename));  // Filename
    wrq_buffer[2 + strlen(filename)] = 0;  // Null-terminate the filename
    memcpy(wrq_buffer + 2 + strlen(filename) + 1, RRQ_MODE, strlen(RRQ_MODE));  // Mode
    wrq_buffer[2 + strlen(filename) + 1 + strlen(RRQ_MODE)] = 0;  // Null-terminate the mode
    int wrq_length = 2 + strlen(filename) + 1 + strlen(RRQ_MODE) + 1;

    write(STDOUT_FILENO, wrq_buffer, wrq_length);
    write(STDOUT_FILENO, "\n", 1);

    // Send the WRQ packet
    if (sendto(sockfd, wrq_buffer, wrq_length, 0, server_addr, sizeof(struct sockaddr)) == -1) {
        perror(SEND_ERROR);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "ERROR: Usage: %s <serverhost> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *serverhost = argv[1];
    char *filename = argv[2];

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;      // Use IPv4
    hints.ai_socktype = SOCK_DGRAM; // Datagram socket for TFTP

    int status = getaddrinfo(serverhost, "1069", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Create a socket and verify its creation
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket creation error");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    // Send WRQ to the server
    sendWRQ(sockfd, res->ai_addr, filename);

    // Free the linked list when done with it
    freeaddrinfo(res);

    // Close the socket when done
    close(sockfd);

    return EXIT_SUCCESS;
}