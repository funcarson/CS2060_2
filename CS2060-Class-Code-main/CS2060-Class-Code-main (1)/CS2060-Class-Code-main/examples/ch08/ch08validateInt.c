//Librarys
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Constants
#define LENGTH 80

//Prototypes
bool exploreValidateInt(const char* buff, int* validInt);
void printLimits();

//Main
int main(void)
{
	//Creates the string or char array
	char inputStr[LENGTH]; // create char arintray

	//Initializes the variable for the input length
	size_t inputLength = 0;

    //Intializes the variable for valid integer
    int validInt = 0;

    //Initalizes the bool isValid
    bool isValid = false;

	//Prints the limits of the data types
	printLimits();
    while (!isValid) {
        puts("\nEnter an integer");

        //Gets the user input
        fgets(inputStr, LENGTH, stdin);

        //Gets the input length
        inputLength = strnlen(inputStr, LENGTH);

        //Adds the EOF character at the end of the input to make it stop reading once the end of line character is read
        if (inputLength > 0 && inputStr[inputLength - 1] == '\n')
        {
            inputStr[inputLength - 1] = '\0';
            inputLength--;
        }
        //Calls the validate intmethod
        isValid = exploreValidateInt(inputStr, &validInt);
    }
}//End of main


//Start of print limits
void printLimits()
{
	printf("The number of bits in a byte %d\n", CHAR_BIT);

	printf("The minimum value of SIGNED CHAR = %d\n", SCHAR_MIN);
	printf("The maximum value of SIGNED CHAR = %d\n", SCHAR_MAX);
	printf("The maximum value of UNSIGNED CHAR = %d\n", UCHAR_MAX);

	printf("The minimum value of SHORT INT = %d\n", SHRT_MIN);
	printf("The maximum value of SHORT INT = %d\n", SHRT_MAX);

	printf("The minimum value of INT = %d\n", INT_MIN);
	printf("The maximum value of INT = %d\n", INT_MAX);

	printf("The minimum value of CHAR = %d\n", CHAR_MIN);
	printf("The maximum value of CHAR = %d\n", CHAR_MAX);

	printf("The minimum value of LONG = %ld\n", LONG_MIN);
	printf("The maximum value of LONG = %ld\n", LONG_MAX);
}//End of print Limits

//Start of validateInput
bool exploreValidateInt(const char* buff, int* validInt) {
    // Creates the char pointer end
    char* end;

    // Initalizes errno
    errno = 0;

    // Creates the long variable called intTest
    long intTest = strtol(buff, &end, 10);

    // Checks if it's a decimal number
    if (end == buff) {
        fprintf(stderr, "%s: not a decimal number\n", buff);
        return false;
    }
    // Checks if there are extra characters
    else if ('\0' != *end) {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
        return false;
    }
    // Checks if it's out of range
    else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", buff);
        return false;
    }
    // Checks if it's greater than the max
    else if (intTest > INT_MAX) {
        fprintf(stderr, "%ld greater than INT_MAX\n", intTest);
        return false;
    }
    // Checks if it's less than the min
    else if (intTest < INT_MIN) {
        fprintf(stderr, "%ld less than INT_MIN\n", intTest);
        return false;
    }
    // If it passes all checks, it confirms it's an integer
    else {
        *validInt = (int)intTest;
        printf("%d is integer value ", *validInt);
        return true;
    }
}//End of validateInt
