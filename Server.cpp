#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv)
{
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Initialize socket

  int socketServer = socket(AF_INET, SOCK_STREAM, 0);

  // // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // // ensures that we don't run into 'Address already in use' errors

  int reuse = 1;
  if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0)
  {
    std::cout << "error setting socket options";
    return 1;
  }
  // server address setup

  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(6379);
  // bind the socket to the port

  if (bind(socketServer, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
  {
    std::cout << "error binding server";
    return 1;
  }

  // listen for connections
  int connectionBacklog = 5;
  if (listen(socketServer, connectionBacklog) != 0)
  {
    std::cout << "error in listening";
    return 1;
  }

  // You can use print statements as follows for debugging, they'll be visible when running tests.
  std::cout << "Logs from your program will appear here!\n";

  // accepting multiple connection with while loop
  char recv_buff[65536];
  memset(recv_buff, '\0', sizeof(recv_buff));
  // outer loop
  while (1)
  {
    struct sockaddr_in clientAddress;
    int socket_len = sizeof(clientAddress);
    // first accept the connection
    int conn = accept(socketServer, (struct sockaddr *)&clientAddress, (socklen_t *)&socket_len);
    if (conn < 0)
    {
      std::cerr << "connection error";
      return 1;
    }

    std::cout << "New client accepted" << std::endl;
    // inner loop for receiving data
    ssize_t bytes_received;
    while ((bytes_received = recv(conn, recv_buff, sizeof(recv_buff), 0)) > 0)
    {
      std::cout << "Received bytes is" << bytes_received << std::endl;
      std::cout << "Received data is" << recv_buff << std::endl;

      std::string Response = "+PONG\r\n";
      send(conn, Response.c_str(), Response.length(), 0);
      memset(recv_buff, '\0', sizeof(recv_buff));
    }

    if (bytes_received == 0)
    {
      std::cout << "Client disconnected gracefully" << std::endl;
    }
    else if (bytes_received == -1)
    {
      std::cerr << "Data receiving error. Client may have disconnected." << std::endl;
    }

    // Close the connection after the client disconnects or on error
    close(conn);
  }

  close(socketServer);

  return 0;
}

