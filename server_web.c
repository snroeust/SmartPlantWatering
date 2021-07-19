/* Simple HTTP server */

/* For UNIX/Linux */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define PORT 8080
#define BUFLEN 2048

#define MAX_REQUEST_SIZE 2047   //for most browsers the maximum size for a http request




/* Function to handle an error by printing a message and exiting */
static void print_error_and_exit(char *errorMessage)
{
    fprintf(stderr, "%s: %s\n", errorMessage, strerror(errno));
    exit(1);
}

const char *getContentTypeFromPath(const char* path) {
    //strchr returns a pointer to the last occurrence of character in str. If the character is not found, the function returns a null pointer.
    const char *last_dot = strrchr(path, '.');
    if (last_dot) {
        if (strcmp(last_dot, ".htm") == 0) return "text/html";
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".txt") == 0) return "text/plain";
    }

    return "text/html";
}

//this struct contains all connected clients
struct clientInformation {
    socklen_t address_length;
    struct sockaddr_storage address;
    int socket;
    char request[MAX_REQUEST_SIZE + 1];
    int received;
    struct clientInformation *next;
};

//statically inititialize the client information
static struct clientInformation *clients = 0;

//this function returns the client with the corresponding socket or allocates a new one if the socket is not found and adds it to beginning of the clientInformation list
struct clientInformation *get_client(int s) {
    struct clientInformation *clientInformation = clients;

    while(clientInformation) {
        if (clientInformation->socket == s)
            break;
        clientInformation = clientInformation->next;
    }

    if (clientInformation) return clientInformation;
    struct clientInformation *n =
        (struct clientInformation*) calloc(1, sizeof(struct clientInformation));

    if (!n) {
        print_error_and_exit("Out of memory.\n");
    }

    n->address_length = sizeof(n->address);
    n->next = clients;
    clients = n;
    return n;
}



const char *getAddressFromClient(struct clientInformation *ci) {
    static char address_buffer[100];
    //gets the address from the current client and translates that into ASCII represantition
    getnameinfo((struct sockaddr*)&ci->address,
            ci->address_length,
            address_buffer, sizeof(address_buffer), 0, 0,
            NI_NUMERICHOST);
    return address_buffer;
}

void deleteClient(struct clientInformation *client) {
    close(client->socket);

    struct clientInformation **p = &clients;

    while(*p) {
        if (*p == client) {
            *p = client->next;
            free(client);
            return;
        }
        p = &(*p)->next;
    }

    print_error_and_exit("client to delete not found");
}

void sendBadRequest(struct clientInformation *client) {
    const char *r400 = "HTTP/1.1 400 Bad Request\r\n"
        "Connection: close\r\n"
        "Content-Length: 11\r\n\r\nBad Request";
    send(client->socket, r400, strlen(r400), 0);
    deleteClient(client);
}

void sendNotFound(struct clientInformation *client) {
    const char *r404 = "HTTP/1.1 404 Not Found\r\n"
        "Connection: close\r\n"
        "Content-Length: 9\r\n\r\nNot Found";
    send(client->socket, r404, strlen(r404), 0);
    deleteClient(client);
}

void sendHeaderAndFile(struct clientInformation *client, const char *path) {
    printf("sendHeaderAndFile %s %s\n", getAddressFromClient(client), path);

    if (strlen(path) > 100) {
        sendBadRequest(client);
        return;
    }

    if (strstr(path, "..")) {
        sendNotFound(client);
        return;
    }

    //redirect to index.html in case of "GET /"
    if (strcmp(path, "/") == 0)
        path = "index.html";
    //else chop the first slash(first character), so u get the actual path to the file to open
    else
        path++; 

    //open the file with read priviliges and we deal with binary data, thats why we use the b flag
    FILE *fp = fopen(path, "rb");

    if (!fp) {
        sendNotFound(client);
        return;
    }

    //get the file size using fstat determining the Content-Length
    struct stat fs;
    int fd = open(path, O_RDONLY);
    if (fstat(fd, &fs) == -1) {
        print_error_and_exit("couldnt tell Content-Length");
    } 
    size_t contentLength = fs.st_size;

    const char *ct = getContentTypeFromPath(path);

    char buffer[BUFLEN]; //2048 bytes

    sprintf(buffer, "HTTP/1.1 200 OK\r\n");
    send(client->socket, buffer, strlen(buffer), 0);
    sprintf(buffer, "Connection: keep-alive\r\n");
    send(client->socket, buffer, strlen(buffer), 0);
    sprintf(buffer, "Content-Length: %zu\r\n", contentLength);
    send(client->socket, buffer, strlen(buffer), 0);
    sprintf(buffer, "Content-Type: %s\r\n", ct);
    send(client->socket, buffer, strlen(buffer), 0);
    sprintf(buffer, "\r\n");
    send(client->socket, buffer, strlen(buffer), 0);

    int r = fread(buffer, 1, BUFLEN, fp);
    while (r) {
        send(client->socket, buffer, r, 0);
        r = fread(buffer, 1, BUFLEN, fp);
    }

    fclose(fp);
    //delete the old request
    client->received = 0;
    memset(&client->request[0], 0, sizeof(client->request));
}


int main(int argc, char *argv[])
{
    struct sockaddr_in srv_addr;

    int srv_fd;

    int backlog = 10;
    ssize_t rcount;
    time_t timestamp;

    //Creating a socket
    srv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv_fd == -1)
        print_error_and_exit("Error at socket()");

    //Define the socket address of the server
    memset(&srv_addr, 0, sizeof(srv_addr));
    //IPv4 Connection */
    srv_addr.sin_family = AF_INET;
    /* Port */
    srv_addr.sin_port = htons(PORT);
    //INADDR_ANY: accept any address
    srv_addr.sin_addr.s_addr = INADDR_ANY;

    //Binding socket to a port
    if (bind(srv_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1)
        print_error_and_exit("Error at bind()");

    //Listen for incoming connections
    if (listen(srv_fd, backlog) == -1)
        print_error_and_exit("Error at listen()");

    printf("Ready to receive connections!\n");
    //While infinite loop to accept connections
    while (1)
    {
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(srv_fd, &reads);
        int max_socket = srv_fd;

        struct clientInformation *ci = clients;

        //put all sockets from clients into a set and get the maximum socket by iterating through all connected clients
        while(ci) {
            FD_SET(ci->socket, &reads);
            if (ci->socket > max_socket)
                max_socket = ci->socket;
            ci = ci->next;
        }

        //select one of the fd which fulfills the conditions. Here we use the read parameter to check if there is any data to read on these sockets
        if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
            print_error_and_exit("select() failed.");
        }

        //check if there are still any sockets in this set, if so we can work on those sockets
        if (FD_ISSET(srv_fd, &reads)) {
            //allocate a new client
            struct clientInformation *client = get_client(-1);

            //awaits the connenction from the client
            client->socket = accept(srv_fd,
                    (struct sockaddr*) &(client->address),
                    &(client->address_length));

            if ((client->socket) < 0) {
                print_error_and_exit("accept() failed.");
            }


            printf("new Connenction from IP: %s\n", getAddressFromClient(client));
        }

        //the new connected client is the first client in the linked list
        struct clientInformation *client = clients;
        while(client) {
            struct clientInformation *next = client->next;

            //check if the file desciptor is set
            if (FD_ISSET(client->socket, &reads)) {
                
                //check if the buffer for the received request data is completly full
                if (MAX_REQUEST_SIZE == client->received) {
                    sendBadRequest(client);
                    client = next;
                    continue;
                }

                //receive the request and store it in client->request[2048]
                int r = recv(client->socket,
                        client->request + client->received,
                        MAX_REQUEST_SIZE - client->received, 0);

                if (r < 1) {
                    printf("Disconnect at receiving data from client with IP: %s.\n", getAddressFromClient(client));
                    deleteClient(client);

                } else {
                    client->received += r;
                    client->request[client->received] = 0;

                    //check if the request is already complete (the request ends with a \r\n\r\n)
                    char *q = strstr(client->request, "\r\n\r\n");
                    if (q) {
                        *q = 0;
                        //only handle GET request, send a 400 Bad Request to all other requests
                        if (strncmp("GET /", client->request, 5) == 0) {
                            char *path = client->request + 4;
                            char *end_path = strstr(path, " ");
                            if (!end_path) {
                                //path contains a blank symbol
                                sendBadRequest(client);
                            } else {
                                *end_path = 0;
                                //korrekte request, read the file and sent it over the socket with the the http header
                                sendHeaderAndFile(client, path);
                            }
                        } else {
                            sendBadRequest(client);
                        }
                    }
                }
            }
            //work on the next client, which has data to read from
            client = next;
        }

    }

    //Close the server socket
    close(srv_fd);

    return EXIT_SUCCESS;
}