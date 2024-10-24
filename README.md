## 2G1TP6 - DU Jack, JIN Clémentine

# Synthesis TP2 - Client TFTP

## Objectives:

The purpose of this lab is to develop a TFTP (Trivial File Transfer Protocol) client that can transfer files between a client and server using the RFC specifications for TFTP. <br>
**TFTP** is widely used in network environments for tasks like operating system installations, firmware updates, and diskless network node operations. <br>
Through this lab, students will gain hands-on experience in implementing networking protocols by creating a TFTP client capable of file transfers, using tools like Wireshark for packet analysis.

The objectives include:
- Building both a file download and upload client for TFTP.
- Implementing features like blocksize optimization, error handling, and packet loss recovery.
- Testing the client with various file types and sizes, while following RFC standards


***
## Getting started:

**Compile with gcc:**
```
gcc -Wall -o QuestionX QuestionX.c
```

**Run:**<br>
Once the compilation is successful, you can run the compiled program using:
```
./QuestionX
```

## Lab answers:

### **Question 1:**

The **main** function checks if the number of command-line arguments is equal to 3. 
If not, it print an **error message** indicating the correct usage. 
It extracts the **server host** and **filename** from the command-line arguments. 
It calls the **puttftp** and **gettftp** functions. 

***
### **Question 2:** Call getaddrinfo to obtain the server's address.

The **main** function sets up a structure **struct addrinfo hints,*res;** and use **gataddrinfo** to get server's address and then extracts it. 

To test the extraction, the program printed the IP address (commented in this version) with **"www.google.com 123"** argument as **www.google.com** is the serverhost and **123** as a random filename. <br>
The program returns: <br>
***Server IP: 142.250.179.78*** 

***
### **Question 3:** Reserve a connection socket to the server

The program creates a socket.<br>
If there is an error in the creation of the socket, then it print an error message **"Socket creation error"** and exits.<br>
It verify also if their is a connection error with the server. If the connexion fails, it prints an error message as well. 

***
### **Question 4:** For gettftp
### **Question 4a:** Build properly formed Read Request (RRQ) and send it to the server 

**Contruction the RRQ Packet** <br>
The opcode field of **rrqPacket** is set to the RRQ opcode, and **htons** is used to convert it to network byte order (big-endian).

**Send the RRQ packet to the server** <br>
It send the RRQ packet to the TFTP server using a UDP socket. <br> 
The **sendto** function is used to send the RRQ packet to the TFTP server. <br>
***sendto(sockfd, &rrqPacket, sizeof(rrqPacket), 0, server_addr, sizeof(struct sockaddr));***

```
sockfd : The socket file descriptor

&rrqPacket : Pointer to the RRQ packet

sizeof(rrqPacket) : Size of the RRQ packet in bytes 

0 : flags 

server_addr: Pointer to the server's address structure

sizeof(struct sockaddr) : Size of the server's address structure
```

A Read Request has been captured in **Q4A_RRQSend.png** in the file **Question4**.

***
### **Question 4b:** Receive a file consisting of a single Data (DAT) packet and its acknowledgment (ACK)

We use the sendRQQ packet to send a Read Request packet to the server, and the receiveDAT function receives a DATA packet from the server, sends an acknowledgment (ACK) packet to the server and write the data in a file. 

**sendRRQ function :** <br>
The first two bytes of ***rrq_buffer*** are set to ***0x00*** and ***0x01***, representing the RRQ opcode.<br> 
The filename is copied into the buffer starting at position 2 and is null-terminated.<br>
The Mode is copied after the filename end and is null-terminated. <br> 
Last the RRQ packet is sent to the server. 

**receiveDAT function :** <br>
The ***recvfrom*** function is used to receive the DATA packet from the server. If there is an error, an error message is printed.<br> 
***recvfrom(sockfd, data_buffer, MAX_BUFFER_SIZE, 0, server_addr, &server_addr_len)***

```
sockfd : The socket file descriptor

data_buffer : Pointer to the buffer where the received data will be stored 

MAX_BUFFER_SIZE : maximum number of bytes that can be received 

0 : flags 

server_addr : Pointer to a variable where the address of the server will be stored 

&server_addr_len : pointer to the size of the server address 

```

The ACK packet is constructed and the first two bytes are **0x00** and **0x04**, representing the ACK opcode. Then the data buffer is copied into the block before send to the server. If the sending occurs an error, it print an error message.

The filename is open in opened in binary write mode and the data from the packet is written in the file (from the 5th byte because there is opcode and block numbre in the first 4 byte). 

***Problem encountered*** <br>
It is possible to send small files. Since packet length is limited, the packets are  split into several shorter ones. 

The data packet and ACK have been captured in **Q4B_motd.png** in the file **Question4**.

***
### **Question 4c:** Receive a file consisting of multiple Data (DAT) packets and their respective acknowledgments (ACK)

The receiveFile function receives DAT packets, sends corresponding ACK packets to the server and wirtes the received data into a file until the last packet is received.<br>
The function has been tested with **ensea.png** and captured with wireshark **cf. Q4C_ensea.png** in the file **Question4**


***
## **Question 5:** For puttftp

### **Question 5a:** Build a properly formed Write Request (WRQ) and send it to the server

The sendWRQ function is very similar to the sendRRQ function, except the opcode.<br> 
The Write Request is send with the **sendto** function.<br> 
A Write Request has been captured in **Q5A_WRQ.png** in the file **Question5**.

***
### **Question 5b:** Send a file consisting of a single Data (DAT) packet and receive its acknowledgment (ACK)

The sendSingleDAT function send a single DAT packet to the server and verify if the received ACK packet is correct. 

**DATA packet**<br>
The DATA packet is prepared with the fist 2 byto being set to **0x00** and **0x03** representing the DATA opcode.<br> 
Then the 2 byte for block is initialised to 1 for simplicity and is send. 

**ACK packet**<br>
The function **recvfrom** is used here as before to receive an ACK packet, and check if the ACK packet matches with the expected block number. Ohterwise, an error message is print. 

***
### **Question 5c:** Send a file consisting of multiple Data (DAT) packets and receive their respective acknowledgments (ACK).

The **sendFile** function prepare the DAT packet, set the block number for each packet, send the DATA, receive and check the ACK packet and increment the block number until the last block of DATA.

<br>
***
*Last update: 2024/10/24*
***