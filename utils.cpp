#include "utils.h"
#include <cmath>

// HCF (Highest Common Factor) using Euclidean algorithm
int hcf(int a, int b)
{
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// LCM (Lowest Common Multiple) calculation
// LCM(a, b) = a * b / HCF(a, b)
int lcm(int a, int b)
{
    if (a == 0 || b == 0) {
        return 0;
    }
    return (a * b) / hcf(a, b);
}

// Check if a number is prime
bool isPrime(int n)
{
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    
    // Check divisibility up to sqrt(n)
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

// Validate number for player 1 (range 50-99, non-prime)
bool isValidPlayer1Number(int n)
{
    // Check range
    if (n < 50 || n > 99) {
        return false;
    }
    // Check if non-prime
    if (isPrime(n)) {
        return false;
    }
    return true;
}

// Validate number for player 2 (range 60-99, non-prime)
bool isValidPlayer2Number(int n)
{
    // Check range
    if (n < 60 || n > 99) {
        return false;
    }
    // Check if non-prime
    if (isPrime(n)) {
        return false;
    }
    return true;
}

// Get last digit of a number
int getLastDigit(int n)
{
    return abs(n) % 10;
}

