//This program gets the fundrasing admins info, creates a fundrasier, than is set into "donor mode" where donation amount, name, and zipcode 
//is asked for (with or wothout a reciept) until the admin enters the special key and  sees the final summmary of the fundrasier

#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

//Constants
#define LENGTH 80
#define MIN 0.0
#define CAPSMIN 1
#define LOWERMIN 1
#define NUMBERMIN 1
#define PASSWORDLENGTH 7
#define PROCESSINGFEE 0.031

//New constants
const char* orgUrlPrefix = "https:donate.com/";
const char* orgUrlSuffix = "?form=popup#";

//Creates Organization structure
typedef struct {
    char organizationName[LENGTH];
    char fullName[LENGTH];
    char fundraiserPurpose[LENGTH];
    char repName[LENGTH];
    char repPassword[LENGTH];
    char repEmail[LENGTH];
    double goalAmount;
    char fundrasierURL[LENGTH];
    double donationTotal;
    unsigned int amountOfDonors;
    double totalProcessingFees;
} Organization;

//Removed time variables as they are not needed

//Prototypes
void setUpOrganization(Organization* org1);
void displayOrganizationCreation(Organization org);
bool emailValid(char email[]);
bool passwordIsValid(char password[]);
void organizationIntro(Organization org);
bool getDonation(Organization* org);
void finalSummary(Organization org);
bool validateZipCode(char zipCode[]);
void displayReceipt(const char* orgName, double donation);
void addEOL(char* str);
bool validateDouble(const char*stringInputPtr, double *outputPtr, int min);
void createUrl(char* url, const char* prefix, const char* suffix, const char* orgName);
void clearInputBuffer();

int main(void)
{
    //Declares the organization structure
    Organization org1;

    //Declares donor structure

    //Initalizes some struct variables
    double total = 0.0;
    unsigned int donors = 0;
    double processing = 0.0;

    org1.donationTotal = total;
    org1.totalProcessingFees = processing;
    org1.amountOfDonors = donors;

    //Intializes a bool variable
    bool endProgram = false;

    //Calls the setUpOrganization function
    setUpOrganization(&org1);

    //Displays the organization
    displayOrganizationCreation(org1);


    //Creates a loop that can only break if they have admin credential
    while (!endProgram) {

        //Organization Intro
        organizationIntro(org1);
        
        //Gets the donation and if the prgram will end 
        endProgram = getDonation(&org1); //needs to impliment double validation function, need to fix admin mode
        
    }//End of while loop
    finalSummary(org1);
}//End of main

//Sets up the organization and its represenitive
void setUpOrganization(Organization* org) {
    
    //Creates a bool if input is incorrect
    bool isValid = false;

    //Creates temp variables
    char tempGoal[LENGTH];
    char tempEmail[LENGTH];
    char tempPassword[LENGTH];
    char url[LENGTH];
    unsigned int min = 0;
    double goal = 0.0;

    //Creates pointers
    char (*goalStringPtr)[LENGTH] = &tempGoal;
    double* goalPtr = &goal;
    char(*urlPtr)[LENGTH] = &url;

    //Gets the organization information
    puts("Enter organization name: ");
    fgets(org->organizationName, LENGTH, stdin);
    puts("Enter fundraiser purpose: ");
    fgets(org->fundraiserPurpose, LENGTH, stdin);

    //Gets the representatives info
    puts("Enter first and last name: ");
    fgets(org->fullName, LENGTH, stdin);

    //Creates a name pointer
    const char(*orgNamePtr)[LENGTH] = &org->organizationName;

    //Gets the goal
  
    //Tests if the goal is valid
    while (!isValid) {
       puts("Enter goal amount: ");
       fgets(tempGoal, LENGTH, stdin);

       //Checks if the goal is valid
       isValid = validateDouble(goalStringPtr, goalPtr, min);

       //adds the goal to the strcuture
       if (isValid) {
           double goal = strtod(tempGoal, NULL);
           org->goalAmount = goal;
       }
    }

    //resets isValid and creates a counter
    isValid = false;
    int counter = 0;

    //Gets the email
    while (!isValid) {
       
       //gets email
       puts("Enter email address ");
       fgets(tempEmail, LENGTH, stdin);
   
       //Checks if email is valid
       isValid = emailValid(tempEmail);
       counter++;
       if (isValid) {
           strcpy(org->repEmail, tempEmail);
       }
    }

    //resets isValid and the counter
    isValid = false;
    counter = 0;

    //Gets the password
    while (!isValid) {

        //Gets password
        puts("Enter password");
        fgets(tempPassword, LENGTH, stdin);
        //Checks if email is valid
        isValid = passwordIsValid(tempPassword);
        counter++;
        if (isValid) {
            strcpy(org->repPassword, tempPassword);
        }
    }

    //Creates the url
    createUrl(urlPtr, orgUrlPrefix, orgUrlSuffix, orgNamePtr);
    strcpy(org->fundrasierURL, url);

}//End of setUpOrganization

bool emailValid(char email[]) {
    

    //Creates the Loop

        char userName[LENGTH];
        char domain[LENGTH];
        char threeLetterExtension[LENGTH];
        bool isThree = false;
        bool isDomain = false;
        bool isUserName = true;
        bool isAt = false;
        bool isPeriod = false;
        bool isNo = false;
        bool isValid = false;
        int extensionCounter = -1;
        

        //Gets the length
        int len = strlen(email);

        int atCounter = 0;
        int periodCounter = 0;
        int validCounter = 0;
        int extensionLen = 0;

        //Checks if there is more than one @ or .
        for (int i = 0; i < len; i++) {
            if (email[i] == '@') {
                atCounter++;
            }
            else if (email[i] == '.') {
                periodCounter++;
            }
        }//End of @ counter

        //Gets the domain, user, and three letter extension
        for (int i = 0; i < len; i++) {
            if (isUserName) {
                userName[i] = email[i];
                if (email[i] == '@') {
                    userName[i] = '\0';
                    isAt = true;
                    isUserName = false;
                    isDomain = true;
                }
            }
            else if (isDomain) {
                domain[i] = email[i];
                if (domain[i] == '.') {
                    domain[i] = '\0';
                    isDomain = false;
                    isPeriod = true;
                }
            }
            else {
                threeLetterExtension[i] = email[i];
                extensionCounter++;
            }
            
        }

        threeLetterExtension[LENGTH -1] = '\0';
        //Checks if the email is valid (I put them all together and just kept getting erros, despite this ways ineffeincy it works)
        if (extensionCounter == 3) {
            validCounter++;
        }
        if (periodCounter == 1){
            validCounter++;
        }
        if (atCounter = 1) {
            validCounter++;
        }
        if (isAt && isPeriod) {
            validCounter++;
        }//End of individual validators

        //If the email is valid
        isNo = false;
        if (validCounter == 4) {
            puts("Is this email correct? (y)es or (n)o");
            char validEmail = getchar();
            while (!isNo) {
                if (validEmail == 'Y' || validEmail == 'y') {
                    isNo = true;
                    isValid = true;
                    clearInputBuffer();
                }
                else if (validEmail == 'N' || validEmail == 'n') {
                    isNo = true;
                    clearInputBuffer();
                }
                else {
                    clearInputBuffer();
                    puts("Incorrect input please try again");
                    puts("Is this email correct? (y)es or (n)o");
                    validEmail = getchar();
                }
            }
        }
        else {
            puts("incorrect email please try again");
            isNo = true;

        }//End of while loop

        return isValid;
}//End of method
bool passwordIsValid(char password[]) {
        
     //Creates temp variables
     int passNumber = 0;
     int passCaps = 0;
     int passLower = 0;
     bool passLength = false;
     bool validPassword = false;

     //gets password length
     int length = strlen(password);
        
     //Checks for the correct password length
     if (length >= PASSWORDLENGTH) {
         passLength = true;
     }
        
     //Checks for differnt requists of the password
     for (int i = 0; i < length; i++) {

     //Checks for amount of numbers in password
         if (isdigit(password[i])) {
            passNumber++;
            }

         //Checks for amount of capslocks in password
         else if (isupper(password[i])) {
            passCaps++;
         }

         //Checks for amount of lower case in password
         else if (islower(password[i])) {
            passLower++;
         }
     }

     //Checks if all password prerequsits are met if not they have to enter a new one
     if (passLength && passLower >= LOWERMIN && passCaps >= CAPSMIN && passNumber >= NUMBERMIN) {
         validPassword = true;
     }
     else {
         puts("Incorrect Password");
     }
     return validPassword;
}//End of password

//Start of display
void displayOrganizationCreation(Organization org) {
    printf("Thank you %s. The url to raise funds for %s is %s\n", org.fullName, org.organizationName, org.fundrasierURL);
    puts("");
}//End of display

//Start of intro
void organizationIntro(Organization org) {
    
    //Creates some temp variables
    double total = org.donationTotal;
    double goal = org.goalAmount;
    double percentComplete = 0.0;
    percentComplete = (total / goal) * 100;

    //Prints the basic info
    printf("%s\n", org.fundrasierURL);
    puts("");
    puts("MAKE A DIFFERENCE BY YOUR DONATION");
    puts("");
    printf("Organization: %s\n", org.organizationName);
    printf("Purpose: %s\n", org.fundraiserPurpose);
    if (total <= 0) {
        puts("We currently have raised 0 dollars");
    }
    else {
        printf("We currently have raised %.2f dollars \n", total);
    }
    


    //Checks if goal is met
    if (org.goalAmount <= org.donationTotal) {
        puts("We have reached our goal but can still use the donations.");
    }
    else {
            printf("We are %.2f percent towards our goal of %.2f\n", percentComplete, org.goalAmount);

    }
}//End of intro
bool getDonation(Organization* org) {

    //temp variables
    char tempDonation[LENGTH];
    bool endProgram = false;
    int emailCounter = 0;
    int passwordCounter = 0;
    char userEmail[LENGTH];
    char userPassword[LENGTH];
    bool isValid = false;
    unsigned int min = 0;
    double donation = 0.0;
    char name[LENGTH];
    char zipCode[LENGTH];

    //Creates the pointers
    char (*tempDonationPtr)[LENGTH] = &tempDonation;
    double* donationPtr = &donation;
    const char(*orgNamePtr)[LENGTH] = &org->organizationName;

    while (!isValid){
        puts("Enter your donation");
        fgets(tempDonation, LENGTH, stdin);
        
        //Admin section
        if (tempDonation[0] == 'q' || tempDonation[0] == 'Q') {

            //Checks if there is a correct amount of email attempts
            while (emailCounter <= 2 && passwordCounter <= 2) {
                puts("Enter your email");
                fgets(userEmail, LENGTH, stdin);

                //If the emails are the same
                if (strcmp(userEmail, org->repEmail) == 0) {
                    while (passwordCounter <= 2) {

                        //Gets the password
                        puts("Enter your password");
                        fgets(userPassword, LENGTH, stdin);

                        //If the passwords are the same
                        if (strcmp(userPassword, org->repPassword) == 0) {
                            endProgram = true;
                            isValid = true;
                            passwordCounter = 3;
                            emailCounter = 3;
                        }
                        else {
                            puts("Incorrect Password please try again");
                            passwordCounter++;
                        }
                        
                    }//End of password while

                }
                else {
                    puts("incorrect email please try again");
                    emailCounter++;
                }//End of comparisons
            }
        }//End of admin section

        else {
            
            //Checks if its valid
            isValid = validateDouble(tempDonationPtr, donationPtr, min);
            if (isValid){
                
                //Gets the donor name
                puts("Enter your name");
                fgets(name, LENGTH, stdin);

                //resets the bool
                isValid = false;

                //Gets the zipcode
                while (!isValid) {
                    puts("Enter your 5 digit zipcode");
                    fgets(zipCode, LENGTH, stdin);

                    //Checks if the goal is valid
                    isValid = validateZipCode(zipCode);
                }

                //When zipcode is correct, the processing fee is calculated and totals are added
                double fee = donation * PROCESSINGFEE;
                double actualDonation = donation - fee;
                org->totalProcessingFees += fee;
                printf("Thank you for your donation. There is a %.3f percent credit card processing fee of %.2f. %.2f will be donated \n", PROCESSINGFEE, fee, actualDonation);
                puts("");
                org->donationTotal += actualDonation;
                org->amountOfDonors++;

                //Displays the recepit
                displayReceipt(orgNamePtr, donation);
                clearInputBuffer();
            }
        }
    }
    return endProgram;
}//End of get donation

//Prints the admin details
void finalSummary(Organization org) {
    printf("Organization name: %s\n", org.organizationName);
    printf("Total number of donations: %d\n", org.amountOfDonors);
    printf("Total amount raised: %.2f\n", org.donationTotal);
    printf("Total amount paid for credit card processing: %.2f\n", org.totalProcessingFees);
}
//End of admin details


//Start of get zipcode
bool validateZipCode(char zipCode[]) {
    //Creates some temp variables
    bool isValid = false;
    

     //Creates a temp counter
     int numCounter = 0;

     //Gets the zipcode length
     int length = strlen(zipCode);

     //Checks if its a valid integer and the amount of characters at the same time
     for (int i = 0; i < length; i++) {
         if (isdigit(zipCode[i])) {
             numCounter++;
         }
     }

     //Checks if the zipcode is correct
     if(numCounter == 5) {
         isValid = true;
     }
     else {
        puts("Invalid Zipcode, Any 5 digit number will be accepted, even 00000.");
     }

     //Returns a boolean
     return isValid;

}//End of getZip

//Displays the Receipt
void displayReceipt(const char* orgName, double donation) {

    //Creates a receipt variable
    char receiptAnwser;
    bool isValid = false;

    //Time variables
    time_t currentTime;
    struct tm* timeInfo;
    char timeString[9];
    char dateString[20];

    //Gets the current time and date
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(timeString, sizeof(timeString), "%I:%M %p", timeInfo);
    strftime(dateString, sizeof(dateString), "%m/%d/%Y", timeInfo);

    while (!isValid) {
        
        //Asks if the user wants a receipt
        puts("Do you want a receipt, (y)es or (n)o?");
        receiptAnwser = getchar();

        //If the user wants a receipt
        if (receiptAnwser == 'y' || receiptAnwser == 'Y') {
            isValid = true;
            printf("Organization: %s\n", orgName);
            printf("Donation Amount %.2f\n", donation);
            printf("Donation Date: %s - %s\n", dateString, timeString);
        }
        else if (receiptAnwser == 'n' || receiptAnwser == 'N') {
            isValid = true;
        }
        else {
            puts("Invalid anwser please try again");
            puts("");
        }
    }



}//end of displayReceipt

//End of line character method
void addEOL(char* str) {
    int len = strlen(str);
    str[len] = '\n';
    str[len + 1] = '\0';
}

//Validates any doubles
bool validateDouble(const char* stringInputPtr, double* outputPtr, int min) {

    //Creates a counter and sets the length
    int length = strlen(stringInputPtr);
    int counter = 0;
    bool result = true;

    //checks if everything added was a digit
    while (counter < length-1 && result) {
        if (!isdigit(stringInputPtr[counter]) && stringInputPtr[counter] != '.') {
            result = false;
            puts("Invalid input");
        }
        counter++;
    }

    //if everything is a digit it converts it to a double and checks if its in the correct range
    if (result) {
        double testAmount = strtod(stringInputPtr, NULL);
        //Checks if the double is too low
        if (testAmount <= min) {
            result = false;
            puts("Amount to low");
        }
        else {
            *outputPtr = strtod(stringInputPtr, NULL);
        }
    }
    //Returns if the input is a valid double
    return result;
}//End of validate double

//Create url function
void createUrl(char* url, const char* prefix, const char* suffix, const char* orgName) {
    
    //Creates a tempString
    char tempString[LENGTH];
    
    
    //Adds the name into a temp array
    strcpy(tempString, orgName);

    tempString[strlen(orgName)] = '\0';
    int length = strlen(tempString);
    //Replaces all spaces with -
    for (int i = 0; i < length; i++) {
        if (tempString[i] == ' ') {
            tempString[i] = '-';
        }
    }
    // Add null terminator to the end of the tempString array
    tempString[length] = '\0';

    //Copies everying into url, creating the url
    strcpy(url, prefix);
    strcat(url, tempString);
    strcat(url, suffix);
}//End of create url method

//Creates a method to clear the input buffer (NEW FEATURE)
void clearInputBuffer() {
    unsigned int c;
    while ((c = getchar()) != '\n' && c != EOF);
}