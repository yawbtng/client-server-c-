#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <stdexcept>

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

    // • The client will send in a request to the server requesting a game (for this program, you always assume the client and server is on the same machine (so you should use "127.0.0.1" as the IP address)
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

    // • The client should then send an ID (for this program, a single string) to the server for the record.
    // Send player ID to server
    string playerID;
    cout << "Enter your ID: ";
    cin >> playerID;
    send(clientSocket, playerID.c_str(), playerID.length(), 0);

    // ROUND 1
    // Receive request for number
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving round 1 request" << endl;
        close(clientSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';

    // Get number input from user and send to server
    int round1Number;
    if (playerNumber == 1) {
        cout << "Round 1: Enter a number (50-99, non-prime): ";
    } else {
        cout << "Round 1: Enter a number (60-99, non-prime): ";
    }
    cin >> round1Number;
    string round1NumStr = to_string(round1Number);
    send(clientSocket, round1NumStr.c_str(), round1NumStr.length(), 0);

    // Receive round 1 results
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving round 1 results" << endl;
        close(clientSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    string round1Result(buffer);
    
    // Parse round 1 results: "ROUND1:num1:num2:score1:score2"
    size_t pos = round1Result.find(':');
    if (pos == string::npos) {
        cerr << "Error: Invalid round 1 result format" << endl;
        close(clientSocket);
        return 1;
    }
    pos = round1Result.find(':', pos + 1); // Skip "ROUND1"
    if (pos == string::npos) {
        cerr << "Error: Invalid round 1 result format" << endl;
        close(clientSocket);
        return 1;
    }
    size_t pos2 = round1Result.find(':', pos + 1);
    size_t pos3 = round1Result.find(':', pos2 + 1);
    size_t pos4 = round1Result.find(':', pos3 + 1);
    
    if (pos2 == string::npos || pos3 == string::npos || pos4 == string::npos) {
        cerr << "Error: Invalid round 1 result format" << endl;
        close(clientSocket);
        return 1;
    }
    
    int round1Num1, round1Num2, round1Score1, round1Score2;
    try {
        round1Num1 = stoi(round1Result.substr(pos + 1, pos2 - pos - 1));
        round1Num2 = stoi(round1Result.substr(pos2 + 1, pos3 - pos2 - 1));
        round1Score1 = stoi(round1Result.substr(pos3 + 1, pos4 - pos3 - 1));
        round1Score2 = stoi(round1Result.substr(pos4 + 1));
        
        cout << "Round 1: Player 1 selected " << round1Num1 
             << ", Player 2 selected " << round1Num2 << endl;
        cout << "Round 1: Player 1 score = " << round1Score1 
             << ", Player 2 score = " << round1Score2 << endl;
    } catch (const exception& e) {
        cerr << "Error parsing round 1 results: " << e.what() << endl;
        cerr << "Received: " << round1Result << endl;
        close(clientSocket);
        return 1;
    }

    // • Once the client receives the information, it should send in a number of round 2
    // ROUND 2
    // Receive request for number
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving round 2 request" << endl;
        close(clientSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';

    // Get number input from user and send to server
    int round2Number;
    if (playerNumber == 1) {
        cout << "Round 2: Enter a number (60-99, non-prime): ";
    } else {
        cout << "Round 2: Enter a number (50-99, non-prime): ";
    }
    cin >> round2Number;
    string round2NumStr = to_string(round2Number);
    send(clientSocket, round2NumStr.c_str(), round2NumStr.length(), 0);

    // Receive round 2 results with final scores and win status
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving round 2 results" << endl;
        close(clientSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    string round2Result(buffer);
    
    // Parse round 2 results: "ROUND2:num1:num2:score1:score2:TOTAL1:TOTAL2:WIN_STATUS"
    pos = round2Result.find(':');
    if (pos == string::npos) {
        cerr << "Error: Invalid round 2 result format" << endl;
        close(clientSocket);
        return 1;
    }
    pos = round2Result.find(':', pos + 1); // Skip "ROUND2"
    if (pos == string::npos) {
        cerr << "Error: Invalid round 2 result format" << endl;
        close(clientSocket);
        return 1;
    }
    pos2 = round2Result.find(':', pos + 1);
    pos3 = round2Result.find(':', pos2 + 1);
    pos4 = round2Result.find(':', pos3 + 1);
    size_t pos5 = round2Result.find(':', pos4 + 1);
    size_t pos6 = round2Result.find(':', pos5 + 1);
    size_t lastPos = round2Result.find(':', pos6 + 1);
    
    if (pos2 == string::npos || pos3 == string::npos || pos4 == string::npos || 
        pos5 == string::npos || pos6 == string::npos || lastPos == string::npos) {
        cerr << "Error: Invalid round 2 result format" << endl;
        close(clientSocket);
        return 1;
    }
    
    int round2Num1, round2Num2, round2Score1, round2Score2, total1, total2, winStatus;
    try {
        round2Num1 = stoi(round2Result.substr(pos + 1, pos2 - pos - 1));
        round2Num2 = stoi(round2Result.substr(pos2 + 1, pos3 - pos2 - 1));
        round2Score1 = stoi(round2Result.substr(pos3 + 1, pos4 - pos3 - 1));
        round2Score2 = stoi(round2Result.substr(pos4 + 1, pos5 - pos4 - 1));
        total1 = stoi(round2Result.substr(pos5 + 1, pos6 - pos5 - 1));
        total2 = stoi(round2Result.substr(pos6 + 1, lastPos - pos6 - 1));
        winStatus = stoi(round2Result.substr(lastPos + 1));
    
        cout << "Round 2: Player 1 selected " << round2Num1 
             << ", Player 2 selected " << round2Num2 << endl;
        cout << "Round 2: Player 1 score = " << round2Score1 
             << ", Player 2 score = " << round2Score2 << endl;
        cout << "Total: Player 1 = " << total1 
             << ", Player 2 = " << total2 << endl;
        
        // • The client, after receiving the information about who wins, should print a statement (you can select what statement to print, it must be different for the 3 cases, and please, no inappropriate language).
        // Print win/lose/draw message based on win status
        if (winStatus == 1) {
            cout << "Congratulations! You won!" << endl;
        } else if (winStatus == -1) {
            cout << "Sorry, you lost. Better luck next time!" << endl;
        } else {
            cout << "It's a draw! Well played!" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error parsing round 2 results: " << e.what() << endl;
        cerr << "Received: " << round2Result << endl;
        close(clientSocket);
        return 1;
    }
    
    // • After that, the client should disconnect from the server, and quit
    close(clientSocket);
    return 0;
}

