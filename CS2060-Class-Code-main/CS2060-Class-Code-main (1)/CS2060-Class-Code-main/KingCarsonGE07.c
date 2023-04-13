#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Constants
#define LENGTH 80
const char* filename = "pets.txt";
//Creates pet structure
typedef struct pet {
    char name[LENGTH];
    int age;
    struct pet* next;
} Pet;

//Prototypes
bool strcasecmp(const char* s1, const char* s2);
void insertPet(Pet** head);
void displayPets(Pet* head);
void removePet(Pet** head);
void removeAllPets(struct pet** head);
void writePetsToFile(Pet* head);

//Start of main
int main() {

    //Initalizes head
    Pet* head = NULL;

    //Creates some variables for input
    char input[LENGTH];
    char name[LENGTH];

    //Creates a do while loop that continues until the user exits
    do {
        printf("Choose an option:\n");
        printf("(1) Add a pet\n");
        printf("(2) Display pets\n");
        printf("(3) Write pets to file\n");
        printf("(4) Remove a pet\n");
        printf("(5) Remove all pets\n");
        printf("(6) Quit\n");
        printf("Enter a number: ");
        fgets(input, sizeof(input), stdin);

        switch (atoi(input)) {
        case 1:
            insertPet(&head);
            break;
        case 2:
            displayPets(head);
            break;
        case 3:
            writePetsToFile(head);
            break;
        case 4:
            removePet(&head);
            break;
        case 5:
            removeAllPets(&head);
            printf("All pets removed.\n");
            break;
        case 6:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid input. Please enter a number from 1 to 6.\n");
            break;
        }
    } while (atoi(input) != 6);

    return 0;
}//End of main

//Creates a function to compare strings
bool strcasecmp(const char* s1, const char* s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    bool isSame = false;
    int correctCounter = 0;
    if (len1 != len2) {
        isSame == false;
    }
    for (int i = 0; i < len1; i++) {
        int c1 = tolower(s1[i]);
        int c2 = tolower(s2[i]);
        if (c1 != c2) {
            isSame = false;
        }
        else {
            correctCounter++;
        }
    }

    if (correctCounter == len1 & correctCounter == len2) {
        isSame = true;
    }
    return isSame;
}

//Insert pet function
void insertPet(Pet** head) {
    char name[LENGTH];
    int age;
    char tempAge[LENGTH];
    char answer;

    do {
        //Ask user to enter name and age of pet
        printf("Enter name of pet: ");
        fgets(name, LENGTH, stdin);

        //Remove trailing newline
        name[strcspn(name, "\n")] = '\0';

        printf("Enter age of pet: ");
        fgets(tempAge, LENGTH, stdin);
        age = strtod(tempAge, NULL);

        //Allocate memory for pet
        Pet* newPet = (Pet*)malloc(sizeof(Pet));
        strcpy(newPet->name, name);
        newPet->age = age;
        newPet->next = NULL;

        //Insert pet into linked list in alphabetical order
        if (*head == NULL || strcmp(name, (*head)->name) < 0) {
            newPet->next = *head;
            *head = newPet;
        }
        else {
            Pet* curr = *head;
            while (curr->next != NULL && strcmp(name, curr->next->name) > 0) {
                curr = curr->next;
            }
            if (curr->next == NULL || strcmp(name, curr->next->name) < 0) {
                newPet->next = curr->next;
                curr->next = newPet;
            }
            else {
                free(newPet);
            }
        }

        //Ask if user wants to add another pet
        printf("Add another pet? (y/n): ");
        scanf(" %c", &answer);
        getchar();
    } while (tolower(answer) == 'y');
}//End of insert pet function

//Function to display list of pets
void displayPets(Pet* head) {
    
    //Check if there are any pets in the list
    if (head == NULL) {
        printf("There aren't any pets in the list.\n");
    }
    else {
        
        //Go through the linked list and print each pet's name and age
        printf("List of pets:\n");
        while (head != NULL) {
            printf("%s: %d years old\n", head->name, head->age);
            head = head->next;
        }
    }

}//End of display pets

// Function to write names and ages of pets to a file
void writePetsToFile(Pet* head) {
    
    //Check if there are any pets in the list
    if (head == NULL) {
        printf("There aren't any pets in the list.\n");
    }
    else {
        
        //Open file for writing
        FILE* file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error opening file.\n");
        }
        else {
            
            //Go through the linked list and write each pet's name and age to the file
            while (head != NULL) {
                fprintf(file, "%s: %d years old\n", head->name, head->age);
                head = head->next;
            }
            //Close file
            fclose(file);

            printf("Names and ages of pets written to file '%s'.\n", filename);
        }//End of correct if
    }//End of failure to write if
}//End of write pets to file

//Function to remove pet
void removePet(Pet** head) {
    char answer[LENGTH] = "y";
    int counter = 0;
    int endCounter = 0;

    while (tolower(answer[0]) == 'y' && *head != NULL && endCounter != 1) {

        //Display pets in the list
        displayPets(*head);

        //Ask if they want to remove a pet
        printf("Do you want to remove a pet? (y)es or (n)o: ");
        fgets(answer, LENGTH, stdin);

        // Remove the newline character from the answer
        if (answer[strlen(answer) - 1] == '\n') {
            answer[strlen(answer) - 1] = '\0';
        }

        // Remove the newline character from the answer
        if (answer[strlen(answer) - 1] == '\n') {
            answer[strlen(answer) - 1] = '\0';
        }

        if (tolower(answer[0]) == 'y') {
            //Ask for the name to be deleted
            char name[LENGTH];
            printf("Enter the name of the pet to be removed: ");
            fgets(name, LENGTH, stdin);

            // Remove the newline character from the name
            if (name[strlen(name) - 1] == '\n') {
                name[strlen(name) - 1] = '\0';
            }

            //Go through the linked list to find the pet
            Pet* curr = *head;
            Pet* prev = NULL;

            while (curr != NULL && counter != 1) {

                //Delete if matches name in the list
                if (strcasecmp(curr->name, name) == true) {

                    //Update head if first node is being deleted
                    if (prev == NULL) {
                        *head = curr->next;
                    }
                    else {
                        prev->next = curr->next;
                    }

                    //Deallocate memory
                    free(curr);

                    printf("Pet '%s' removed from the list.\n", name);
                    counter = 1;
                }

                //Move to the next node
                prev = curr;
                curr = curr->next;
            }

            //Report if name not in the list
            if (curr == NULL) {
                printf("Pet '%s' not found in the list.\n", name);
            }

            //Display pets in the list after removal
            if (*head != NULL) {
                displayPets(*head);
            }
            else {
                printf("There are no more pets in the list.\n");
                endCounter = 1;
            }//End of display after deletion
        }//End of if yes
    }//End of infinite ask to remove while
}//End of removePet method

//Remove all pets function
void removeAllPets(struct pet** head) {
    struct pet* current = *head;
    struct pet* next;

    //Iterates through linked list till its all gone
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}//End of remove all pets function





