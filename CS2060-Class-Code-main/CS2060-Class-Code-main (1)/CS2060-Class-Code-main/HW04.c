#include <stdio.h>
#include <ctype.h>

//Defines the constants
#define STUDENTS 4
#define GRADE_CATEGORIES 5
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final "
const double GRADE_CATEGORY_WEIGHT[] = { 0.1, 0.3, 0.3, 0.15, 0.15 };

//Function Prototypes
void enterGrades(int grades[][GRADE_CATEGORIES]);
void displayStudentGrades(int grades[][GRADE_CATEGORIES]);
void displayCategoryWeights(void);
double calculateFinalGrade(int studentGrades[]);
char calculateLetterGrade(double finalGrade);
double calculateClassAverage(double finalGrades[]);

//Start of main
int main() {

    //Creates variables
    double finalGrades[STUDENTS];
    char letterGrades[STUDENTS];
    double classAverage;

    //Displays the category weights
    displayCategoryWeights();

    //Creates a variable to store the grades
    int studentGrades[STUDENTS][GRADE_CATEGORIES];

    //Gets the student grades
    enterGrades(studentGrades);

    //Displays the grades
    displayStudentGrades(studentGrades);

    //Calculates the final grades and their letter
    for (int i = 0; i < STUDENTS; i++) {
        finalGrades[i] = calculateFinalGrade(studentGrades[i]);
        letterGrades[i] = calculateLetterGrade(finalGrades[i]);
    }//End of for loop

    //display final grades and letter grades
    printf("\nFinal grades for students, respectively:\n");
    for (int i = 0; i < STUDENTS; i++) {
        printf("Student %d: %8.1f %c\n", i + 1, finalGrades[i], letterGrades[i]);
    }//End of for loop

    //Calculates and displays the average
    classAverage = calculateClassAverage(finalGrades);
    printf("\nClass average is %.1f\n", classAverage);
}//End of main

//Enter grades method
void enterGrades(int grades[][GRADE_CATEGORIES]) {
    
    //Creates some temp values
    unsigned studentNum = 1;
    unsigned int check = 1;
    int grade = 0;


    
    //This loop goes through per student
    for (int j = 0; j < STUDENTS; j++) {

        printf("Enter the grade for each category for student %d, %s\n", studentNum, CATEGORIES);

        //This loop goes until all catagories for one student is done
        for (int i = 0; i < GRADE_CATEGORIES; i++) {
            check = 1;

            //Checks valid input
            while (check != 0) {
                printf("Category %d: ", i + 1);
                char input[10];
                fgets(input, 10, stdin);

                // Check if input is a valid integer
                if (sscanf(input, "%d", &grade) != 1) {
                    printf("Invalid input. Please enter a number between 0 and 105.\n");
                    continue;
                }//End of if

                // Check if input is within valid range
                if (grade < 0 || grade > 105) {
                    printf("Invalid input. Please enter a number between 0 and 105.\n");
                    continue;
                }//End of if

                // If input is valid, break out of loop and save grade
                check = 0;

            }//End of while
            //Stores the grades
            grades[j][i] = grade;
            
        }//End of if
        studentNum++;
    }//End of for loop
}//End of method

//Displays there grades
void displayStudentGrades(int grades[][GRADE_CATEGORIES])
{
    printf("\nGrades entered for each student\n");
    for (int i = 0; i < STUDENTS; i++) {
        printf("Student %d :", i + 1);
        for (int j = 0; j < GRADE_CATEGORIES; j++) {
            printf("%8d", grades[i][j]);
        }
        printf("\n");
    }
}//End of method

//Displays the category weights
void displayCategoryWeights(void) {
    printf("This program will calculate the grades for these categories %s\n", CATEGORIES);
    printf("The category weights are\n");
    for (int i = 0; i < GRADE_CATEGORIES; i++) {
        printf("Category %d weight is %.2f\n", i + 1, GRADE_CATEGORY_WEIGHT[i]);
    }
}//End of method

//Calculates the final grade
double calculateFinalGrade(int studentGrades[]) {
    double finalGrade = 0;
    for (int i = 0; i < GRADE_CATEGORIES; i++) {
        finalGrade += studentGrades[i] * GRADE_CATEGORY_WEIGHT[i];
    }
    return finalGrade;
}//end of method

//Calculates the letter grade
char calculateLetterGrade(double finalGrade) {
    if (finalGrade >= 90) {
        return 'A';
    }
    else if (finalGrade >= 80) {
        return 'B';
    }
    else if (finalGrade >= 70) {
        return 'C';
    }
    else if (finalGrade >= 60) {
        return 'D';
    }
    else {
        return 'F';
    }
}//End of method

//Calculates the class average
double calculateClassAverage(double finalGrades[]) {
    
    //Creates temp variables
    double sum = 0;
    double average = 0;

    for (int i = 0; i < STUDENTS; i++) {
        sum += finalGrades[i];
    }
    average = sum / STUDENTS;
    return average;
}//End of method