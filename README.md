2G1TP6 - DU Jack, JIN Clémentine

# TP2 : Client TFTP

 ## Question 1 :

 The **main** function checks if the number of command-line arguments is equal to 3. 
 If not, it print an **error message** indicating the correct usage. 
 It extracts the **server host** and **filename** from the command-line arguments. 
 It calls the **puttftp** and **gettftp** functions. 
 
 
 ## Question 2: Call getaddrinfo to obtain the server's address
 
 The **main** function sets up a structure **struct addrinfo hints,*res;** and use **gataddrinfo** to get server's address and then extracts it. 
 
 To test the extraction, the program printed the IP address (commented in this version) with **"www.google.com 123"** argument as **www.google.com** is the serverhost and **123** as a random filename. The the program return : ***Server IP: 142.250.179.78*** 
 

 ## Question 3 : Reserve a connection socket to the server
 
 The program creates a socket. 
 If there is an error in the creation of the socket, then it print an error message **"Socket creation error"** and exits. 
 It verify also if their is a connection error with the server. If the connexion fails, it prints an error message as well. 
 
 
 ## Question 4 : For gettftp

  ### Question 4a : Build properly formed Read Request (RRQ) and send it to the server 

  **Contruction the RRQ Packet** 
  The opcode field of **rrqPacket** is set to the RRQ opcode, and **htons** is used to convert it to network byte order (big-endian).


  **Send the RRQ packet to the server** 
  It send the RRQ packet to the TFTP server using a UDP socket. 
  The **sendto** function is used to send the RRQ packet to the TFTP server. 
  ***sendto(sockfd, &rrqPacket, sizeof(rrqPacket), 0, server_addr, sizeof(struct sockaddr));*** 
  > sockfd : The socket file descriptor
  > &rrqPacket : Pointer to the RRQ packet
  > sizeof(rrqPacket) : Size of the RRQ packet in bytes 
  > 0 : flags 
  > server_addr: Pointer to the server's address structure
  > sizeof(struct sockaddr) : Size of the server's address structure

  A Read Request has been captured in **Q4A_RRQSend.png** in the file **Question4**.

  ### Question 4b : Receive a file consisting of a single Data (DAT) packet and its acknowledgment (ACK)

  We use the sendRQQ packet to send a Read Request packet to the server, and the receiveDAT function receives a DATA packet from the server, sends an acknowledgment (ACK) packet to the server and write the data in a file. 


  **sendRRQ function :**
  The first two bytes of ***rrq_buffer*** are set to ***0x00*** and ***0x01***, representing the RRQ opcode. 
  The filename is copied into the buffer starting at position 2 and is null-terminated.
  The Mode is copied after the filename end and is null-terminated. 
  Last the RRQ packet is sent to the server. 

  **receiveDAT function :**
  The ***recvfrom*** function is used to receive the DATA packet from the server. If there is an error, an error message is printed. 
  ***recvfrom(sockfd, data_buffer, MAX_BUFFER_SIZE, 0, server_addr, &server_addr_len)***
  > sockfd : The socket file descriptor
  > data_buffer : Pointer to the buffer where the received data will be stored 
  > MAX_BUFFER_SIZE : maximum number of bytes that can be received 
  > 0 : flags 
  > server_addr : Pointer to a variable where the address of the server will be stored 
  > &server_addr_len : pointer to the siez of the server address 

  The ACK packet is constructed and the first two bytes are **0x00** and **0x04**, representing the ACK opcode. Then the data buffer is copied into the block before send to the server. If the sending occurs an error, it print an error message. 

  The filename is open in opened in binary write mode and the data from the packet is written in the file (from the 5th byte because there is opcode and block numbre in the first 4 byte). 

  ***Problem encountered***
  It is possible to send small files. Since packet length is limited, the packets are  split into several shorter ones. 

  The data packet and ACK have been captured in **Q4B_motd.png** in the file **Question4**.

  ### Question 4c : Receive a file consisting of multiple Data (DAT) packets and their respective acknowledgments (ACK)

  The receiveFile function receives DAT packets, sends corresponding ACK packets to the server and wirtes the received data into a file until the last packet is received. 
  
  The function has been tested with **ensea.png** and captured with wireshark **cf. Q4C_ensea.png** in the file **Question4**