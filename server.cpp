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

    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket to port " << PORT << endl;
        close(serverSocket);
        return 1;
    }

    // • The server should be started at the background, and it will listen for client connecting. (Use port 24175 for this program)
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

    // • Once the server accepts a connection from the client, it should inform the client which player (player one or player two) that he/she is.
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

    // • Once the server collects the names, it should print the statement "<player one ID> vs. <player two ID>: Game start".
    // Print game start message
    cout << player1ID << " vs. " << player2ID << ": Game start" << endl;

    // Game variables
    int player1Score = 0;
    int player2Score = 0;
    int round1Player1Num = 0, round1Player2Num = 0;
    int round2Player1Num = 0, round2Player2Num = 0;
    int round1Player1Score = 0, round1Player2Score = 0;
    int round2Player1Score = 0, round2Player2Score = 0;

    // • Then the server should send a request to ask the two players to send in the first number.
    // ROUND 1: Player 1 picks first (50-99), Player 2 picks second (60-99)
    // Request numbers from both players
    string requestMsg = "REQUEST_NUMBER";
    send(client1Socket, requestMsg.c_str(), requestMsg.length(), 0);
    send(client2Socket, requestMsg.c_str(), requestMsg.length(), 0);

    // Receive Player 1 number for round 1
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(client1Socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving Player 1 number for round 1" << endl;
        close(client1Socket);
        close(client2Socket);
        close(serverSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    round1Player1Num = stoi(string(buffer));

    // Receive Player 2 number for round 1
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(client2Socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving Player 2 number for round 1" << endl;
        close(client1Socket);
        close(client2Socket);
        close(serverSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    round1Player2Num = stoi(string(buffer));

    // Validate numbers for round 1
    bool player1Valid = isValidPlayer1Number(round1Player1Num);
    bool player2Valid = isValidPlayer2Number(round1Player2Num);

    // Calculate scores for round 1 with validation
    if (!player1Valid && !player2Valid) {
        // Both invalid: both get 0 points
        round1Player1Score = 0;
        round1Player2Score = 0;
    } else if (!player1Valid) {
        // Player 1 invalid: Player 1 gets 0, Player 2 gets 100
        round1Player1Score = 0;
        round1Player2Score = 100;
    } else if (!player2Valid) {
        // Player 2 invalid: Player 2 gets 0, Player 1 gets 100
        round1Player1Score = 100;
        round1Player2Score = 0;
    } else {
        // Both valid: normal scoring
        // Player 1 score = HCF of both numbers
        // Player 2 score = last digit of LCM of both numbers
        round1Player1Score = hcf(round1Player1Num, round1Player2Num);
        int lcmValue = lcm(round1Player1Num, round1Player2Num);
        round1Player2Score = getLastDigit(lcmValue);
    }

    // Update total scores
    player1Score += round1Player1Score;
    player2Score += round1Player2Score;

    // • Once the server receives the numbers, it should calculate the score for this round. And then sent the score of both players, together with the numbers both player picked, back to the client.
    // Send round 1 results to both clients
    // Format: "ROUND1:num1:num2:score1:score2"
    string round1Result = "ROUND1:" + to_string(round1Player1Num) + ":" + 
                          to_string(round1Player2Num) + ":" + 
                          to_string(round1Player1Score) + ":" + 
                          to_string(round1Player2Score);
    send(client1Socket, round1Result.c_str(), round1Result.length(), 0);
    send(client2Socket, round1Result.c_str(), round1Result.length(), 0);

    // • The server should then print the result of round 1: including the number selected, and the score of each player.
    // Print round 1 results
    cout << "Round 1: Player 1 selected " << round1Player1Num 
         << ", Player 2 selected " << round1Player2Num << endl;
    cout << "Round 1: Player 1 score = " << round1Player1Score 
         << ", Player 2 score = " << round1Player2Score << endl;

    // ROUND 2: Players switch roles
    // Player 2 picks first (50-99), Player 1 picks second (60-99)
    // Request numbers from both players
    send(client1Socket, requestMsg.c_str(), requestMsg.length(), 0);
    send(client2Socket, requestMsg.c_str(), requestMsg.length(), 0);

    // Receive Player 2 number for round 2 (they pick first now)
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(client2Socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving Player 2 number for round 2" << endl;
        close(client1Socket);
        close(client2Socket);
        close(serverSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    round2Player2Num = stoi(string(buffer));

    // Receive Player 1 number for round 2 (they pick second now)
    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(client1Socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving Player 1 number for round 2" << endl;
        close(client1Socket);
        close(client2Socket);
        close(serverSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    round2Player1Num = stoi(string(buffer));

    // Validate numbers for round 2
    // In round 2, Player 2 picks first (50-99), Player 1 picks second (60-99)
    bool round2Player2Valid = isValidPlayer1Number(round2Player2Num);
    bool round2Player1Valid = isValidPlayer2Number(round2Player1Num);

    // Check for duplicate numbers (same number in both rounds)
    bool player1Duplicate = (round1Player1Num == round2Player1Num);
    bool player2Duplicate = (round1Player2Num == round2Player2Num);

    // Calculate scores for round 2 with validation
    if ((!round2Player2Valid || player2Duplicate) && (!round2Player1Valid || player1Duplicate)) {
        // Both invalid: both get 0 points
        round2Player2Score = 0;
        round2Player1Score = 0;
    } else if (!round2Player2Valid || player2Duplicate) {
        // Player 2 invalid: Player 2 gets 0, Player 1 gets 100
        round2Player2Score = 0;
        round2Player1Score = 100;
    } else if (!round2Player1Valid || player1Duplicate) {
        // Player 1 invalid: Player 1 gets 0, Player 2 gets 100
        round2Player2Score = 100;
        round2Player1Score = 0;
    } else {
        // Both valid: normal scoring
        // In round 2, Player 2 gets HCF, Player 1 gets last digit of LCM
        round2Player2Score = hcf(round2Player1Num, round2Player2Num);
        int lcmValue = lcm(round2Player1Num, round2Player2Num);
        round2Player1Score = getLastDigit(lcmValue);
    }

    // Update total scores
    player1Score += round2Player1Score;
    player2Score += round2Player2Score;

    // • Then the server should calculate the score for round 2, and send the following tp the players: The score of round 2 for each player, The total score for each player, Whether the player win/lose/draw the match (sending 1/-1/0 respectively)
    // Determine winner (1 = win, -1 = lose, 0 = draw)
    int player1WinStatus = 0;
    int player2WinStatus = 0;
    if (player1Score > player2Score) {
        player1WinStatus = 1;
        player2WinStatus = -1;
    } else if (player2Score > player1Score) {
        player1WinStatus = -1;
        player2WinStatus = 1;
    } else {
        player1WinStatus = 0;
        player2WinStatus = 0;
    }

    // Send round 2 results and final scores to both clients
    // Format: "ROUND2:num1:num2:score1:score2:TOTAL1:TOTAL2:WIN_STATUS"
    string round2Result = "ROUND2:" + to_string(round2Player1Num) + ":" + 
                          to_string(round2Player2Num) + ":" + 
                          to_string(round2Player1Score) + ":" + 
                          to_string(round2Player2Score) + ":" + 
                          to_string(player1Score) + ":" + 
                          to_string(player2Score) + ":" + 
                          to_string(player1WinStatus);
    send(client1Socket, round2Result.c_str(), round2Result.length(), 0);
    
    // Send Player 2's win status (opposite of Player 1)
    string round2Result2 = "ROUND2:" + to_string(round2Player1Num) + ":" + 
                           to_string(round2Player2Num) + ":" + 
                           to_string(round2Player1Score) + ":" + 
                           to_string(round2Player2Score) + ":" + 
                           to_string(player1Score) + ":" + 
                           to_string(player2Score) + ":" + 
                           to_string(player2WinStatus);
    send(client2Socket, round2Result2.c_str(), round2Result2.length(), 0);

    // • The server should then print the result of round 2: including the number selected, and the score of each player, and then print out who is the winner.
    // Print round 2 results
    cout << "Round 2: Player 1 selected " << round2Player1Num 
         << ", Player 2 selected " << round2Player2Num << endl;
    cout << "Round 2: Player 1 score = " << round2Player1Score 
         << ", Player 2 score = " << round2Player2Score << endl;
    cout << "Total: Player 1 = " << player1Score 
         << ", Player 2 = " << player2Score << endl;
    
    // Print winner
    if (player1Score > player2Score) {
        cout << "Winner: " << player1ID << endl;
    } else if (player2Score > player1Score) {
        cout << "Winner: " << player2ID << endl;
    } else {
        cout << "Result: Draw" << endl;
    }
    
    // • The server should also quit.
    // Close connections
    close(client1Socket);
    close(client2Socket);
    close(serverSocket);
    return 0;
}

