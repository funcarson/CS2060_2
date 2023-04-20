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
#define PROCESSINGFEE 0.027

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
typedef struct {
    double donation;
    char donorName[LENGTH];
    char zipCode[LENGTH];
} Donor;

//Prototypes
void setUpOrganization(Organization* org1);
void displayOrganizationCreation(Organization org);
void emailValid(Organization* org);
void passwordAndURL(Organization* org);
void organizationIntro(Organization org);
bool getDonation(Organization* org, Donor* don);
void finalSummary(Organization org);
void getName(Donor* don);
void getZip(Donor* don, Organization* org);
void displayReceipt(Donor don, Organization org);

int main(void)
{
    //Declares the organization structure
    Organization org1;

    //Declares donor structure
    Donor don1;

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

    //Sets up the email (Needs ti be fixed)
    emailValid(&org1);

    //gets the password and URL (should be sepperated, url should have const chars pointers)
    passwordAndURL(&org1);

    //Displays the organization
    displayOrganizationCreation(org1);

    //need a whole input validation section

    //Creates a loop that can only break if they have admin credential
    while (!endProgram) {

        //Organization Intro
        organizationIntro(org1);
        
        //Gets the donation and if the prgram will end 
        endProgram = getDonation(&org1, &don1); //needs to impliment double validation function, need to fix admin mode
        
        //If its a normal donation
        if (!endProgram) {

            //Gets the name and zipcode
            getName(&don1);
            getZip(&don1, &org1);

            //Gets the receipt
            displayReceipt(don1, org1);

        }
    }//End of while loop
    finalSummary(org1);
}//End of main

//Sets up the organization and its represenitive
void setUpOrganization(Organization* org) {
    //Creates a bool if input is incorrect
    bool isValid = false;

    //Creates a temp variables
    char tempGoal[LENGTH];

    //Gets the organization information
    puts("Enter organization name: ");
    fgets(&org->organizationName, LENGTH, stdin);
    puts("Enter fundraiser purpose: ");
    fgets(&org->fundraiserPurpose, LENGTH, stdin);

    //Gets the representatives info
    puts("Enter first and last name: ");
    fgets(&org->fullName, LENGTH, stdin);
    
    //Tests if the goal is valid
    while (!isValid) {
       puts("Enter goal amount: ");
       fgets(tempGoal, LENGTH, stdin);
       double goal = strtod(tempGoal, NULL);
       org->goalAmount = goal;

       //Tests if its a digit
       if (org->goalAmount != 0.0 || (tempGoal[0] == '0' && tempGoal[1] == '\n')) {
           
           //Tests if its in range
           if (org->goalAmount >= MIN) {
               isValid = true;
           }
           else {
               puts("Goal to small please try again");
           }
       }
       else {
           puts("Invalid input. Please enter a numeric value.");
       }
    }

}//End of setUpOrganization

void emailValid(Organization* org) {
    
    //Creates temp variables
    bool isValid = false;
    char userName[LENGTH];
    char domain[LENGTH];
    char threeLetterExtension[LENGTH];
    bool isThree = false;
    bool isDomain = false;
    bool isUserName = true;
    bool isAt = false;
    bool isPeriod = false;
    bool isNo = false;
    int extensionCounter = -1;

    //Creates the Loop
    while (!isValid) {
        puts("Enter email address ");
        fgets(&org->repEmail, LENGTH, stdin);


        //Gets the length
        int len = strlen(org->repEmail);

        int atCounter = 0;
        int periodCounter = 0;
        int validCounter = 0;
        int extensionLen = 0;

        //Checks if there is more than one @ or .
        for (int i = 0; i < len; i++) {
            if (org->repEmail[i] == '@') {
                atCounter++;
            }
            else if (org->repEmail[i] == '.') {
                periodCounter++;
            }
        }//End of @ counter

        //Gets the domain, user, and three letter extension
        for (int i = 0; i < len; i++) {
            if (isUserName) {
                userName[i] = org->repEmail[i];
                if (org->repEmail[i] == '@') {
                    userName[i] = '\0';
                    isAt = true;
                    isUserName = false;
                    isDomain = true;
                }
            }
            else if (isDomain) {
                domain[i] = org->repEmail[i];
                if (domain[i] == '.') {
                    domain[i] = '\0';
                    isDomain = false;
                    isPeriod = true;
                }
            }
            else {
                threeLetterExtension[i] = org->repEmail[i];
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
        fflush(stdin);
        //If the email is valid
        isNo = false;
        while (!isNo) {
            if (validCounter == 4) {
                puts("Is this email correct? (y)es or (n)o");
                char validEmail = getchar();
                if (validEmail == 'Y' || validEmail == 'y') {
                    isValid = true;
                    isNo = true;
                }
                else if (validEmail == 'N' || validEmail == 'n') {
                    isNo = true;
                }
                else {
                    puts("Incorrect input");
                }
            }
            else {
                puts("incorrect email please try again");
                isNo = true;
                
            }
        }//End of while loop
    }//End of methods main while loop
}//End of method
void passwordAndURL(Organization* org) {
    
    //Temp variables
    bool validPassword = false;
    char tempString[LENGTH];
    int counter = 0;


    //Starts the while loop of passwords
    while(!validPassword) {
        
        //Creates temp variables
        int passNumber = 0;
        int passCaps = 0;
        int passLower = 0;
        bool passLength = false;

        //gets password
        if (counter > 0) {
            puts("Enter password ");
        }
        fgets(&org->repPassword, LENGTH, stdin);

        //gets password length
        int length = strlen(org->repPassword);
        
        //Checks for the correct password length
        if (length >= PASSWORDLENGTH) {
            passLength = true;
        }
        
        //Checks for differnt requists of the password
        for (int i = 0; i < length; i++) {

            //Checks for amount of numbers in password
            if (isdigit(org->repPassword[i])) {
                passNumber++;
            }

            //Checks for amount of capslocks in password
            else if (isupper(org->repPassword[i])) {
                passCaps++;
            }

            //Checks for amount of lower case in password
            else if (islower(org->repPassword[i])) {
                passLower++;
            }
        }

        //Checks if all password prerequsits are met if not they have to enter a new one
        if (passLength && passLower >= LOWERMIN && passCaps >= CAPSMIN && passNumber >= NUMBERMIN) {
            validPassword = true;
        }
        else {
            if (counter > 0){
                puts("Incorrect Password please enter a new one");
            }
            counter++;
            
        }
    }//End of while loop

    //Adds the name into a temp array
    strcpy(tempString, org->fundrasierURL);

    tempString[strlen(org->organizationName)] = '\0';
    int length = strlen(tempString);
    //Replaces all spaces with -
    for (int i = 0; i < length; i++) {
        if (tempString[i] == ' ') {
            tempString[i] = '-';
        }
    }
    // Add null terminator to the end of the tempString array
    tempString[length] = '\0';

    // Copy tempString to fundraiserURL
    strcpy(org->fundrasierURL, tempString);

    double total = 0.0;
    unsigned int donors = 0;
    double processing = 0.0;

    org->donationTotal = total;
    org->totalProcessingFees = processing;
    org->amountOfDonors = donors;
}//End of password and URL

//Start of display
void displayOrganizationCreation(Organization org) {
    printf("Thank you %s. The url to raise funds for %s is https:donate.com/%s?form=popup#. \n", org.fullName, org.organizationName, org.fundrasierURL);
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
    printf("https:donate.com/%s?form=popup# \n", org.fundrasierURL);
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
bool getDonation(Organization* org, Donor* don) {

    //temp variables
    char tempDonation[LENGTH];
    char tempEmail[LENGTH];
    bool endProgram = false;
    int emailCounter = 0;
    int passwordCounter = 0;
    char userEmail[LENGTH];
    char userPassword[LENGTH];
    bool isValid = false;



    while (!isValid){
        puts("Enter your donation");
        fgets(tempDonation, LENGTH, stdin);
        //Admin section
        if (tempDonation[0] == 'q' || tempDonation[0] == 'Q') {

            //Checks if there is a correct amount of email attempts
            while (emailCounter <= 2 || passwordCounter <= 2) {
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
            //Converts to double and adds to structure
            don->donation = strtod(tempDonation, NULL);

            //Tests if its a digit
            if (don->donation != 0.0 || (tempDonation[0] == '0' && tempDonation[1] == '\n')) {

                //Tests if its in range
                if (don->donation >= MIN) {
                    isValid = true;

                    //Adds to the totals
                    org->amountOfDonors++;

                    //Gets the exact time of donation
;

                }
                else {
                    puts("Donation to small please try again");
                }
            }
            else {
                puts("Invalid input. Please enter a numeric value.");
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

//Gets donor name 
void getName(Donor* don) {
    puts("Enter your First and Last Name name: ");
    fgets(&don->donorName, LENGTH, stdin);
}//End of getname

//Start of get zipcode
void getZip(Donor* don, Organization* org) {
    //Creates some temp variables
    bool isValid = false;
    bool isNumber = false;
    bool isLength = false;
    
    //Tests if the zipcode is valid
    while (!isValid) {

        //Creates a temp counter
        int numCounter = 0;

        //Gets the zipcode
        puts("Enter your 5 digit zipcode");
        fgets(&don->zipCode, LENGTH, stdin);

        //Gets the zipcode length
        int length = strlen(&don->zipCode);

        //Checks if its a valid integer and the amount of characters at the same time
        for (int i = 0; i < length; i++) {
            if (isdigit(don->zipCode[i])) {
                numCounter++;
            }
        }

        //Checks if the zipcode is correct
        if(numCounter == 5) {
            isValid = true;
            double donation = don->donation;
            double fee = donation * PROCESSINGFEE;
            double actualDonation = donation - fee;
            org->totalProcessingFees += fee;
            printf("Thank you for your donation. There is a %.3f percent credit card processing fee of %.2f. %.2f will be donated \n", PROCESSINGFEE, fee, actualDonation);
            puts("");
            don->donation = actualDonation;
            org->donationTotal += actualDonation;
        }
        else {
            puts("Invalid Zipcode, Any 5 digit number will be accepted, even 00000.");
        }
    }//End of while loop
}//End of getZip

//Displays the Receipt
void displayReceipt(Donor don, Organization org) {

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
            printf("Organization: %s\n", org.organizationName);
            printf("Donation Amount %.2f\n", don.donation);
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