2G1TP6 - DU Jack, JIN Clémentine

## TP2 : Client TFTP

**Question 1 :** 

The **main** function checks if the number of command-line arguments is equal to 3. 
 If not, it prrint an **error message** indicating the correct usage. 
 It extracts the **server host** and **filename** from the command-line arguments. 
 It calls the **puttftp** and **gettftp** functions. 
 
 
 **Question 2:** 
 
 The **main** function sets up a structure **struct addrinfo hints,*res;** and use **gataddrinfo** to get server's address and then extracts it. 
 
 To test the extraction, the program printed the IP address (commented in this version) with *'www.google.com 123" argument as *www.google.com* is the serverhost and *123* as a random filename. The the program return : ***Server IP: 142.250.179.78*** 
 

 
