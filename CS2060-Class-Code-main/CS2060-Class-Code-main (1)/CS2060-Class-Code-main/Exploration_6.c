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
void setUpOrganization(Organization* org1);
void displayOrganization(Organization org);

//Creates Organization structure
typedef struct {
    char organizationName[LENGTH];
    char fullName[LENGTH];
    char fundraiserPurpose[LENGTH];
    double goalAmount;
} Organization;

int main(void)
{
    //Declares the organization structure
    Organization org1;

    //Calls the setUpOrganization function
    setUpOrganization(&org1);
}

//Sets up the organization
void setUpOrganization(Organization* org) {
    puts("Enter organization name: ");
    fgets(&org->organizationName, LENGTH, stdin);
    puts("Enter fundraiser purpose: ");
    fgets(&org->fundraiserPurpose, LENGTH, stdin);
    puts("Enter first and last name: ");
    fgets(&org->fullName, LENGTH, stdin);
    puts("Enter goal amount: ");
    scanf("%lf", &org->goalAmount);
}//End of setUpOrganization

//Start of display
void displayOrganization(Organization org) {
    printf("Organization Name: %s\n", org.organizationName);
    printf("First and Last Name: %s\n", org.fullName);
    printf("Fundraiser Purpose: %s\n", org.fundraiserPurpose);
    printf("Goal Amount: %.2f\n", org.goalAmount);
}//End of display

