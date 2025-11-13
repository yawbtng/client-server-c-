# Testing Guide for Client-Server HCF/LCM Game

## Quick Start Testing

### Method 1: Manual Testing (Recommended for First Time)

You need **3 terminal windows** open:

#### Terminal 1: Start the Server
```bash
cd "/Users/yawbt/Documents/ECE 3341/client-server"
./server
```

Expected output:
```
Server listening on port 24175...
```

#### Terminal 2: Start First Client (Player 1)
```bash
cd "/Users/yawbt/Documents/ECE 3341/client-server"
./client
```

Expected flow:
1. You'll see: "Connected to server"
2. You'll see: "You are Player 1"
3. Enter your ID (e.g., "Alice")
4. When prompted, enter a number between 50-99 (non-prime) for Round 1
5. When prompted, enter a number between 60-99 (non-prime) for Round 2

#### Terminal 3: Start Second Client (Player 2)
```bash
cd "/Users/yawbt/Documents/ECE 3341/client-server"
./client
```

Expected flow:
1. You'll see: "Connected to server"
2. You'll see: "You are Player 2"
3. Enter your ID (e.g., "Bob")
4. When prompted, enter a number between 60-99 (non-prime) for Round 1
5. When prompted, enter a number between 50-99 (non-prime) for Round 2

### Test Cases

#### Test Case 1: Valid Numbers (Normal Game)
- Player 1 Round 1: `60` (valid: 50-99, non-prime)
- Player 2 Round 1: `72` (valid: 60-99, non-prime)
- Player 1 Round 2: `84` (valid: 60-99, non-prime)
- Player 2 Round 2: `55` (valid: 50-99, non-prime)

Expected: Normal scoring with HCF and LCM calculations

#### Test Case 2: Invalid Number (Prime)
- Player 1 Round 1: `53` (prime - INVALID)
- Player 2 Round 1: `72` (valid)

Expected: Player 1 gets 0 points, Player 2 gets 100 points

#### Test Case 3: Invalid Number (Out of Range)
- Player 1 Round 1: `45` (below 50 - INVALID)
- Player 2 Round 1: `72` (valid)

Expected: Player 1 gets 0 points, Player 2 gets 100 points

#### Test Case 4: Duplicate Number
- Player 1 Round 1: `60` (valid)
- Player 1 Round 2: `60` (same as Round 1 - INVALID)

Expected: Player 1 gets 0 points for Round 2, Player 2 gets 100 points

#### Test Case 5: Both Invalid
- Player 1 Round 1: `53` (prime - INVALID)
- Player 2 Round 1: `59` (prime - INVALID)

Expected: Both get 0 points

## Valid Non-Prime Numbers Reference

**Player 1 Round 1 / Player 2 Round 2 (50-99):**
50, 51, 52, 54, 55, 56, 57, 58, 60, 62, 63, 64, 65, 66, 68, 69, 70, 72, 74, 75, 76, 77, 78, 80, 81, 82, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 98, 99

**Player 2 Round 1 / Player 1 Round 2 (60-99):**
60, 62, 63, 64, 65, 66, 68, 69, 70, 72, 74, 75, 76, 77, 78, 80, 81, 82, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 98, 99

## Troubleshooting

### "Address already in use"
- Another server instance is running
- Solution: Kill the existing server or wait for it to finish
- Find and kill: `lsof -ti:24175 | xargs kill -9`

### "Connection refused"
- Server is not running
- Solution: Start the server first in Terminal 1

### Server exits immediately
- Check for compilation errors
- Rebuild: `make clean && make`

### Numbers not being accepted
- Make sure numbers are:
  - In correct range (50-99 for Player 1 Round 1, 60-99 for Player 2 Round 1)
  - Non-prime
  - Different in Round 2 (no duplicates)

## Expected Server Output Example

```
Server listening on port 24175...
First client connected (Player 1)
Second client connected (Player 2)
Alice vs. Bob: Game start
Round 1: Player 1 selected 60, Player 2 selected 72
Round 1: Player 1 score = 12, Player 2 score = 0
Round 2: Player 1 selected 84, Player 2 selected 55
Round 2: Player 1 score = 0, Player 2 score = 5
Total: Player 1 = 12, Player 2 = 5
Winner: Alice
```

## Expected Client Output Example

**Player 1:**
```
Connected to server
You are Player 1
Enter your ID: Alice
Round 1: Enter a number (50-99, non-prime): 60
Round 1: Player 1 selected 60, Player 2 selected 72
Round 1: Player 1 score = 12, Player 2 score = 0
Round 2: Enter a number (60-99, non-prime): 84
Round 2: Player 1 selected 84, Player 2 selected 55
Round 2: Player 1 score = 0, Player 2 score = 5
Total: Player 1 = 12, Player 2 = 5
Congratulations! You won!
```

**Player 2:**
```
Connected to server
You are Player 2
Enter your ID: Bob
Round 1: Enter a number (60-99, non-prime): 72
Round 1: Player 1 selected 60, Player 2 selected 72
Round 1: Player 1 score = 12, Player 2 score = 0
Round 2: Enter a number (50-99, non-prime): 55
Round 2: Player 1 selected 84, Player 2 selected 55
Round 2: Player 1 score = 0, Player 2 score = 5
Total: Player 1 = 12, Player 2 = 5
Sorry, you lost. Better luck next time!
```

