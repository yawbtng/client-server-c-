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

    // TODO: Accept client connections and handle game logic
    
    close(serverSocket);
    return 0;
}

