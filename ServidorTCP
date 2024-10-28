#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BUFFER_SIZE 128

WSADATA wsa_data;
SOCKET server_socket, client_socket;
struct sockaddr_in server_address, client_address;

void msg_err_exit(char *msg) {
    fprintf(stderr, "%s", msg);
    system("PAUSE");
    exit(EXIT_FAILURE);
}

int main() {
    // Inicia o Winsock 2.0
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        msg_err_exit("WSAStartup() falhou\n");

    // Cria o socket TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        WSACleanup();
        msg_err_exit("socket() falhou\n");
    }

    // Configura o endereço e a porta do servidor
    unsigned short server_port;
    printf("Porta do servidor: ");
    scanf("%hu", &server_port);
    fflush(stdin);

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Associa o socket ao endereço e porta
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        closesocket(server_socket);
        WSACleanup();
        msg_err_exit("bind() falhou\n");
    }

    // Escuta por conexões
    if (listen(server_socket, 1) == SOCKET_ERROR) {
        closesocket(server_socket);
        WSACleanup();
        msg_err_exit("listen() falhou\n");
    }

    printf("Aguardando conexão...\n");

    int client_size = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_size);
    if (client_socket == INVALID_SOCKET) {
        closesocket(server_socket);
        WSACleanup();
        msg_err_exit("accept() falhou\n");
    }

    printf("Conexão estabelecida com %s\n", inet_ntoa(client_address.sin_addr));

    // Abre o arquivo para escrita
    FILE *file = fopen("arquivo_recebido.txt", "wb");
    if (file == NULL) msg_err_exit("Erro ao criar o arquivo\n");

    // Recebe o arquivo em blocos
    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), bytes_received, file);
    }

    if (bytes_received == SOCKET_ERROR)
        msg_err_exit("recv() falhou\n");

    printf("Arquivo recebido com sucesso.\n");

    fclose(file);
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    system("PAUSE");
    return 0;
}
