#include <stdio.h>
#include <WinSock2.h>
#include <stdint.h>
#pragma comment(lib,"Ws2_32.lib")

int main() {
#ifdef WIN32

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {

        printf("Error initializing the Windows Sockets LIbrary");

        return -1;
    }

#endif
    uint16_t n, sum;
    printf("n = ");
    scanf("%hu", &n);
    uint16_t *arr = malloc(sizeof(uint16_t) * n);
    for(int i = 0; i < n; ++i) {
        printf("arr[%d] = ",i);
        scanf("%hu", arr + i);
    }
    printf("array is:");
    for(int i = 0; i < n; ++i) {
        printf("%hu ", arr[i]);
    }

    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("Socket error");
        return 1;
    }
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("192.168.1.141");
    int c = connect(sock, (struct sockaddr*) &server, sizeof(server));
    if(c < 0) {
        perror("Connect error");
        return 1;
    }
    n = htons(n);
    send(sock, &n, sizeof(n), 0);
    n = ntohs(n);

    for(uint16_t i = 0; i < n; ++i) {
        arr[i] = htons(arr[i]);
        send(sock, arr + i, sizeof(uint16_t), 0);
    }

    recv(sock, &sum, sizeof(sum), 0);
    sum = ntohs(sum);
    printf("sum = %hu", sum);
    closesocket(sock);

    return 0;
}