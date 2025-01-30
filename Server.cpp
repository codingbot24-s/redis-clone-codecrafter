#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <vector>
#include <cctype>
#include <algorithm>


// Define a function to interact with client with multiThreading


class RESPParser {
  public:
  void log(std::string message) {
    std::cout << message << std::endl;
  }

    char Integer = ':';
    char String = '+';
    char Bulk  = '$';
    char Array = '*';
    char Error = '-';
    
    struct RESP {
      std::vector<std::string> Raw;
      std::vector<std::string> Data;
      int                      Count;
      char                     Type;  
    };

  std::vector<std::string> RawData (RESP r) {
    return r.Data;
  };  

  std::string Convert_String (RESP r) {
    return std::string(r.Data[0]);
  };

  int Convert_Integer (RESP r) {
    int i = std::stoi(r.Data[0]);
    return i;
  };

  float Convert_Float (RESP r) {
    float f = std::stof(r.Data[0]);
    return f;
  };
  
  // public:  
  //   static std::vector<std::string> ParseCommand() {
  
    
  // }

  
  
};

// TODO Move from multithreading to event loop
void interactWithClient(int clientSocket)
{
  while (1)
  {
    char recv_buff[65536];
    memset(recv_buff, '\0', sizeof(recv_buff));
    ssize_t bytes_received = 0;
    if (bytes_received = recv(clientSocket, recv_buff, sizeof(recv_buff), 0) > 0)
    {
      // std::cout << "Received bytes is " << bytes_received << std::endl;
      // std::cout << "Received data is " << recv_buff << std::endl;
      
      
      // Code for sending Pong to the client
      // std::string Response = "+PONG\r\n";
      // send(clientSocket, Response.c_str(), Response.length(), 0);
      // memset(recv_buff, '\0', sizeof(recv_buff));
    }
    if (bytes_received == 0)
    {
      std::cout << "client disconnected" << std::endl;
      break;
    }
    else if (bytes_received == -1)
    {
      std::cerr << "data receiving error" << std::endl;
      break;
    }
  }
  close(clientSocket);
}
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
  serverAddress.sin_port = htons(6378);
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

  // loop for running the program
  while (1)
  {

    // accepting connection
    struct sockaddr_in clientAddress;
    int socket_len = sizeof(clientAddress);
    // first accept the connection
    int clientSocket = accept(socketServer, (struct sockaddr *)&clientAddress, (socklen_t *)&socket_len);
    if (clientSocket < 0)
    {
      std::cerr << "connection error";
      return 1;
    }

    std::cout << "New client accepted going to spawn a thread" << std::endl;
    // Thread spawning for multiple connection
    std::thread t1(interactWithClient, clientSocket);
    t1.detach();
  }

  close(socketServer);

  return 0;
}
