#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BUFFER_SIZE 128
#define FINALIZAR "#fim"

int sockfd;
struct sockaddr_in server_address;
WSADATA wsa_data;

/* Exibe uma mensagem de erro e termina o programa */
void msg_err_exit(char *msg) {
    fprintf(stderr, "%s\n", msg); // Adiciona uma nova linha para melhor formatação
    system("PAUSE");
    exit(EXIT_FAILURE);
}

int main() {
    // Inicia o Winsock 2.0 (DLL)
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        msg_err_exit("WSAStartup() falhou\n");

    // Cria o socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        WSACleanup();
        msg_err_exit("socket() falhou\n");
    }

    // Preenche a estrutura do endereço do servidor
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

    // Abre o arquivo para leitura
    char filename[BUFFER_SIZE];
    printf("Nome do arquivo para enviar: ");
    scanf("%s", filename);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) msg_err_exit("Erro ao abrir o arquivo\n");

    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Lê e envia o arquivo em blocos
    while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        if (sendto(sockfd, buffer, bytes_read, 0, 
                   (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
            msg_err_exit("sendto() falhou\n");
        }
        printf("Enviado %d bytes\n", bytes_read); // Log de envio
    }

    // Envia a mensagem de finalização
    strcpy(buffer, FINALIZAR);
    sendto(sockfd, buffer, strlen(buffer), 0, 
           (struct sockaddr *)&server_address, sizeof(server_address));

    printf("Arquivo enviado com sucesso.\n");

    fclose(file);
    closesocket(sockfd);
    WSACleanup();
    system("PAUSE");
    return 0;
}
