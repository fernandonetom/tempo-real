/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Comunicação entre processos usando Named Pipes(FIFOs)
 * Processo Leitor
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "create_socket.h"

#define TAM_MSG 1024

int main(int argc, char *argv[]) {
    int clients[100];
    int nClient = 0;


    // Require an address to be supplied from the command line
    if (argc != 2) {
        printf("Please supply and address to connect to.\n");
        return 1;
    }
    
    // Creates a server socket and terminate on error
    int server = create_socket(1, argv[1]);
    if (server == -1) return 1;


    // Listener loop
    while (1) {

        fd_set RD, WR, ER;
        FD_ZERO(&RD);
        FD_ZERO(&WR);
        FD_ZERO(&ER);

        FD_SET(server, &RD);
        FD_SET(server, &ER);

        int maxFD = server;

        for(int i = 0;  i < nClient; i++){
            if(i > maxFD){
                maxFD = i;
            }

            FD_SET(clients[i], &RD);
            FD_SET(clients[i], &ER);
        }

        printf("Ready for clients...\n");
        
        // Wait for connections on the socket server.
        // Each client gets a new private socket.
        if(select(maxFD + 1, &RD, 0, &ER, 0) < 0){
            printf("Error...\n");
            break;
        }

        if(FD_ISSET(server, &RD)){
            nClient+=1;
            int client = accept(server, 0, 0);
						printf("Aqui...\n");
            clients[nClient] = client;

            for(int i = 0;  i < nClient; i++){
                if(FD_ISSET(clients[i], &RD)){
									// Reads a message.
									char  msg[TAM_MSG];
									ssize_t sz = read(client, msg, TAM_MSG);
									
									// read returns -1 if read has failed.
									if (sz == -1) break;
									
									// If read size is zero then something went wrong too.
									if (!sz) break;
									
									// Add the null terminator to make sure it is a valid C string
									msg[sz] = 0;
									
									// Print back the received message.
									printf("Server Got: \"%s\"\n", msg);
									
									// Change the message to uppercase
									for (char *p=msg; *p; p++) *p = toupper(*p);
                    
                  for(int j = 0;  j < nClient; j++){
										if(j != i) {
											// Send to another clients
            					write(clients[j], msg, sz);
										}
									}
                }
            }
        }


        // printf("Got a client!\n");
        
        // Terminate the client connection.
        // printf("Client is disconnected.\n");
        // close(client);
    }

    // Close and remove the server socket
    close(server);
    unlink(argv[1]);

    // All went well, let the OS know it.
    return 0;
}
