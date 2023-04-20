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

//For all chars
#define LENGTH 80

//For minimum amount a goal/donation can be
#define MIN 0.0

//For password requirements
#define CAPSMIN 1
#define LOWERMIN 1
#define NUMBERMIN 1
#define PASSWORDLENGTH 7

//For creditcard fee
#define PROCESSINGFEE 0.031

//For maximum admin attemots
#define PASSLIMIT 3
#define EMAILLIMIT 3

//char constants
const char* orgUrlPrefix = "https:donate.com/";
const char* orgUrlSuffix = "?form=popup#";
const char* folderPath = "C:\\fundraiser\\";
const char* fileSuffix = "-receipt.txt";
const char* finalFolderName = "orgs.txt";

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

//Define the Node struct for the linked list
typedef struct Node {
    Organization org;
    struct Node* next;
} Node;

//Prototypes
void setUpOrganization(Node** head);
void displayOrganizationCreation(Organization org);
bool emailValid(char email[]);
bool passwordIsValid(char password[]);
void organizationIntro(Organization* org);
bool getDonation(Organization* org);
void finalSummary(Node* head, const char* folderPath, const char* finalFolderName);
bool validateZipCode(char zipCode[]);
void displayReceipt(char orgName[], double donation);
void addEOL(char* str);
bool validateDouble(const char*stringInputPtr, double *outputPtr, int min);
void createUrl(char* url, const char* prefix, const char* suffix, const char* orgName);
void clearInputBuffer();
void addNode(Node** head, Organization org);
Organization* chooseOrganization(Node* head);
void generate_receipt(Organization org, const char* folderPath, const char* fileSuffix);
void replaceSpaces(char* str);
void deallocateList(Node** head);
char* createFolderPath(const char* folderPath, const char* fileSuffix, char orgName[]);
void sortOrgs(Node** head);

int main(void)
{
    //Declares the head
    Node* head = NULL;

    //Intializes  bool variables
    bool endProgram = false;
    bool createOrgs = true;



    while (createOrgs) {
        
        //Asks the user if they want to create a organization
        puts("Do you want to create a organization? (Y)es or (N)o");
        char responce = getchar();
        clearInputBuffer();

        //Creates a new org and adds it to the linked list, than displays it
        if (responce == 'Y' || responce == 'y') {
            //Calls the setUpOrganization function
            setUpOrganization(&head);
        }

        //Ends the loop and moves onto selecting the organization
        else if (responce == 'N' || responce == 'n') {
            createOrgs = false;
        }

        //If the user enters the wrong input
        else {
            puts("Invalid input");
        }
    }

    //Sorts the organizations alphabetically
    sortOrgs(&head);

    //Creates a loop that can only break if they have admin credential
    while (!endProgram) {

        //determines which organization will be used
        Organization* org1 = chooseOrganization(head);

        //Organization Intro
        organizationIntro(org1);
        
        //Gets the donation and if the prgram will end 
        endProgram = getDonation(org1); 

    }//End of while loop
    //Displays the final summary when the program ends
    finalSummary(head, folderPath, finalFolderName);
}//End of main

//Sets up the organization and its represenitive
void setUpOrganization(Node** head) {
    
    //Creates a bool if input is incorrect
    bool isValid = false;

    //Creates an instance of the organization
    Organization org;

    //Creates temp variables
    char tempGoal[LENGTH];
    char tempEmail[LENGTH];
    char tempPassword[LENGTH];
    char url[LENGTH];
    unsigned int min = 0;
    double goal = 0.0;

    //Initalizes some struct variables
    org.donationTotal = 0.0;
    org.totalProcessingFees = 0.0;
    org.amountOfDonors = 0;
    org.donationTotal = 0.0;

    //Creates pointers
    char (*goalStringPtr)[LENGTH] = &tempGoal;
    double* goalPtr = &goal;
    char(*urlPtr)[LENGTH] = &url;

    //Gets the organization information
    puts("Enter organization name: ");
    fgets(org.organizationName, LENGTH, stdin);
    puts("Enter fundraiser purpose: ");
    fgets(org.fundraiserPurpose, LENGTH, stdin);

    //Gets the representatives info
    puts("Enter first and last name: ");
    fgets(org.fullName, LENGTH, stdin);

    //Creates a name pointer
    const char(*orgNamePtr)[LENGTH] = &org.organizationName;

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
           org.goalAmount = goal;
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
           strcpy(org.repEmail, tempEmail);
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
            strcpy(org.repPassword, tempPassword);
        }
    }

    //Creates the url
    createUrl(urlPtr, orgUrlPrefix, orgUrlSuffix, orgNamePtr);
    strcpy(org.fundrasierURL, url);

    //Adds the orginzation to the list
    addNode(head, org);
    puts("Organization Added");
    puts("");

    //reciept is sent to file
    generate_receipt(org, folderPath, fileSuffix);

    //displays organization
    displayOrganizationCreation(org);

}//End of setUpOrganization

//Validates the admin email
bool emailValid(char email[]) {

    //Creates variables to store differnt parts of the email, and bools to check if somthing is true
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

    //Creates counters for the verification checks
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
     //Checks if the email is valid 
     if (extensionCounter == 3) {
         validCounter++;
     }
     if (periodCounter == 1) {
         validCounter++;
     }
     if (atCounter == 1) {
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
}//End of meth
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


    //Displays the current organizations information
    printf("Thank you %s. The url to raise funds for %s is %s\n", org.fullName, org.organizationName, org.fundrasierURL);
    puts("");
}//End of display

//Start of intro
void organizationIntro(Organization* org) {
    
    //Creates some temp variables
    double total = org->donationTotal;
    double goal = org->goalAmount;
    double percentComplete = 0.0;
    percentComplete = (total / goal) * 100;

    //Prints the basic info
    printf("%s\n", org->fundrasierURL);
    puts("");
    puts("MAKE A DIFFERENCE BY YOUR DONATION");
    puts("");
    printf("Organization: %s\n", org->organizationName);
    printf("Purpose: %s\n", org->fundraiserPurpose);
    if (total <= 0) {
        puts("We currently have raised 0 dollars");
    }
    else {
        printf("We currently have raised %.2f dollars \n", total);
    }
    


    //Checks if goal is met
    if (org->goalAmount <= org->donationTotal) {
        puts("We have reached our goal but can still use the donations.");
    }
    else {
            printf("We are %.2f percent towards our goal of %.2f\n", percentComplete, org->goalAmount);

    }
    puts("");
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
    char orgName[LENGTH];
    strcpy(orgName, org->organizationName);

    while (!isValid){
        puts("Enter your donation");
        fgets(tempDonation, LENGTH, stdin);
        
        //Admin section
        if (tempDonation[0] == 'q' || tempDonation[0] == 'Q') {

            //Checks if there is a correct amount of email attempts
            while (emailCounter < EMAILLIMIT && passwordCounter < PASSLIMIT) {
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
                            if (passwordCounter >= PASSLIMIT) {
                                isValid = true;
                            }
                        }
                        
                    }//End of password while

                }
                else {
                    puts("incorrect email please try again");
                    emailCounter++;
                    if (emailCounter >= EMAILLIMIT) {
                        isValid = true;
                    }
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
                displayReceipt(orgName, donation);
                clearInputBuffer();
            }
        }
    }
    return endProgram;
}//End of get donation

//Prints the admin details
void finalSummary(Node* head, const char* folderPath, const char* finalFolderName) {
    
    //sets the currNode
    Node* currNode = head;

    //Creates the file path variable
    char filePath[LENGTH];
    strcpy(filePath, folderPath);
    strcat(filePath, finalFolderName);
    //Creates the file pointer
    FILE* cfPtr;

    if ((cfPtr = fopen("%s", "w", filePath)) == NULL) {
        puts("Error opening receipt file!");
    }
    else {
        //Displays every organization and add it to the file
        while (currNode != NULL) {
            fprintf(cfPtr, "Organization name: %s\n", currNode->org.organizationName);
            fprintf(cfPtr, "Total number of donations: %d\n", currNode->org.amountOfDonors);
            fprintf(cfPtr, "Total amount raised: %.2f\n", currNode->org.donationTotal);
            fprintf(cfPtr, "Total amount paid for credit card processing: %.2f\n", currNode->org.totalProcessingFees);
            puts("");
            printf("Organization name: %s\n", currNode->org.organizationName);
            printf("Total number of donations: %d\n", currNode->org.amountOfDonors);
            printf("Total amount raised: %.2f\n", currNode->org.donationTotal);
            printf("Total amount paid for credit card processing: %.2f\n", currNode->org.totalProcessingFees);
            currNode = currNode->next;
        }
    }
    //Closes the file
    fclose(cfPtr);
    deallocateList(&head);


}
//End of admin details


//Start of validateZipcode
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
     //If zip is incorrect informs the user any 5 digits will work
     else {
        puts("Invalid Zipcode, Any 5 digit number will be accepted, even 00000.");
     }

     //Returns a boolean
     return isValid;

}//End of getZip

//Displays the Receipt
void displayReceipt(char orgName[], double donation) {

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

    //creates and gets the filename
    char filePath[LENGTH];
    strcpy(filePath, createFolderPath(folderPath, fileSuffix, orgName));

    //Creates the file pointer
    FILE* cfPtr;


    while (!isValid) {
        
        //Asks if the user wants a receipt
        puts("");
        puts("Do you want a receipt, (y)es or (n)o?");
        receiptAnwser = getchar();

        //If the user wants a receipt
        if (receiptAnwser == 'y' || receiptAnwser == 'Y') {
            
            //If the file can't be opened it still  displays the receipt, just nothing is added to the file
            if ((cfPtr = fopen("%s", "a", filePath)) == NULL) {
                puts("Error opening receipt file!");
                puts("");
                fclose(cfPtr);
            }

            //If file is able to be appened, add these to the receipt
            else {
                fprintf(cfPtr, "Organization: %s\n", orgName);
                fprintf(cfPtr, "Donation Amount %.2f\n", donation);
                fprintf(cfPtr, "Donation Date: %s - %s\n", dateString, timeString);
                puts("");
                printf("Receipt generated successfully at %s\n", filePath);
                fclose(cfPtr);
            }

            //This is shown regardless of the file
            isValid = true;
            printf("Organization: %s\n", orgName);
            printf("Donation Amount %.2f\n", donation);
            printf("Donation Date: %s - %s\n", dateString, timeString);
            
        }
        
        //If they dont want a reciept, they are taken out of the loop
        else if (receiptAnwser == 'n' || receiptAnwser == 'N') {
            isValid = true;
        }
        //If they do not enter y or n
        else {
            puts("Invalid anwser please try again");
            puts("");
        }
    }//End of while loop

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
    replaceSpaces(tempString);

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

//Function that adds a structure to the list
void addNode(Node** head, Organization org) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->org = org;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    }
    else {
        Node* curr_node = *head;
        while (curr_node->next != NULL) {
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
    }
}//End of function

Organization* chooseOrganization(Node* head) {
    
    //Creates the head variables, and some int variables
    unsigned int choice = 0;
    unsigned int numOrgs = 0;
    char testInput[LENGTH];
    Node* currNode = head;
    bool isValid = false;
    
    //Gets the amount of organizations in the list
    while (currNode != NULL) {
        numOrgs++;
        currNode = currNode->next;
    }

     //Displays all the organizations names next to there corresponding number
     puts("Select the organization you would like to make a donation.");
     puts("Organization             Goal Amount          Current Donations");
     currNode = head;
     
     //Iterates through and displays each array
     while (currNode != NULL) {
         printf("%s                         %.2f             %.2f\n", currNode->org.organizationName, currNode->org.goalAmount, currNode->org.donationTotal);
         currNode = currNode->next;
     }

    //Gets which org the user wants and tests if the input is correct, and resets current node 
     while (!isValid) {
         bool isCorrect = true;
         currNode = head;
         puts("Enter the name of the organization you want to register.");
         fgets(testInput, LENGTH, stdin);

        //Compares the org with the input and iterates throught them until the correct one is foundm or none at all
         while(isCorrect) {
             if (strcmp(testInput, currNode->org.organizationName) == 0) {
                 isValid = true;
                 isCorrect = false;
             }
             
             //iterates through the linked list, if there is nothing left to iterate through than it exits the loop and is reprompted
             else {
                 currNode = currNode->next;
                 if (currNode == NULL) {
                     isCorrect = false;
                 }
             }
             
         }
         //If the org enetred does not exist
         if (!isValid) {
             puts("Organization does not exit");
         }
     }

    //Returns the organization they chose
    return &currNode->org;
}

//Generates a receipt when a organization is created
void generate_receipt(Organization org, const char* folderPath, const char* fileSuffix) {
    //Creates a file name holder and a file path
    char fileName[LENGTH];
    char filePath[LENGTH];

    //Gets the file path
    strcpy(filePath, createFolderPath(folderPath, fileSuffix, org.organizationName));

    //Creates the file pointer
    FILE* cfPtr;

    
    //Checks if the file can be opened
    if ((cfPtr = fopen("%s", "w", filePath)) == NULL) {
        puts("Error opening receipt file!");
    }
    else {

        //Adds information to the receipt
        fprintf(cfPtr, "Donation Receipt for %s\n", org.organizationName);
        fprintf(cfPtr, "Fundrasier purpose: %s\n", org.fundraiserPurpose);
        fprintf(cfPtr, "Represenative name: %s\n", org.fullName);
        fprintf(cfPtr, "Represenative email %s\n", org.repEmail);
        fprintf(cfPtr, "Represenative password: %s\n", org.repPassword);
        fprintf(cfPtr, "Fundrasier goal: %.2f\n", org.goalAmount);
        fprintf(cfPtr, "Fundrasier URL: %s\n", org.fundrasierURL);

        //Closes the file
        fclose(cfPtr);
        printf("Receipt generated successfully at %s\n", filePath);
    }

}//End of generate receipt

//repalces the spaces in a given str with - (NEW METHOD)
void replaceSpaces(char* str) {
    
    //Searches a string for a space, if its found its replaced with -
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '-';
        }
    }
}//End of replace speaces

//Deallocates all memory from lists
void deallocateList(Node** head) {
    Node* current = *head;
    Node* next;

    //goes through the linked list and frees each indivudal organization
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL; // Set head to NULL to indicate the list is empty

    //Indicates to the admin the iists are emptied
    puts("");
    puts("all organizations are emptied and memory is freed");
}//End of deallocation

//Create createFolderPath
char* createFolderPath(const char* folderPath, const char* fileSuffix, char orgName[]) {

    //Creates the base variables
    char name[LENGTH];
    char fileName[LENGTH];
    char filePath[LENGTH]; 
    strcpy(name, orgName);
    replaceSpaces(name);

    //Creates the file path
    strcpy(fileName, name);
    strcpy(filePath, folderPath);
    strcat(filePath, fileName);
    strcat(filePath, fileSuffix);
    printf("%s\n", filePath);

    //Returns the file path
    return filePath;
}//End of createFolderPath

//Sorts the orgs in alphabetical order
void sortOrgs(Node** head) {
    // Check if the list is empty
    if (*head == NULL) {
        return;
    }

    //Initialize variables
    int swapped;
    Node* ptr1 = NULL;
    Node* lptr = NULL;

    //Perform bubble sort
    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->org.organizationName, ptr1->next->org.organizationName) > 0) {
                
                // Swap the nodes
                Organization tempOrg = ptr1->org;
                ptr1->org = ptr1->next->org;
                ptr1->next->org = tempOrg;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}//End of sort ORgs

