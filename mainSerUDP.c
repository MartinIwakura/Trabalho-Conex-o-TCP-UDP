#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BUFFER_SIZE 128
#define FINALIZAR "#fim"

int local_socket;
struct sockaddr_in local_address, remote_address;
WSADATA wsa_data;

void msg_err_exit(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    WSACleanup();
    exit(EXIT_FAILURE);
}

int main() {
    // Inicia o Winsock 2.0
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        msg_err_exit("WSAStartup() falhou");

    // Cria o socket UDP
    local_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (local_socket == INVALID_SOCKET) {
        msg_err_exit("socket() falhou");
    }

    unsigned short local_port;
    printf("Porta local: ");
    scanf("%hu", &local_port);
    fflush(stdin);

    memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(local_port);
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(local_socket, (struct sockaddr *)&local_address, sizeof(local_address)) == SOCKET_ERROR) {
        closesocket(local_socket);
        msg_err_exit("bind() falhou");
    }

    printf("Servidor aguardando pacotes...\n");

    char buffer[BUFFER_SIZE];
    FILE *file = fopen("arquivo_recebido.txt", "wb");
    if (file == NULL) {
        msg_err_exit("Erro ao criar o arquivo");
    }

    int remote_length = sizeof(remote_address);
    int bytes_received;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recvfrom(local_socket, buffer, BUFFER_SIZE, 0, 
                                  (struct sockaddr *)&remote_address, &remote_length);
        if (bytes_received == SOCKET_ERROR) {
            msg_err_exit("recvfrom() falhou");
        }

        printf("Recebido %d bytes: %s\n", bytes_received, buffer);

        if (strncmp(buffer, FINALIZAR, strlen(FINALIZAR)) == 0) {
            printf("Recebimento finalizado.\n");
            break;
        }

        fwrite(buffer, sizeof(char), bytes_received, file);
    }

    fclose(file);
    closesocket(local_socket);
    WSACleanup();
    printf("Arquivo salvo como 'arquivo_recebido.txt'.\n");
    return 0;
}
