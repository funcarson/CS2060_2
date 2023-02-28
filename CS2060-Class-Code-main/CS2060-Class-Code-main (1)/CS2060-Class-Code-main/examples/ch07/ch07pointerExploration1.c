/*******************
 Understanding pointers

 Add comments to explain the code
**************/

#include <stdio.h>


int main ()
{
	// Initalize houseNum
	int houseNum = 13;
	int calcHouseNum1 = 0;
	int calcHouseNum2 = 0;
	int *houseNumPtr = &houseNum;
	
	//Prints the house num
	printf("houseNum %d\n", houseNum);

	//Prints the address of the base variable
	printf("&houseNum %p\n\n", &houseNum);

	//Prints pointer variable address
	printf ("*houseNumPtr  %d\n", *houseNumPtr);

	//Prints pointer variable
	printf ("houseNumPtr %p\n\n", houseNumPtr);

	//Prints pointer variable address
	printf ("&houseNumPtr %p\n\n", &houseNumPtr);

	//Calulates houseNum1
	calcHouseNum1 =  *houseNumPtr + *houseNumPtr;

	//Prints the calculated house number
	printf("*houseNumPtr + *houseNumPtr is %d\n\n", calcHouseNum1);

	//gets Housenum2
	calcHouseNum2 = 2 * (*houseNumPtr);

	//Prints housenum2
	printf("2 * (*houseNumPtr) is %d\n\n", calcHouseNum2);

	//Prints the housenumPtr address twice
	printf("\n&*houseNumPtr = %p"
		"\n*&houseNumPtr = %p\n", &*houseNumPtr, *&houseNumPtr);
			
	return 0;
}
