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
