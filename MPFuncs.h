#define MAX_COURSES 10
#define MAX_FACULTYCOURSES 5
#define MAX_COURSESOFFERED 50
#define MAX_ROOMS 10
#define MAX_STUDENTS 20
#define MAX_FACULTY 20

typedef struct
{
	char code[10];
	char section[5];
	int units;
	char day[3];
	char time[10];
	char room[10];
	char faculty[50];
} Course;

typedef struct
{
	char id[10];
    char name[50];
    char program[10];
    int eafNumber;
    int totalUnits;
    Course completedCourses[MAX_COURSES]; 
    int completedCount;
    Course enrolledCourses[MAX_COURSES]; 
    int enrolledCount;	
} Student;

typedef struct {
    char id[10];
    char name[50];
    char department[20];
    int deloadingUnits;
    Course teachingLoad[MAX_FACULTY_COURSES];
    int courseCount;
} Faculty;

typedef struct {
    char code[10];
    char section[5];
    int units;
    char day[3]; 
    char time[10]; 
    char room[10];
    char faculty[50];
    char prerequisite[10];
} OfferedCourse;

typedef struct {
    char roomNumber[10];
    char day[3];  
    char time[10];
    char assignedCourse[10];
    char section[5];
    int occupancy;
} Room;

typedef struct {
    Student students[20];
    int studentCount;

    Faculty faculties[20];
    int facultyCount;

    OfferedCourse courses[MAX_COURSES_OFFERED];
    int courseCount;

    Room rooms[MAX_ROOMS];
    int roomCount;
} EnrollmentSystem;

void MainMenu();
void LoadData(EnrollmentSystem *system);
void SaveData(EnrollmentSystem *system);
// if commented, not yet done
// student functions
void studentMenu(EnrollmentSystem *system);
int studentLogin(EnrollmentSystem *system, char studentID[]);
void enrollCourses(EnrollmentSystem *system, Student *student);
void viewAvailableCourses(EnrollmentSystem system);
void editEnrolledCourses(EnrollmentSystem *system, Student *student);
void deleteEnrolledCourse(EnrollmentSystem *system, Student *student);
void printEAF(EnrollmentSystem *system, Student *student);
void viewAvailableCourses(EnrollmentSystem *system);

// faculty functions
void facultyMenu(EnrollmentSystem *system);
void facultyLogin(EnrollmentSystem *system);
//void enterFacultyDeload(EnrollmentSystem *system, Faculty *faculty);
//void selectCourseLoad(EnrollmentSystem *system, Faculty *faculty);
//void deleteFacultyLoad(EnrollmentSystem *system, Faculty *faculty);
//void printFacultyLoad(EnrollmentSystem *system, Faculty *faculty);
//void printStudentsPerSubject(EnrollmentSystem *system, Faculty *faculty);

// academic assistant functions
//void academicAssistantMenu(EnrollmentSystem *system);
//void enterStudentInfo(EnrollmentSystem *system);
//void enterFacultyInfo(EnrollmentSystem *system);
//void enterCoursesAndPrerequisites(EnrollmentSystem *system);
//void assignScheduleAndRooms(EnrollmentSystem *system);
//void editStudentInfo(EnrollmentSystem *system);
//void editFacultyInfo(EnrollmentSystem *system);
//void editCourseInfo(EnrollmentSystem *system);
//void displayFacultyList(EnrollmentSystem *system);

/* MainMenu displays the main menu
   Pre-condition: N/A
*/
void MainMenu()
{
    printf("\n========== Enrollment System ==========\n");
    printf("1. Student\n");
    printf("2. Faculty\n");
    printf("3. Academic Assistant\n");
    printf("4. Exit\n");
    printf("=======================================\n");
    printf("Enter your choice: ");
}

/* studentMenu displays the menu for students and processes the different options
   @param system - pointer to the EnrollmentSystem struct
   @return void
   Pre-condition: system must be initialized and contain valid student records
                  The student must provide a valid ID to access the menu
*/
void studentMenu(EnrollmentSystem *system)
{
	char studentID[10]; //initializes studentID for input
    int choice, index = -1; //initializes choice for input, initializes index to be -1 (because index can be 0)
	int flag = 1; // initializes a flag to skip while loop if needed
	
    index = studentLogin(system, studentID);//gets index of idnumber if it exists
    if (index == -1) flag = 0;  // exits if student is not found, sets flag to 0 so it doesn't go through while loop

    while (flag) {
        printf("\n========== Student Menu ==========\n"); //shows student menu
        printf("1. Enroll in Courses\n");
        printf("2. Edit Enrolled Courses\n");
        printf("3. Delete an Enrolled Course\n");
        printf("4. Print EAF (Enrollment Assessment Form)\n");
        printf("5. View Available Courses\n");
        printf("6. Exit to Main Menu\n");
        printf("===================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) { // goes through choices in the student menu
            case 1:
                enrollCourses(system, &system->students[index]);
                break;
            case 2:
                editEnrolledCourses(system, &system->students[index]);
                break;
            case 3:
                deleteEnrolledCourse(system, &system->students[index]);
                break;
            case 4:
                printEAF(system, &system->students[index]);
                break;
            case 5:
                viewAvailableCourses(system);
                break;
            case 6:
                flag = 0;  // sets flag to 0 to end loop and go back to main menu
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

/* studentLogin verifies if the given student ID exists in the system
   @param system - pointer to the EnrollmentSystem struct
   @param studentID - array to store the entered student ID
   @return index of the student if found, otherwise -1
   Pre-condition: system->students contains valid student records
*/
int studentLogin(EnrollmentSystem *system, char studentID[]) {
    
    for (int i = 0; i < system->studentCount; i++) { //iterates through system to find id number
        if (strcmp(system->students[i].id, studentID) == 0) {
            return i; //if id number exists, return its index in the system
        }
    }

    printf("Error: Student ID not found.\n");
    return -1;
}

/* enrollCourses allows a student to enroll in available courses
   @param system - pointer to the EnrollmentSystem struct
   @param student - pointer to the Student struct
   @return void
   Pre-condition: Student must be valid and have available enrollment slots
*/
void enrollCourses(EnrollmentSystem *system, Student *student) 
{
    int i, j, found, prereqMet; //initializes variables to be used
    char courseCode[10], section[5]; // initializes variables to be inputted
    int flag = 1; // initializes flag so it ggoes through while loop
	int hasEnrolled = (student.enrolledCount > 0); //checks if student has enrolled courses
	int failedPreReq = 0; //tracks how many courses failed the prereq check
	Course notAllowedCourses[MAX_COURSES]; //stores the courses the student cant enroll in

	if (!hasEnrolled) 
	{
	    printf("\nYou have no enrolled courses yet.\n");
	}

    while (flag) {
        printf("\n========== Available Courses ==========\n");
        viewAvailableCourses(*system); //shows available courses

        if (student->enrolledCount >= MAX_COURSES)  //if student has max enrolled courses
        {
            printf("Error: You have reached the maximum enrollment limit.\n");
            flag = 0;
        }

        printf("\nEnter course code and section to enroll (e.g., CCPROG1 S11). Type EXIT to stop: ");
        scanf("%s %s", courseCode, section);

        if (strcmp(courseCode, "EXIT") == 0) 
		{
            flag = 0;
        } 
		else 
		{
            found = 0;
            prereqMet = 1; // initializes prereqmet to be true for now

            for (i = 0; i < system->courseCount; i++) 
			{
                if (strcmp(system->courses[i].code, courseCode) == 0 && strcmp(system-->courses[i].section, section) == 0) 
				{
                    found = 1;
                    prereqMet = (strlen(system->courses[i].prerequisite) == 0); //checks if the course has no prerequisite

                    for (j = 0; j < student->completedCount; j++) 
					{
                        if (strcmp(student->completedCourses[j].code, system->courses[i].prerequisite) == 0) 
						{
                            prereqMet = 1;
                        }
                    }

                    if (!prereqMet) //if prereq isnt met, add to denied courses
					{
                        deniedCourses[deniedCount++] = system->courses[i];
                    } 
					else //else, enroll student in the course
					{ 
                        student.enrolledCourses[student.enrolledCount++] = system.courses[i];
                        printf("Successfully enrolled in %s %s\n", courseCode, section);
                    }
                }
            }

            if (!found) 
			{
                printf("Error: Course not found or incorrect section.\n");
            }
        }
    }

    if (deniedCount > 0) //displays denied courses
	{ 
        printf("\nBelow are the courses you are not allowed to enroll in:\n");
        for (i = 0; i < deniedCount; i++) {
            printf("%s %s - Prerequisite %s not met\n",
                   deniedCourses[i].code,
                   deniedCourses[i].section,
                   deniedCourses[i].prerequisite);
        }
    }
}

/* viewAvailableCourses displays all available courses for the current term
   @param system - pointer to the EnrollmentSystem struct
   @return void
   Pre-condition: system must be initialized with valid course records
   Post-condition: Available courses are printed in a structured format
*/
void viewAvailableCourses(EnrollmentSystem system) 
{
    int i;

    printf("\n========== Available Courses ==========\n");//formatting
    printf("%-10s %-8s %-5s %-5s %-10s %-8s %-20s\n",
           "Course", "Section", "Units", "Day", "Time", "Room", "Faculty");
    printf("---------------------------------------------------------------\n");

    for (i = 0; i < system.courseCount; i++) //iterates through the courses
	{ 
        printf("%-10s %-8s %-5d %-5s %-10s %-8s %-20s\n", //prints details
               system.courses[i].code,
               system.courses[i].section,
               system.courses[i].units,
               system.courses[i].day,
               system.courses[i].time,
               system.courses[i].room,
               system.courses[i].faculty);
    }

    if (system.courseCount == 0) // if no courses are available
	{
        printf("No available courses at the moment.\n"); 
    }
}

/* editEnrolledCourses allows a student to modify an enrolled course's section
   @param system - pointer to the EnrollmentSystem struct
   @param student - pointer to the Student struct
   @return void
   Pre-condition: Student must have at least one enrolled course
   Post-condition: The selected course's section is updated if valid
*/
void editEnrolledCourses(EnrollmentSystem *system, Student *student) {
    int i, j, found; //initializes variables to be used
    char courseCode[10], newSection[5]; //initializes coursecode and newsection to be inputted
    int flag = 1; // controls the loop

    if (student->enrolledCount == 0) //if no enrolled courses
	{
        printf("\nYou have no enrolled courses to edit.\n");
        flag = 0;
    }

    while (flag) 
	{
        printf("\n========== Your Enrolled Courses ==========\n");
        for (i = 0; i < student->enrolledCount; i++) //displays enrolled courses
		{
            printf("%s %s\n", student->enrolledCourses[i].code, student->enrolledCourses[i].section);
        }

        printf("\nEnter the course code you want to change (or type EXIT): ");
        scanf("%s", courseCode); //asks for course code to change

        if (strcmp(courseCode, "EXIT") == 0) 
		{
            flag = 0;
        } 
		else 
		{
            found = -1;
            for (i = 0; i < student->enrolledCount; i++) //looks for the course in enrolledCourses
			{
                if (strcmp(student->enrolledCourses[i].code, courseCode) == 0) 
				{
                    found = i;
                }
            }

            if (found == -1) //if not found
			{
                printf("Error: Course not found in your enrollment.\n");
            } 
			else 
			{
                printf("Enter new section: "); //asks for new section
                scanf("%s", newSection);

                int validSection = 0;
                for (j = 0; j < system->courseCount; j++) 
				{
                    if (strcmp(system->courses[j].code, courseCode) == 0 && strcmp(system->courses[j].section, newSection) == 0) 
					{
                        validSection = 1; //checks if course code and new section are the same and valid
                        strcpy(student->enrolledCourses[found].section, newSection); //replaces the section at index found with newsection
                        printf("Course section updated successfully.\n");
                    }
                }

                if (!validSection) 
				{
                    printf("Error: New section not found.\n");
                }
            }
        }
    }
}

/* deleteEnrolledCourse removes a selected course from a student's enrollment
   @param system - pointer to the EnrollmentSystem struct
   @param student - pointer to the Student struct
   @return void
   Pre-condition: Student must have at least one enrolled course
*/
void deleteEnrolledCourse(EnrollmentSystem *system, Student *student) {
    int i, j, found; //initializes variables to be used
    char courseCode[10]; //initializes courseCode string to be filled
    int flag = 1; //initializes flag to be 1

    if (student->enrolledCount == 0) //checks if student has no enrolled courses
	{
        printf("\nYou have no enrolled courses to delete.\n");
        flag = 0; //skips while loop if nothing to delete
    }

    while (flag) 
	{
        printf("\n========== Your Enrolled Courses ==========\n"); //shows enrolled courses
        for (i = 0; i < student->enrolledCount; i++) 
		{
            printf("%s %s\n", student->enrolledCourses[i].code, student->enrolledCourses[i].section);
        }

        printf("\nEnter the course code you want to drop (or type EXIT): ");
        scanf("%s", courseCode); //asks for courseCode to delete

        if (strcmp(courseCode, "EXIT") == 0) 
		{
            flag = 0;
        } 
		else 
		{
            found = -1; //makes found -1 to because of 0 indexing

            for (i = 0; i < student->enrolledCount; i++) //loops through the courses
			{
                if (strcmp(student->enrolledCourses[i].code, courseCode) == 0) 
				{
                    found = i; //if found, set found equal to index of course code to be deleted
                }
            }

            if (found == -1) {
                printf("Error: %s not part of enrolled courses.\n", courseCode);
            } 
			else 
			{
                for (j = found; j < student->enrolledCount - 1; j++) //removes the course 
				{	//replaces the course with the next course starting at the deleted course's index
                    student->enrolledCourses[j] = student->enrolledCourses[j + 1];
                }
                student->enrolledCount--;
                printf("%s has successfully been dropped.\n", courseCode);
            }
        }
    }
}

/* printEAF displays the student's Enrollment Assessment Form (EAF)
   @param system - pointer to the EnrollmentSystem struct
   @param student - pointer to the Student struct
   @return void
   Pre-condition: Student must be valid and have existing records

*/
void printEAF(EnrollmentSystem *system, Student *student) { //uses student so it doesnt have to look if its a
    int i, totalUnits = 0;

    if (student->enrolledCount == 0) {
        printf("\nEnrollment Assessment Form\n");
        printf("Student Name: %s\n", student->name);
        printf("Student ID: %s\n", student->id);
        printf("No enrolled courses found.\n");
        return;
    }

    printf("\n============================================\n");
    printf("           Enrollment Assessment Form        \n");
    printf("============================================\n");
    printf("Student Name: %s\n", student->name);
    printf("Student ID: %s\n\n", student->id);
    printf("%-10s %-8s %-5s %-5s %-10s %-8s\n", "Course", "Section", "Units", "Day", "Time", "Room");
    printf("---------------------------------------------------------------\n");

    // Print enrolled courses
    for (i = 0; i < student->enrolledCount; i++) {
        printf("%-10s %-8s %-5d %-5s %-10s %-8s\n",
               student->enrolledCourses[i].code,
               student->enrolledCourses[i].section,
               student->enrolledCourses[i].units,
               student->enrolledCourses[i].day,
               student->enrolledCourses[i].time,
               student->enrolledCourses[i].room);

        totalUnits += student->enrolledCourses[i].units;
    }

    printf("---------------------------------------------------------------\n");
    printf("Total Units Enrolled: %d\n", totalUnits);
}

/* facultyMenu provides an interface for faculty members to manage their course load
   @param system - pointer to the EnrollmentSystem struct
   @return void
   Pre-condition: system must be initialized with valid faculty records
*/
void facultyMenu(EnrollmentSystem *system) 
{
    char facultyID[10]; //initializes facultyID to be inputted
    int index, choice, flag = 1;//index is index of faculty member with the faculty ID

    index = facultyLogin(system, facultyID); //finds index of faculty member w faculty ID, otherwise -1
    if (index == -1) flag = 0;//if not found dont go through the loop

    while (flag) 
	{
        printf("\n========== Faculty Menu ==========\n");
        printf("1. View Teaching Load\n");
        printf("2. Edit Teaching Load\n");
        printf("3. Remove a Course from Teaching Load\n");
        printf("4. Print Teaching Load\n");
        printf("5. View Students Enrolled in a Course\n");
        printf("6. Exit to Main Menu\n");
        printf("===================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) //functions not yet done
		{
            case 1:
                enterFacultyDeload(system, &system->faculties[index]);
                break;
            case 2:
                selectCourseLoad(system, &system->faculties[index]);
                break;
            case 3:
                deleteFacultyLoad(system, &system->faculties[index]);
                break;
            case 4:
                printFacultyLoad(system, &system->faculties[index]);
                break;
            case 5:
                printStudentsPerSubject(system, &system->faculties[index]);
                break;
            case 6:
                flag = 0;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

/* facultyLogin check if the given faculty ID exists in the system
   @param system - pointer to the EnrollmentSystem struct
   @param facultyID - array to store the entered faculty ID
   @return index of the faculty if found, otherwise -1
   Pre-condition: system->faculties must contain valid faculty records
*/
int facultyLogin(EnrollmentSystem *system, char facultyID[]) 

{
    int i, index = -1;

    printf("\nEnter Faculty ID: ");
    scanf("%s", facultyID);

    for (i = 0; i < system->facultyCount && index == -1; i++) 
	{
        if (strcmp(system->faculties[i].id, facultyID) == 0) 
		{
            index = i;
        }
    }

    if (index == -1) 
	{
        printf("Error: Faculty ID not found.\n");
    }

    return index;
}

/* selectCourseLoad allows a faculty member to assign courses to their teaching load
   @param system - pointer to the EnrollmentSystem struct
   @param faculty - pointer to the Faculty struct
   @return void
   Pre-condition: Faculty must exist in the system
*/
void selectCourseLoad(EnrollmentSystem *system, Faculty *faculty) {
    int i, found;
    char courseCode[10], section[5];
    int totalUnits = 0; // tracks assigned units

    for (i = 0; i < faculty->courseCount; i++) //calculates total units
	{ 
        totalUnits += faculty->teachingLoad[i].units;
    }

    printf("\nHere are the available courses:\n\n");
    printf("%-10s %-8s %-5s %-5s %-10s %-8s\n", "Course", "Section", "Units", "Day", "Time", "Room");
    printf("---------------------------------------------------------------\n");

    for (i = 0; i < system->courseCount; i++) 
	{
        printf("%-10s %-8s %-5d %-5s %-10s %-8s\n",
               system->courses[i].code,
               system->courses[i].section,
               system->courses[i].units,
               system->courses[i].day,
               system->courses[i].time,
               system->courses[i].room);
    }

    printf("\nEnter your preferred courses and section (Type EXIT to stop the entry)\n");

	int flag = 1;
	
    while (flag) 
	{ 
        scanf("%s", courseCode);
        if (strcmp(courseCode, "EXIT") == 0) 
		{
            flag = 0;
        }
        scanf("%s", section);

        found = 0;
        for (i = 0; i < system->courseCount; i++) { //finds if there is an existing/valid course and section
            if (strcmp(system->courses[i].code, courseCode) == 0 && strcmp(system->courses[i].section, section) == 0) 
				{
                found = 1;

                int duplicate = 0;
                for (int j = 0; j < faculty->courseCount; j++) //checks if they already have the course and section
                {
                    if (strcmp(faculty->teachingLoad[j].code, courseCode) == 0 && strcmp(faculty->teachingLoad[j].section, section) == 0) 
					{
                        duplicate = 1;
                    }
                }

                if (duplicate) 
				{
                    printf("Error: You are already assigned to %s %s.\n", courseCode, section);
                    flag = 0;
                }

                if (totalUnits + system->courses[i].units > faculty->deloadingUnits)//checks if it more than their 
                {
                    printf("Error: Cannot add %s %s (exceeds max load of %d units).\n",
                           courseCode, section, faculty->maxLoad);
                } 
				else 
				{ //if not, add to teaching load courses
                    faculty->teachingLoad[faculty->courseCount++] = system->courses[i];
                    totalUnits += system->courses[i].units;
                    printf("Successfully assigned %s %s.\n", courseCode, section);
                }
            }
        }

        if (!found) { //if course or incorrect section isnt in the system
            printf("Error: Course not found or incorrect section.\n");
        }
    }
}
