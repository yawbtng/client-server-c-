# Client-Server Computing - HCF/LCM Game

## Project Goal

This project implements a client-server application where two players compete in a game based on Highest Common Factor (HCF) and Lowest Common Multiple (LCM) calculations. The server hosts the game and manages communication between two client players.

## Learning Objectives

This project teaches:

- **Client-server architecture**: Understanding how servers listen for connections and clients communicate with servers
- **Network programming**: Implementing socket-based communication between processes
- **Concurrent programming**: Using processes (C++) or threads (Java) to handle multiple clients
- **Inter-process communication**: Coordinating game state and data exchange between server and clients
- **Protocol design**: Defining a communication protocol for game flow and data exchange

## Game Overview

Two players take turns selecting numbers, and scores are calculated using HCF and LCM:
- Player one's score = HCF of both numbers
- Player two's score = last digit of LCM of both numbers

The game consists of two rounds where players switch roles, and the player with the higher cumulative score wins.

## Implementation

- **Server**: Listens on port 24175, manages game logic, calculates scores, and coordinates between two clients
- **Client**: Connects to server, sends player ID and number selections, receives game results

See `3341f25prog.pdf` for detailed specifications and requirements.

## Building the Project

### Prerequisites
- C++ compiler (g++ recommended)
- Make utility
- Unix-like operating system (Linux/macOS) for socket programming

### Build Instructions

1. **Compile both server and client:**
   ```bash
   make
   ```

2. **Compile only the server:**
   ```bash
   make server
   ```

3. **Compile only the client:**
   ```bash
   make client
   ```

4. **Clean compiled executables:**
   ```bash
   make clean
   ```

This will create two executables:
- `server` - The game server
- `client` - The game client

## Running the Game

### Step 1: Start the Server

Open a terminal and run:
```bash
./server
```

The server will start listening on port 24175 and wait for two clients to connect.

### Step 2: Start First Client

Open a second terminal and run:
```bash
./client
```

The first client will be assigned as Player 1. Enter your player ID when prompted.

### Step 3: Start Second Client

Open a third terminal and run:
```bash
./client
```

The second client will be assigned as Player 2. Enter your player ID when prompted.

### Step 4: Play the Game

Once both clients are connected:
1. **Round 1:**
   - Player 1 enters a number between 50-99 (must be non-prime)
   - Player 2 enters a number between 60-99 (must be non-prime)
   - Scores are calculated and displayed

2. **Round 2:**
   - Players switch roles
   - Player 2 enters a number between 50-99 (must be non-prime)
   - Player 1 enters a number between 60-99 (must be non-prime)
   - Final scores are calculated and winner is determined

### Game Rules

- **Valid numbers for Player 1 (Round 1)**: 50-99, non-prime
- **Valid numbers for Player 2 (Round 1)**: 60-99, non-prime
- **Scoring:**
  - Round 1: Player 1 gets HCF, Player 2 gets last digit of LCM
  - Round 2: Player 2 gets HCF, Player 1 gets last digit of LCM
- **Penalties:**
  - Invalid number (prime or out of range): 0 points for player, 100 points for opponent
  - Duplicate number (same number in both rounds): 0 points for player, 100 points for opponent
  - If both players are invalid: both get 0 points

### Example Valid Numbers

Non-prime numbers between 50-99:
50, 51, 52, 54, 55, 56, 57, 58, 60, 62, 63, 64, 65, 66, 68, 69, 70, 72, 74, 75, 76, 77, 78, 80, 81, 82, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 98, 99

## File Structure

```
client-server/
├── server.cpp          # Server implementation
├── client.cpp          # Client implementation
├── utils.h             # Utility function declarations
├── utils.cpp           # Utility function implementations
├── hcf.cpp             # Original HCF function (reference)
├── Makefile            # Build configuration
├── README.md           # This file
└── 3341f25prog.pdf    # Assignment specifications
```

## Troubleshooting

- **"Address already in use"**: Another server may be running on port 24175. Kill it or wait for it to finish.
- **Connection refused**: Make sure the server is running before starting clients.
- **Compilation errors**: Ensure you have g++ installed and are using a Unix-like system.
