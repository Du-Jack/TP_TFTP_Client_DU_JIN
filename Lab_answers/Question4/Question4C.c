#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAX_BUFFER_SIZE 516
#define TFTP_OPCODE_RRQ 1
#define TFTP_OPCODE_DATA 3
#define TFTP_OPCODE_ACK 4
#define RRQ_MODE "octet"
#define SEND_ERROR "Error sending RRQ packet"

// Structure for the RRQ (Read Request) packet
struct TFTPRequestPacket {
    uint16_t opcode;    // Opcode for RRQ
    char filename[512]; // Name of the file to read
    char mode[10];      // Transfer mode (e.g., "octet")
};

// Structure for the ACK packet
struct TFTPAckPacket {
    uint16_t opcode;      // Opcode for ACK
    uint16_t block_number; // Block number
};

// Function to send a RRQ packet to the server
void sendRRQ(int sockfd, struct sockaddr *server_addr, const char *filename) {
    char rrq_buffer[MAX_BUFFER_SIZE];
    rrq_buffer[0] = 0x00;  // Opcode (2 bytes)
    rrq_buffer[1] = 0x01;
    memcpy(rrq_buffer + 2, filename, strlen(filename));  // Filename
    rrq_buffer[2 + strlen(filename)] = 0;  // Null-terminate the filename
    memcpy(rrq_buffer + 2 + strlen(filename) + 1, RRQ_MODE, strlen(RRQ_MODE));  // Mode
    rrq_buffer[2 + strlen(filename) + 1 + strlen(RRQ_MODE)] = 0;  // Null-terminate the mode
    int rrq_length = 2 + strlen(filename) + 1 + strlen(RRQ_MODE) + 1;

    write(STDOUT_FILENO, rrq_buffer, rrq_length);
    write(STDOUT_FILENO, "\n", 1);

    // Send the RRQ packet
    if (sendto(sockfd, rrq_buffer, rrq_length, 0, server_addr, sizeof(struct sockaddr)) == -1) {
        perror(SEND_ERROR);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void receiveFile(int sockfd, struct sockaddr *server_addr, socklen_t server_addr_len, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    uint16_t expected_block = 1;

    while (1) {
        char data_buffer[MAX_BUFFER_SIZE];

        // Receive the DAT packet from the server
        ssize_t recv_length = recvfrom(sockfd, data_buffer, MAX_BUFFER_SIZE, 0, server_addr, &server_addr_len);

        // If error
        if (recv_length == -1) {
            perror("Error receiving DAT packet");
            fclose(file);
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Check if the expected DAT packet is received
        uint16_t received_block;
        memcpy(&received_block, data_buffer + 2, 2);

        if (received_block == htons(expected_block)) {
            // Write the data into the file
            fwrite(data_buffer + 4, 1, recv_length - 4, file);

            // Send the ACK packet
            struct TFTPAckPacket ack_packet;
            ack_packet.opcode = htons(TFTP_OPCODE_ACK);
            ack_packet.block_number = htons(expected_block);

            if (sendto(sockfd, &ack_packet, sizeof(ack_packet), 0, server_addr, server_addr_len) == -1) {
                perror("Error sending ACK");
                fclose(file);
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            // Increment the expected block number
            expected_block++;

            // If the packet is less than 512 bytes, it is the last packet
            if (recv_length < MAX_BUFFER_SIZE) {
                break;
            }
        }
    }

    // Close the file
    fclose(file);
}

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

    // Create a socket and verify its creation
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket creation error");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    // Send RRQ to the server
    sendRRQ(sockfd, res->ai_addr, filename);

    // Receive the file (DAT packets and corresponding ACKs)
    receiveFile(sockfd, res->ai_addr, res->ai_addrlen, filename);

    // Free the linked list when done with it
    freeaddrinfo(res);

    // Close the socket when done
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "ERROR : Usage: %s <serverhost> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *serverhost = argv[1];
    char *filename = argv[2];

    gettftp(serverhost, filename);

    return EXIT_SUCCESS;
}