#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

const char* SERVER_IP = "127.0.0.1";
const int PORT = 24175;

int main() {
    // Create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    // Set up server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        cerr << "Invalid address/Address not supported" << endl;
        close(clientSocket);
        return 1;
    }

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Connection failed" << endl;
        close(clientSocket);
        return 1;
    }

    cout << "Connected to server" << endl;

    // Receive player assignment (1 or 2)
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving player assignment" << endl;
        close(clientSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    int playerNumber = stoi(string(buffer));
    cout << "You are Player " << playerNumber << endl;

    // Send player ID to server
    string playerID;
    cout << "Enter your ID: ";
    cin >> playerID;
    send(clientSocket, playerID.c_str(), playerID.length(), 0);

    // TODO: Implement game interaction (send/receive game data)
    
    close(clientSocket);
    return 0;
}

