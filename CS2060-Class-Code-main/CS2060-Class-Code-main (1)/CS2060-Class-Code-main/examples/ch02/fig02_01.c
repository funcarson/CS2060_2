// Fig. 2.1: fig02_01.c
// A first program in C 
#include <stdio.h>

// function main begins program execution 
int main(void)
{
	//Declares the variables
	double length = 0;
	double width = 0;
	double area = 0;
	double average = 0;


	//Welcomes the user and asks for the length
	printf("Welcome to C!\n");
	puts("Enter the length");

	//Gets the input
	scanf("%d", &length);

	//Asks for the width
	puts("Enter the width");

	//Gets the input
	scanf("%d", &width);

	//Calculates the area
	area = length * width;

	//Displays the area
	printf("The area is: %d\n", area);

	//Divides the length by the width
	average = length / width;

	//displays the average
	printf("The average is: %.1f\n", average);
	return 0;
} // end function main 


/**************************************************************************
 * (C) Copyright 1992-2015 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/
