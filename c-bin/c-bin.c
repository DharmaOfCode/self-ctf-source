#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <stdbool.h>
#include <unistd.h>
#define MAX 80 
#define PORT 1984 
#define SA struct sockaddr 

bool checkPassword(char pwd[]);
bool checkName(char name[]);
void jackpot();

//Auth function
void authenticate(int sockfd)
{
    char buff[MAX]; 
    char r1[] = "Intruder! intruder!\n";
    char r2[] = "Nice try You have pieced a lot of this together, though.";

    char q1[] = "What is your name: ";
    write(sockfd, q1, sizeof(q1));
    read(sockfd, buff, sizeof(buff));

    //Strip newline
    buff[strcspn(buff, "\n")] = 0;

    bool success;
    success = checkName(buff);
    if (!success){
        printf("[-] Invalid user. Closing connection.\n");
        write(sockfd, r1, sizeof(r1));
        return;
    }
    
    bzero(buff, MAX);
    bzero(q1, sizeof(q1));

    char q2[] = "Enter the password: ";
    write(sockfd, q2, sizeof(q2));
    read(sockfd, buff, sizeof(buff));
    
    //Strip newline
    buff[strcspn(buff, "\n")] = 0;

    success = checkPassword(buff);
    if (!success){
        printf("[-] Password incorrect. Closing connection.\n");
        write(sockfd, r2, sizeof(r2));
        return;
    }

    bzero(buff, MAX);
    bzero(q2, sizeof(q2));

    jackpot(sockfd);
}

void jackpot(int sockfd){
    FILE *fptr = fopen("../secrets/metaverse.txt", "r"); 
    if (fptr == NULL) 
    { 
        printf("Unexpected error \n"); 
        exit(0); 
    } 

    char buffer[100]; // Buffer to store data
    FILE * stream;
    stream = fopen("../secrets/metaverse.txt", "r");
    if (stream == NULL) 
    { 
        printf("Unexpected error \n"); 
        exit(0); 
    } 

    int count = fread(&buffer, sizeof(char), 98, stream);
    fclose(stream);

    char m[] = "\nWelcome to the Metaverse. Here are your secrets\n\n"; 
    write(sockfd, m, sizeof(m));
    write(sockfd, buffer, sizeof(buffer));
}

bool checkPassword(char pwd[])
{
    printf("[+] Checking password\n");
    if ((unsigned)strlen(pwd) == 0){
        return false;
    }
    for(int i = 0; i < (unsigned)strlen(pwd); i++){
        char c = pwd[i];

        switch (i)
        {
            case 0:
                if ((int)c != 104){
                    return false;
                }
            break;
            case 1:
                if ((int)c != 52){
                    return false;
                }
            break;
            case 2:
                if ((int)c != 99){
                    return false;
                }
            break;
            case 3:
                if ((int)c != 107){
                    return false;
                }
            break;
            case 4:
                if ((int)c != 49){
                    return false;
                }
            break;
            case 5:
                if ((int)c != 57){
                    return false;
                }
            break;
            case 6:
                if ((int)c != 56){
                    return false;
                }
            break;
            case 7:
                if ((int)c != 52){
                    return false;
                }
            break;   
            default:
                return false;
        }
    }

    return true;
}

bool checkName(char name[])
{
    printf("[+] Checking name\n");
    if ((unsigned)strlen(name) != 12){
        return false;
    }

    char f = name[0];
    if (f != 'H'){
        return false;
    }

    int len = strlen(name);
    char last = name[len-11];
    if (!strcmp("Protagonist", &last)){
        return false;
    }
    return true;
}

void handleConnection(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 

    //Write welcome message
    char greeting[] = "Welcome, human\n";
    printf("[+] Connection received\n");
    write(sockfd, greeting, sizeof(greeting)); 

    authenticate(sockfd);
    return;
}
  
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("[-] Socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("[+] Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("[-] socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("[+] Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("[-] Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("[+] Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("[-] Server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("[+] Server acccept the client...\n"); 
  
    handleConnection(connfd); 
  
    close(sockfd); 
} 
