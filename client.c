#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[BUFFER_SIZE] = {0};
  const char *server_ip = (argc > 1) ? argv[1] : "127.0.0.1";

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("\nSocket creation error");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address or Address not supported: %s\n", server_ip);
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed. Is the server running at %s:%d?\n", server_ip,
           PORT);
    return -1;
  }

  printf("Connected to server at %s:%d\n", server_ip, PORT);
  printf("Type your messages below. Type 'exit' to quit.\n\n");

  while (1) {
    printf("> ");

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      printf("\nError reading input.\n");
      break;
    }

    send(sock, buffer, strlen(buffer), 0);

    if (strncmp(buffer, "exit", 4) == 0) {
      printf("Closing connection...\n");
      break;
    }
  }

  close(sock);
  return 0;
}
