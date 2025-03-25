/***********************************************************************************
**********************
This is to certify that this project is my own work, based on my personal efforts in studying and
applying the concepts learned. I have constructed the functions and their respective algorithms and
corresponding code by myself. The program was run, tested, and debugged by my own efforts. I
further certify that I have not copied in part or whole or otherwise plagiarized the work of other
students and/or persons.
Theodore Rodolfo III C. Garcia, DLSU ID# 12409200
************************************************************************************
*********************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "StudentFunctions.h"

int main ()
{
	System system;

    loadCourses(&system);
    loadPrerequisites(&system);
    loadStudents(&system);
    loadRooms(&system);
    loadFaculty(&system);

	int choice;
	
    while (1) {
    	clearScreen();
        MainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                studentMenu(&system);
                break;
            case 2:
                //facultyMenu();
                break;
            case 3:
                //academicAssistantMenu();
                break;
            case 4:
                printf("Saving data...\n");
                //saveData();
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } 
	return 0;
}/***********************************************************************************
**********************
This is to certify that this project is my own work, based on my personal efforts in studying and
applying the concepts learned. I have constructed the functions and their respective algorithms and
corresponding code by myself. The program was run, tested, and debugged by my own efforts. I
further certify that I have not copied in part or whole or otherwise plagiarized the work of other
students and/or persons.
Theodore Rodolfo III C. Garcia, DLSU ID# 12409200
************************************************************************************
*********************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "StudentFunctions.h"

int main ()
{
	System system;

    loadCourses(&system);
    loadPrerequisites(&system);
    loadStudents(&system);
    loadRooms(&system);
    loadFaculty(&system);

	int choice;
	
    while (1) {
    	clearScreen();
        MainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                studentMenu(&system);
                break;
            case 2:
                //facultyMenu();
                break;
            case 3:
                //academicAssistantMenu();
                break;
            case 4:
                printf("Saving data...\n");
                //saveData();
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } 
	return 0;
}
