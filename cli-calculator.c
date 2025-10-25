// Basic calculator project, runs in command line interface
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// It supports addition, subtraction, multiplication and division operations with BODMAS rule.
// Usage: cli-calculator <number1> <operator> <number2> <operator> <number3> ...
int main(int argc, char const *argv[])
{
    // Check if there are enough arguments
    if (argc < 4) {
        printf("Usage: %s <number1> <operator> <number2> [<operator> <number3> ...]\n", argv[0]);
        return 1;
    }

    // Number of numbers
    int numCount = (argc - 1) / 2 + 1;
    double numbers[numCount];
    char operators[numCount - 1];

    // Parse numbers and operators from command line arguments
    numbers[0] = atof(argv[1]);
    for (int i = 1, j = 2; j < argc; i++, j+= 2) {
        operators[i - 1] = argv[j][0];
        numbers[i] = atof(argv[j + 1]);
    }

    // Perform multiplication and division first
    for (int i = 0; i < numCount - 1; i++) {
        if (operators[i] == '*') {
            numbers[i] = numbers[i] * numbers[i + 1];
            // Shift numbers and operators left
            for (int j = i + 1; j < numCount - 1; j++) {
                numbers[j] = numbers[j + 1];
                operators[j - 1] = operators[j];
            }
            numCount--;
            i--; // Stay at the same index
        } else if (operators[i] == '/') {
            if (numbers[i + 1] == 0) {
                printf("Error: Division by zero\n");
                return 1;
            }
            numbers[i] = numbers[i] / numbers[i + 1];
            // Shift numbers and operators left
            for (int j = i + 1; j < numCount - 1; j++) {
                numbers[j] = numbers[j + 1];
                operators[j - 1] = operators[j];
            }
            numCount--;
            i--; // Stay at the same index
        }
    }

    // Perform addition and subtraction
    double result = numbers[0];
    for (int i = 0; i < numCount - 1; i++) {
        if (operators[i] == '+') {
            result  += numbers[i + 1];
        } else if (operators[i] == '-') {
            result  -= numbers[i + 1];
        } else {
            printf("Error: Unknown operator %c\n", operators[i]);
            return 1;
        }
    }

    // Print the result
    printf("Result: %lf\n", result);
    return 0;
}
