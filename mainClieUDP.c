#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BUFFER_SIZE 128
#define FINALIZAR "#fim"

int sockfd;
struct sockaddr_in server_address;
WSADATA wsa_data;

void msg_err_exit(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    WSACleanup();
    exit(EXIT_FAILURE);
}

int main() {
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        msg_err_exit("WSAStartup() falhou");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        msg_err_exit("socket() falhou");
    }

    char server_ip[16];
    unsigned short server_port;
    printf("Endereço IP do servidor: ");
    scanf("%15s", server_ip);
    printf("Porta do servidor: ");
    scanf("%hu", &server_port);
    fflush(stdin);

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    char filename[BUFFER_SIZE];
    printf("Nome do arquivo para enviar: ");
    scanf("%s", filename);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        msg_err_exit("Erro ao abrir o arquivo");
    }

    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        if (sendto(sockfd, buffer, bytes_read, 0, 
                   (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
            fclose(file);
            msg_err_exit("sendto() falhou");
        }
        printf("Enviado %d bytes\n", bytes_read);
    }

    strcpy(buffer, FINALIZAR);
    sendto(sockfd, buffer, strlen(buffer) + 1, 0, 
           (struct sockaddr *)&server_address, sizeof(server_address));

    printf("Arquivo enviado com sucesso.\n");

    fclose(file);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
