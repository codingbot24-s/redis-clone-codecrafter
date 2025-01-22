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
  

  close(socketServer);

  return 0;
}

