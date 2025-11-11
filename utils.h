#ifndef UTILS_H
#define UTILS_H

// HCF (Highest Common Factor) function
int hcf(int a, int b);

// LCM (Lowest Common Multiple) calculation
// LCM(a, b) = a * b / HCF(a, b)
int lcm(int a, int b);

// Check if a number is prime
bool isPrime(int n);

// Validate number for player 1 (range 50-99, non-prime)
bool isValidPlayer1Number(int n);

// Validate number for player 2 (range 60-99, non-prime)
bool isValidPlayer2Number(int n);

// Get last digit of a number
int getLastDigit(int n);

#endif // UTILS_H

