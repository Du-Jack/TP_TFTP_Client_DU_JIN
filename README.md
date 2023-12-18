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

