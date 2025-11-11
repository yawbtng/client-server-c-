#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include "utils.h"

using namespace std;

const int PORT = 24175;
const int MAX_CLIENTS = 2;

int main() {
    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    // Set socket options to allow reuse of address
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "Error setting socket options" << endl;
        close(serverSocket);
        return 1;
    }

    // Bind socket to port 24175
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket to port " << PORT << endl;
        close(serverSocket);
        return 1;
    }

    // Listen for connections
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        cerr << "Error listening on socket" << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server listening on port " << PORT << "..." << endl;

    // Accept first client connection (Player 1)
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int client1Socket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (client1Socket < 0) {
        cerr << "Error accepting first client connection" << endl;
        close(serverSocket);
        return 1;
    }
    cout << "First client connected (Player 1)" << endl;

    // Inform first client that they are Player 1
    string player1Msg = "1";
    send(client1Socket, player1Msg.c_str(), player1Msg.length(), 0);

    // Accept second client connection (Player 2)
    int client2Socket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (client2Socket < 0) {
        cerr << "Error accepting second client connection" << endl;
        close(client1Socket);
        close(serverSocket);
        return 1;
    }
    cout << "Second client connected (Player 2)" << endl;

    // Inform second client that they are Player 2
    string player2Msg = "2";
    send(client2Socket, player2Msg.c_str(), player2Msg.length(), 0);

    // Receive player IDs from both clients
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    // Receive Player 1 ID
    int bytesReceived = recv(client1Socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving Player 1 ID" << endl;
        close(client1Socket);
        close(client2Socket);
        close(serverSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    string player1ID(buffer);

    // Receive Player 2 ID
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(client2Socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving Player 2 ID" << endl;
        close(client1Socket);
        close(client2Socket);
        close(serverSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    string player2ID(buffer);

    // Print game start message
    cout << player1ID << " vs. " << player2ID << ": Game start" << endl;

    // TODO: Implement game logic (rounds, scoring, etc.)
    
    // Close connections
    close(client1Socket);
    close(client2Socket);
    close(serverSocket);
    return 0;
}

