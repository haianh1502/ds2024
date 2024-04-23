1. Demo:
- Server:
    ┌──(kali㉿kali)-[~/Desktop/checkcode]
    └─$ ./server                  
    Socket successfully created..
    Socket successfully binded!
    Listening...
    Data written in the text file                                             
- Client:
    ┌──(kali㉿kali)-[~/Desktop/checkcode]
    └─$ ./client                  
    Socket successfully created..
    Connected to server.
    File send successfully. 
    Close. 

2. Build
- Server: create a socket -> initialize the address structure of the socket and then bind the address -> listen to the connection -> accept the connection -> receive file -> write file.
- Client: create a socket -> initialize the address structure of the socket according to the server -> connect the newly created socket to the server's address -> send file -> close.