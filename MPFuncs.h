#include "structsndef.h"
#include "helperFuncs.h"

void loadCourses(System *system)
{
    FILE *file = fopen("courses.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    char line[200]; //buffer variable for sscanf
    system->courseCount = 0;

    while (fgets(line, sizeof(line), file) != NULL && system->courseCount < MAX_COURSES_OFFERED) //while fgets doesnt get NULL (while not EOF)
    {
        Course *course = &system->courses[system->courseCount];

        int result = sscanf(line, "%s %s %d %s %s %s %s %s",
                            course->code,
                            course->section,
                            &course->units,
                            course->days,
                            course->time,
                            course->room,
                            course->facultyName.first,
                            course->facultyName.last);

        if (result == 6) //if only up to 6 formats, change facultyname
        {
            strcpy(course->facultyName.first, "");
            strcpy(course->facultyName.last, "");
        }

        system->courseCount++;
    }

    fclose(file);
}

void loadRooms(System *system) {
    FILE *file = fopen("rooms.txt", "r");
    String10 roomNumber, day, time, courseCode, section; //temp variables
    int occupancy, i, j, k, l, roomIndex;

    if (file == NULL) {
        printf("Error opening file.\n");
    } else {
        system->roomCount = 0;//sets room count

        while (fscanf(file, "%s %s %s %s %s %d", roomNumber, day, time, courseCode, section, &occupancy) == 6) 
			{

	            roomIndex = -1;
	
	            for (i = 0; i < system->roomCount; i++) 
				{
	                if (strcmp(system->rooms[i].roomNumber, roomNumber) == 0) 
					{
	                    roomIndex = i; //room is found
	                }
	            }
	
	            if (roomIndex == -1 && system->roomCount < MAX_ROOMS) //if room doesnt exist yet, make new one
				{
	                roomIndex = system->roomCount;
	                Room *room = &system->rooms[roomIndex];
	                strcpy(room->roomNumber, roomNumber);
	                room->classCount = 0;
	                system->roomCount++;
	            }
	
	            if (roomIndex != -1) //adds class to a room
				{
	                Room *room = &system->rooms[roomIndex];
	                if (room->classCount < 7)  //only adds if there is enough space for a class
					{
	                    int slot = room->classCount;
	                    strcpy(room->days[slot], day);
	                    strcpy(room->time[slot], time);
	                    strcpy(room->courseCode[slot], courseCode);
	                    strcpy(room->section[slot], section);
	                    room->occupancy[slot] = occupancy;
	                    room->classCount++;
	                }
	            }
        	}
        fclose(file);

        //updates occupancy
        for (i = 0; i < system->studentCount; i++) 
		{
            for (j = 0; j < system->students[i].enrolledCount; j++) 
			{
                String10 enrolledCourseCode;
                String5 enrolledSection;
                strcpy(enrolledCourseCode, system->students[i].enrolledCourses[j].code);
                strcpy(enrolledSection, system->students[i].enrolledCourses[j].section);

                // Find the room and class that matches the enrolled course
                for (k = 0; k < system->roomCount; k++) 
				{
                    for (l = 0; l < system->rooms[k].classCount; l++) 
					{
                        if (strcmp(system->rooms[k].courseCode[l], enrolledCourseCode) == 0 &&strcmp(system->rooms[k].section[l], enrolledSection) == 0)
						{
                            system->rooms[k].occupancy[l]++;
                        }
                    }
                }
            }
        }
    }
}
void loadPrerequisites(System *system) {
    FILE *file = fopen("prerequisite.txt", "r");
    int flag = 1;
    int i;
    if (file == NULL) {
        printf("Error opening file");
        flag = 0;
    }
    String10 courseCode, prereqCode;
    while (flag && fscanf(file, "%s %s", courseCode, prereqCode) == 2) 
	{
        for (i = 0; i < system->courseCount; i++) 
		{
            if (strcmp(system->courses[i].code, courseCode) == 0) 
			{
                strcpy(system->courses[i].prereqCourse[system->courses[i].prereqCount], prereqCode);
                system->courses[i].prereqCount++;
            }
        }
    }

    fclose(file);
}


void loadStudents(System *system)
{
	FILE *fp = fopen("students.txt", "r");
	String10 temp1, temp2;
	int i;
    int flag;
    int found = 0;
    long position;
    int count = 0;
    int sum = 0;
    int totalsum =0;
	while (fscanf(fp, "%s", temp1) == 1) {
        if (strcmp(temp1, "STUDENT") == 0) {
            fscanf(fp, "%d", &count); 
        }
        
        fscanf(fp, "%s %s %s", 
            system->students[count-1].name.first, 
            system->students[count-1].name.last,
            system->students[count-1].id);
            
            printf("%s ",temp1);
            printf("%d\n",count);
			printf("%s ",system->students[count-1].name.first);
			printf("%s ",system->students[count-1].name.last);
			printf("%s\n",system->students[count-1].id);
			
        system->students[count-1].completedCount = 0;
        flag = 1;
        while (flag) {
            position = ftell(fp);

            if (fscanf(fp, "%s", temp1) != 1) {
                flag = 0;
                
            }

           	if (strcmp(temp1,":") == 0 && flag){
                printf("%s",temp1);
            }
            
            if (strcmp(temp1, "STUDENT") == 0 && flag) {
                fseek(fp, position, SEEK_SET);
                flag = 0;
                
            }

            if (strcmp(temp1, "STUDENT") != 0 && flag && strcmp(temp1, "EAF")!= 0 && strcmp(temp1, ":")!= 0) {
                if (system->students[count-1].completedCount < MAX_COURSES) { 
                    strcpy(system->students[count-1].subjectsTaken[system->students[count-1].completedCount], temp1);
                    printf("%s\n",system->students[count-1].subjectsTaken[system->students[count-1].completedCount]);
                    system->students[count-1].completedCount++;
                }
            }
            if (strcmp(temp1,"EAF")==0){
            	system->students[count-1].enrolledCount = 0; 
            	printf("\n%s\n",temp1);
            	while (flag){
            		position = ftell(fp);
            			if (fscanf(fp,"%s %s",temp1,temp2)!=2){
            			flag = 0;
					}
						if (strcmp(temp1,"STUDENT")==0&&flag){
				 		fseek(fp, position, SEEK_SET);
                 		flag = 0;
             		}
             			if (strcmp(temp1,"STUDENT")!= 0 && flag && IsCourseCodeStudent(temp1)){
             				if (system->students[count-1].enrolledCount<MAX_COURSES){
             					strcpy(system->students[count-1].enrolledCourses[system->students[count-1].enrolledCount].code, temp1);
             					strcpy(system->students[count-1].enrolledCourses[system->students[count-1].enrolledCount].section, temp2);
                    			printf("%s ",system->students[count-1].enrolledCourses[system->students[count-1].enrolledCount].code);
                    			printf("%s ",system->students[count-1].enrolledCourses[system->students[count-1].enrolledCount].section);
                    			system->students[count-1].enrolledCount++;
            
             					position = ftell(fp);
             					fscanf(fp, "%d", &sum);
             					printf("%d\n",sum);
             					fseek(fp,position,SEEK_SET);
             					
             					for (i = 0; i < system->courseCount;i++){
             						if (strcmp(temp1,system->courses[i].code)==0){
             							if (strcmp(temp2, system->courses[i].section)==0){
             								system->courses[i].studentCount++;
										 }
									 }
								 }
								
							 }
							 totalsum += sum;
						 }
						 
            	}	
            		system->students[count-1].totalUnits = totalsum;
            		  totalsum = 0;
			}
        }
       // printf("\n");
    }
    system->studentCount = count;
}

void loadFaculty(System *system) {
	FILE *fp = fopen("teachers.txt", "r");
    String10 temp1, temp2;
    int flag;
    long position;
    int count = 0;

    while (fscanf(fp, "%s", temp1) == 1) {
        if (strcmp(temp1, "FACULTY") == 0) {
            fscanf(fp, "%d", &count); 
        }

        fscanf(fp, "%s %s %s %s %d", 
            system->faculties[count-1].name.first, 
            system->faculties[count-1].name.last,
            system->faculties[count-1].id, 
            system->faculties[count-1].dept, 
            &system->faculties[count-1].deloadingUnits);

        system->faculties[count-1].courseCount = 0;

        flag = 1;
        while (flag) {
            position = ftell(fp);

            if (fscanf(fp, "%s %s", temp1, temp2) != 2) {
                flag = 0;
            }
          
            if (strcmp(temp1, "FACULTY") == 0 && flag) {
                fseek(fp, position, SEEK_SET);
                flag = 0;
            }
            
            if (IsCourseCodeFac(temp1) && flag) {
                if (system->faculties[count-1].courseCount < MAX_COURSES) { 
                    strcpy(system->faculties[count-1].teachingLoad[system->faculties[count-1].courseCount].code, temp1);
                    strcpy(system->faculties[count-1].teachingLoad[system->faculties[count-1].courseCount].section, temp2);
                    
                    system->faculties[count-1].courseCount++;
                }
            }
        }
    }
    system->facultyCount = count;
}

void MainMenu()
{
	clearScreen();
    printf("\n");
    printf("========================================\n");
    printf("|               MAIN MENU              |\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|      1] Student                      |\n");
    printf("|      2] Faculty                      |\n");
    printf("|      3] Academic Assistant           |\n");
    printf("|                                      |\n");
    printf("========================================\n");
    printf("\n");
}
//			STUDENT FUNCTIONS CATEGORY

/* studentLogin verifies if the given student ID exists in the system
   @param system - pointer to the EnrollmentSystem struct
   @param studentID - array to store the entered student ID
   @return index of the student if found, otherwise -1
   Pre-condition: system->students contains valid student records
*/
int studentLogin(System *system, String10 id)
{
	int i;
    for (i = 0; i < system->studentCount; i++) 
    {
        if (strcmp(system->students[i].id, id) == 0) 
        {
            return i; // Return the index if the ID matches
        }
    }
    return -1; // Return -1 if the ID is not found
}

void printOfferedCourses(System *system)
{
	printf("SUBJECT\tSection\tUnits\tDay\tTime\t\tRoom\tFaculty\n");
	printf("\n");

    int i;
    for (i = 0; i < system->courseCount; i++)
    {
        Course course = system->courses[i];

        printf("%-8s%-6s\t%d\t%s\t%-9s\t%-6s",
               course.code,
               course.section,
               course.units,
               course.days,
               course.time,
               course.room);

        if (strcmp(course.facultyName.first, "") != 0)
        {
            printf("\t%s, %s", course.facultyName.last, course.facultyName.first);
        }

        printf("\n");
    }
}

void checkValidity(System *system, int stdIdx, CourseID temp[], CourseID valid[], CourseID invalid[], String80 reason[], int count, int *validCount, int *invalidCount) {
    int i, j, k, m;
    
    //loops through all courses
    for (i = 0; i < count; i++) {
        int found = 0;
        int correctSection = 0;
        int enrolledFlag = 0;
        int takenFlag = 0;
        int metPrereq = 1;
        String30 prqTemp = ""; //initialze to avoid garbage variables
        int courseIndex = -1;

        //checks if course exists and section is correct
        for (j = 0; j < system->courseCount; j++) 
		{
            if (strcmp(temp[i].code, system->courses[j].code) == 0) 
			{
                found = 1;
                if (strcmp(temp[i].section, system->courses[j].section) == 0) 
				{
                    correctSection = 1;
                    courseIndex = j;  // Remember which course matched
                }
            }
        }
        
		//only goes through if previous check (course and section exist)
        if (found && correctSection) 
		{
			//checks if already enrolled
            for (k = 0; k < system->students[stdIdx].enrolledCount; k++) 
			{
                if (strcmp(temp[i].code, system->students[stdIdx].enrolledCourses[k].code) == 0 &&
                    strcmp(temp[i].section, system->students[stdIdx].enrolledCourses[k].section) == 0) 
					{
                    enrolledFlag = 1;
                }
            }

            //checks if course was already taken before
            for (k = 0; k < system->students[stdIdx].completedCount; k++) 
			{
                if (strcmp(temp[i].code, system->students[stdIdx].subjectsTaken[k]) == 0) 
				{
                    takenFlag = 1;
                }
            }

            //checks prerequisites
            if (courseIndex >= 0) 
			{
                int prqCount = system->courses[courseIndex].prereqCount;
                if (prqCount > 0) 
				{
                    metPrereq = 1;
                    int prqMissingCount = 0;
                    for (k = 0; k < prqCount; k++) 
					{
                        int hasPrereq = 0;
                        for (m = 0; m < system->students[stdIdx].completedCount; m++) 
						{
                            if (strcmp(system->courses[courseIndex].prereqCourse[k], system->students[stdIdx].subjectsTaken[m]) == 0) 
									  {
                                hasPrereq = 1;
                            }
                        }
                        if (!hasPrereq) 
						{
                            if (prqMissingCount > 0) 
							{
                                strcat(prqTemp, ", ");
                            }
                            strcat(prqTemp, system->courses[courseIndex].prereqCourse[k]);
                            prqMissingCount++;
                            metPrereq = 0;
                        }
                    }
                }
            }
        }

        //picks the final status of the course if invalid give reason, if not add it to valid
        if (!found) {
            strcpy(reason[*invalidCount], "Invalid class.");
            invalid[*invalidCount] = temp[i];
            (*invalidCount)++;
        } 
        else if (!correctSection) {
            strcpy(reason[*invalidCount], "Incorrect section.");
            invalid[*invalidCount] = temp[i];
            (*invalidCount)++;
        } 
        else if (enrolledFlag) {
            strcpy(reason[*invalidCount], "Already enrolled.");
            invalid[*invalidCount] = temp[i];
            (*invalidCount)++;
        } 
        else if (takenFlag) {
            strcpy(reason[*invalidCount], "Course already taken before.");
            invalid[*invalidCount] = temp[i];
            (*invalidCount)++;
        } 
		else if (!metPrereq) {
		    String80 prqString;
		    strcpy(prqString, "Missing prerequisites: ");
		    strcat(prqString, prqTemp);
		    strcpy(reason[*invalidCount], prqString);
		    invalid[*invalidCount] = temp[i];
		    (*invalidCount)++;
		}
        else {
            valid[*validCount] = temp[i];
            (*validCount)++;
        }
    }
}

void printAddResults(System *system, int stdIdx, CourseID valid[], CourseID invalid[], String80 reason[], int validCount, int invalidCount) {
    int i, j;

    printf("\nBelow are the approved enrolled courses:\n");
    for (i = 0; i < validCount; i++) {
        for (j = 0; j < system->courseCount; j++) {
            if (strcmp(system->courses[j].code, valid[i].code) == 0 &&
                strcmp(system->courses[j].section, valid[i].section) == 0) {
                
                //prints course info
                printf("%s\t%s\t%d\t%s\t%s\t%s\t",
                       system->courses[j].code,
                       system->courses[j].section,
                       system->courses[j].units,
                       system->courses[j].days,
                       system->courses[j].time,
                       system->courses[j].room);

                //prints faculty name if there is
                if (strcmp(system->courses[j].facultyName.first, "") != 0 && 
                    strcmp(system->courses[j].facultyName.last, "") != 0) {
                    printf("%s, %s", 
                           system->courses[j].facultyName.last, 
                           system->courses[j].facultyName.first);
                }
                printf("\n");
            }
        }
    }

    //print not allowed courses
    printf("\nBelow are not allowed to enroll:\n");
    for (i = 0; i < invalidCount; i++) 
	{
        printf("%s %s. %s\n", invalid[i].code, invalid[i].section, reason[i]);
    }
}

void addCourses(System *system, int stdIdx) {
    int flag = 1;

    printf("Adding Courses...\n");
    if (system->students[stdIdx].enrolledCount == 0) {
        printf("You have no enrolled courses yet\n\n");
    }

    printf("Here are the Offered Courses:\n\n");
    printOfferedCourses(system);

    printf("Enter the courses and section to enroll (Ex: CCPROG1 S11). Type EXIT once you are done with the entry: \n");
    //initialize so no garbage values
    CourseID temp[10] = {0};
    CourseID invalid[10] = {0};
    CourseID valid[10] = {0};
    String80 reason[10] = {""};
    int count = 0;

    while (flag) {
        scanf("%s", temp[count].code);
        if (strcmp(temp[count].code, "EXIT") == 0) {
            flag = 0; // Exit the loop
        } else {
            scanf("%s", temp[count].section);
            count++;
        }
    }

    int validCount = 0;
    int invalidCount = 0;
    checkValidity(system, stdIdx, temp, valid, invalid, reason, count, &validCount, &invalidCount);
    printAddResults(system, stdIdx, valid, invalid, reason, validCount, invalidCount);
    
	int i, j, r, s;
	int unitsToAdd = 0;
    for (i = 0; i < validCount && system->students[stdIdx].enrolledCount < MAX_COURSES; i++) {
        int courseFound = 0;
        
        for (j = 0; j < system->courseCount && !courseFound; j++) {
            if (strcmp(valid[i].code, system->courses[j].code) == 0 && 
                strcmp(valid[i].section, system->courses[j].section) == 0) {
                
                //adds to student's enrollment
                system->students[stdIdx].enrolledCourses[system->students[stdIdx].enrolledCount] = valid[i];
                unitsToAdd += system->courses[j].units;
                
                //increments student count for the course
                system->courses[j].studentCount++;
                

                for (r = 0; r < system->roomCount; r++) //checks rooms
				{
				    int roomSlotFound = 0;
				    for (s = 0; s < system->rooms[r].classCount && !roomSlotFound; s++) //checks class slots
					{
				        if (strcmp(system->rooms[r].courseCode[s], valid[i].code) == 0 &&
				            strcmp(system->rooms[r].section[s], valid[i].section) == 0) //finds matching class
						{
				            system->rooms[r].occupancy[s]++;
				            roomSlotFound = 1; //flag to exit inner loop
				            r = system->roomCount - 1; //will exit the loop since its the last room
				        }
				    }
				}
                courseFound = 1;
            }
        }
        
        if (courseFound) {
            system->students[stdIdx].enrolledCount++;
        }
    }
    
    system->students[stdIdx].totalUnits += unitsToAdd;
	
	pauseProgram();
}

void editCourses(System *system, int stdIdx) 
{
	//initialize to avoid garbage values
    char inputCode[10] = {0};
    char newSection[10] = {0};
    int i, j;
    int courseFound = 0;
    int validSection = 0;
    int courseIndex = -1;
    int sectionCount = 0;
    int alreadyEnrolled = 0;
    char confirm = 'N';
    int shouldEdit = 0;
    int done = 0;

    printf("\n===== EDIT ENROLLED COURSE =====\n");
    
    //check enrollment status
    if (system->students[stdIdx].enrolledCount == 0) {
        printf("You are not currently enrolled in any courses.\n");
        done = 1;
    }

    //show enrolled courses
    if (!done) {
        printf("Your currently enrolled courses:\n");
        for (i = 0; i < system->students[stdIdx].enrolledCount; i++) {
            printf("%d. %s %s\n", i+1, system->students[stdIdx].enrolledCourses[i].code, system->students[stdIdx].enrolledCourses[i].section);
        }

        //get course to edit
        printf("\nEnter course code to edit: ");
        scanf("%9s", inputCode);
        
        //find in enrolled courses
        for (i = 0; i < system->students[stdIdx].enrolledCount && !courseFound; i++) 
		{
            if (strcmp(inputCode, system->students[stdIdx].enrolledCourses[i].code) == 0) 
			{
                courseFound = 1;
                courseIndex = i;
            }
        }

        if (!courseFound) 
		{
            printf("Course not found in your enrollments.\n");
            done = 1;
        }
    }

    //shows available sections
    if (!done) 
	{
        printf("\nAvailable sections for %s:\n", inputCode);
        for (j = 0; j < system->courseCount; j++) 
		{
            if (strcmp(inputCode, system->courses[j].code) == 0) 
			{
                printf("- %s\n", system->courses[j].section);
                sectionCount++;
            }
        }

        if (sectionCount == 0) 
		{
            printf("No other sections available for this course.\n");
            done = 1;
        }
    }

    //get and check new section
    if (!done) 
	{
        printf("\nEnter new section: ");
        scanf("%9s", newSection);

        for (j = 0; j < system->courseCount && !validSection; j++) 
		{
            if (strcmp(inputCode, system->courses[j].code) == 0 && 
                strcmp(newSection, system->courses[j].section) == 0) 
			{
                validSection = 1;
            }
        }

        if (!validSection) 
		{
            printf("Invalid section. Please choose from available sections.\n");
            done = 1;
        }
    }

    //check if already enrolled
    if (!done) 
	{
        for (i = 0; i < system->students[stdIdx].enrolledCount && !alreadyEnrolled; i++) 
		{
            if (strcmp(inputCode, system->students[stdIdx].enrolledCourses[i].code) == 0 &&
                strcmp(newSection, system->students[stdIdx].enrolledCourses[i].section) == 0) 
			{
                alreadyEnrolled = 1;
            }
        }

        if (alreadyEnrolled) 
		{
            printf("You are already enrolled in this section.\n");
            done = 1;
        }
    }

	//confirmation
    if (!done) 
	{
        printf("\nYou are about to change to section %s\n", newSection);
        printf("Confirm change? (Y/N): ");
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y') 
		{
            strcpy(system->students[stdIdx].enrolledCourses[courseIndex].section, newSection);
            printf("Successfully changed to section %s!\n", newSection);
        } 
		else 
		{
            printf("Section change cancelled.\n");
        }
    }

	pauseProgram();
}

void dropCourses(System *system, int stdIdx) 
{
	//initialize garbage values
    char courseCode[10] = {0};
    int i, j;
    int courseFound = 0;
    int courseIndex = -1;
    int unitsToRemove = 0;
    int newUnitTotal = 0;
    char confirm = 'N';
    int shouldDrop = 0;
    int done = 0;

    printf("\n===== DROP COURSE =====\n");
    
    //check enrollment status
    if (system->students[stdIdx].enrolledCount == 0) 
	{
        printf("You are not currently enrolled in any courses.\n");
        done = 1;
    }

    //show enrolled courses
    if (!done) 
	{
        printf("Your enrolled courses:\n");
        for (i = 0; i < system->students[stdIdx].enrolledCount; i++) 
		{
            printf("%d. %s %s\n", i+1, system->students[stdIdx].enrolledCourses[i].code, system->students[stdIdx].enrolledCourses[i].section);
        }

        //get course to drop
        printf("\nEnter course code to drop (e.g. CCPROG3): ");
        scanf("%s", courseCode);

        //fidn course
        for (i = 0; i < system->students[stdIdx].enrolledCount && !courseFound; i++) 
		{
            if (strcmp(courseCode, system->students[stdIdx].enrolledCourses[i].code) == 0) 
			{
                courseFound = 1;
                courseIndex = i;
            }
        }

        if (!courseFound) {
            printf("Course not found in your enrollments.\n");
            done = 1;
        }
    }

    // get unit count
    if (!done) 
	{
        for (j = 0; j < system->courseCount && unitsToRemove == 0; j++) 
		{
            if (strcmp(courseCode, system->courses[j].code) == 0 )
			{
                unitsToRemove = system->courses[j].units;
            }
        }
        
        newUnitTotal = system->students[stdIdx].totalUnits - unitsToRemove;
        
        //confirmation
        printf("\nYou will only have %d units left after removing this.\n", newUnitTotal);
        printf("Are you sure you want to drop %s ? (Y/N): ", courseCode);
        scanf(" %c", &confirm);
        
        if (confirm == 'Y' || confirm == 'y') 
		{
            shouldDrop = 1;
        } 
		else 
		{
            printf("Course drop cancelled.\n");
            done = 1;
        }
    }

    if (!done && shouldDrop) 
	{
        for (i = courseIndex; i < system->students[stdIdx].enrolledCount - 1; i++) //shift from deleted course
		{
            system->students[stdIdx].enrolledCourses[i] = system->students[stdIdx].enrolledCourses[i+1];
        }

		//update counts
        system->students[stdIdx].enrolledCount--;
        system->students[stdIdx].totalUnits = newUnitTotal;
        system->courses[courseIndex].studentCount--;
        
        printf("Successfully dropped %s %s.\n", courseCode);
    }

	pauseProgram();
}

void viewCurrentEnrollment(System *system, int stdIdx) {
    int i, j, found;
    
    printf("\n===== CURRENT ENROLLMENT =====\n");
    printf("Student: %s, %s\n",
           system->students[stdIdx].name.last,
           system->students[stdIdx].name.first);
    printf("Total Units: %d\n\n", system->students[stdIdx].totalUnits);

    if (system->students[stdIdx].enrolledCount == 0) {
        printf("No currently enrolled courses.\n");
    } else {
        printf("%-10s %-8s %-6s %-10s %-12s %-10s %-20s\n", "Code", "Section", "Units", "Days", "Time", "Room", "Faculty");
        printf("---------------------------------------------------------------------------------\n");

        for (i = 0; i < system->students[stdIdx].enrolledCount; i++) 
		{
            found = 0;

            for (j = 0; j < system->courseCount && !found; j++) 
			{
                if (strcmp(system->students[stdIdx].enrolledCourses[i].code, system->courses[j].code) == 0 &&
                    strcmp(system->students[stdIdx].enrolledCourses[i].section, system->courses[j].section) == 0) 
					{
                    found = 1;

                    String30 facultyName = "";
                    if (strlen(system->courses[j].facultyName.last) > 0) 
					{
						strcat(facultyName, system->courses[j].facultyName.last);
						strcat(facultyName, ", ");
						strcat(facultyName, system->courses[j].facultyName.first);
                    } 
					else 
					{
                        strcpy(facultyName, "TBA"); //if no teacher put TBA
                    }
                    
                    printf("%-10s %-8s %-6d %-10s %-12s %-10s %-20s\n",
                           system->courses[j].code,
                           system->courses[j].section,
                           system->courses[j].units,
                           system->courses[j].days,
                           system->courses[j].time,
                           system->courses[j].room,
                           facultyName);
                }
            }
        }
    }

    pauseProgram(); // Pause before exiting
}


void enrollCourses(System *system, int stdIdx)
{
	int choice;
    int flag = 0;
    
    while (!flag) {
    	clearScreen();
	    printf("========================================\n");
	    printf("|          ENROLLMENT MENU             |\n");
	    printf("========================================\n");
	    printf("|                                      |\n");
	    printf("| Choose your action:                  |\n");
	    printf("|                                      |\n");
	    printf("| [1] Add Courses                      |\n");
	    printf("| [2] Edit Enrolled Courses            |\n");
	    printf("| [3] Drop Courses                     |\n");
	    printf("| [4] View Current Enrollment          |\n");
	    printf("| [5] Return to Student Menu           |\n");
	    printf("|                                      |\n");
	    printf("========================================\n");
	    printf("\n");
	    printf("Enter your choice (1-5): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addCourses(system, stdIdx);
                break;
            case 2: 
                editCourses(system, stdIdx);
                break;
            case 3:
                dropCourses(system, stdIdx);
                break;
            case 4:
                viewCurrentEnrollment(system, stdIdx);
                break;
            case 5:
                flag = 1;
                break;
            default:
                printf("Invalid choice! Please enter 1-5.\n");
                while (getchar() != '\n');
        }
    }
}

void viewCompletedCourses(System *system, int stdIdx) {
    int i = 0;
    
    printf("\n===== COMPLETED COURSES =====\n");
    printf("Student: %s %s (%s)\n", system->students[stdIdx].name.first, system->students[stdIdx].name.last, system->students[stdIdx].id);
    printf("Program: %s\n", system->students[stdIdx].program);    
    printf("\nCompleted Courses:\n");
    printf("-------------------\n");
    
    while (i < system->students[stdIdx].completedCount) 
	{
        printf("%s\n", system->students[stdIdx].subjectsTaken[i]);
        i++;
    }
    printf("-------------------\n");
    printf("\nTotal Units Completed: %d\n", system->students[stdIdx].completedCount);
}

void printEAF(System *system, int stdIdx) {
    
	String30 filename;
    int i = 0;
    int j = 0;
    int totalUnits = 0;
    int fileError = 0;

    sprintf(filename, "%s.txt", system->students[stdIdx].id);
	FILE *file = fopen(filename, "w");
  
    if (file != NULL) {
        fprintf(file, "ENROLLMENT ASSESSMENT FORM\n");
        fprintf(file, "=========================\n\n");
        fprintf(file, "Student ID: %s\n", system->students[stdIdx].id);
        fprintf(file, "Name: %s %s\n", system->students[stdIdx].name.first, 
                                      system->students[stdIdx].name.last);
        fprintf(file, "Program: %s\n", system->students[stdIdx].program);
        fprintf(file, "EAF Number: %d\n\n", system->students[stdIdx].eafNumber);

        fprintf(file, "ENROLLED COURSES:\n");
        fprintf(file, "%-10s %-8s %-5s %-6s %-10s %-10s %-20s\n", 
                "Code", "Section", "Units", "Days", "Time", "Room", "Faculty");
        fprintf(file, "----------------------------------------------------------------\n");

        i = 0;
        while (i < system->students[stdIdx].enrolledCount)
		{
            j = 0;
            int courseFound = 0;
            
            // Find course details
            while (j < system->courseCount && !courseFound) 
			{
                if (strcmp(system->courses[j].code, system->students[stdIdx].enrolledCourses[i].code) == 0 && strcmp(system->courses[j].section, system->students[stdIdx].enrolledCourses[i].section) == 0) 
					{
                    String80 facultyName = "";
                    if (strlen(system->courses[j].facultyName.first) > 0 || strlen(system->courses[j].facultyName.last) > 0) 
					{
                        sprintf(facultyName, "%s, %s", system->courses[j].facultyName.last, system->courses[j].facultyName.first);
                    }
                    
                    fprintf(file, "%-10s %-8s %-5d %-6s %-10s %-10s %-20s\n",
                           system->courses[j].code,
                           system->courses[j].section,
                           system->courses[j].units,
                           system->courses[j].days,
                           system->courses[j].time,
                           system->courses[j].room,
                           facultyName);
                    
                    totalUnits += system->courses[j].units;
                    courseFound = 1;
                }
                j++;
            }
            i++;
        }

        // Print footer
        fprintf(file, "\nTotal Units Enrolled: %d\n", totalUnits);
        fprintf(file, "\nAs of: %s %s\n", __DATE__, __TIME__);

        // Close file
        fclose(file);
        printf("EAF successfully saved as %s\n", filename);
    } else {
        printf("Error: Could not create EAF file for student %s\n", system->students[stdIdx].id);
    }
}

void studentMenu(System *system) {
    String10 studentID;
    int choice;
    int studentIndex;
    int flag = 1;

    clearScreen();

    do {
        printf("Please Enter Your Student ID: ");
        scanf("%s", studentID);

        studentIndex = studentLogin(system, studentID);
        if (studentIndex == -1) {
            printf("Invalid Student ID! Please try again.\n");
        }
    } while (studentIndex == -1);

    while (flag) {
        clearScreen();
        printf("\nWelcome, %s!", system->students[studentIndex].name.first);
        printf("\n");
        printf("========================================\n");
        printf("|             STUDENT MENU             |\n");
        printf("========================================\n");
        printf("|                                      |\n");
        printf("|                                      |\n");
        printf("| Choose your transaction:             |\n");
        printf("|                                      |\n");
        printf("| [1] Enroll Courses                   |\n");
        printf("| [2] Print EAF                        |\n");
        printf("| [3] View Enrolled Courses            |\n"); 
        printf("| [4] View Completed Courses           |\n"); 
        printf("| [5] Exit to Return to Main Menu      |\n");
        printf("|                                      |\n");
        printf("========================================\n");
        printf("\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou selected: Enroll Courses\n");
                enrollCourses(system, studentIndex);
                pauseProgram();
                break;
            case 2:
                printf("\nYou selected: Print EAF\n");
                printEAF(system, studentIndex); 
                pauseProgram();
                break;
            case 3:
                printf("\nYou selected: View Enrolled Courses\n");
                viewCurrentEnrollment(system, studentIndex);
                pauseProgram();
                break;
            case 4:
                printf("\nYou selected: View Completed Courses\n");
                viewCompletedCourses(system, studentIndex); 
                pauseProgram();
                break;
            case 5:
                printf("\nReturning to Main Menu...\n");
                getchar();
                getchar();
                flag = 0; 
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
    }
}

// ACADEMIC ASSISTANT FUNCTIONS

void enterCourses(System *system) {
    Course newCourse;
    int conflictFound = 0; //flagg for conflicts
    int i, j, k;
    String80 conflictReason;
    int newCStart, newCEnd, curCStart, curCEnd;
    int roomExists;

    //initialize new course
    strcpy(newCourse.code, "");
    strcpy(newCourse.section, "");
    newCourse.units = 0;
    strcpy(newCourse.days, "");
    strcpy(newCourse.time, "");
    strcpy(newCourse.room, "");
    strcpy(newCourse.facultyName.first, "");
    strcpy(newCourse.facultyName.last, "");
    newCourse.prereqCount = 0;

    printf("\n===== ADD NEW COURSE =====\n");

    printf("Enter course code (e.g., CCPROG1): ");
    scanf("%s", newCourse.code);

    printf("Enter section (e.g., S11): ");
    scanf("%s", newCourse.section);

    printf("Enter units: ");
    scanf("%d", &newCourse.units);

    printf("Enter days (e.g., MWF): ");
    scanf("%s", newCourse.days);

    printf("Enter time (e.g., 0830-1400): ");
    scanf("%s", newCourse.time);

	//checks if room exists
    do 
	{
        printf("Enter room: ");
        scanf("%s", newCourse.room);

        // Check if room exists (without break)
        roomExists = 0;
        i = 0;
        for (i = 0; i < system->roomCount && roomExists == 0; i++) 
		{
            if (strcmp(system->rooms[i].roomNumber, newCourse.room) == 0) 
			{
                roomExists = 1;
            }
        }

        if (!roomExists) 
		{
            printf("Error: Room %s does not exist in the system.\n", newCourse.room);
        }
    } while (roomExists == 0);
    
    //sscanf new time into newcstart and newcend as integers
    sscanf(newCourse.time, "%4d-%4d", &newCStart, &newCEnd);

    i = 0;
    while (i < system->courseCount && !conflictFound) 
	{
        //check if same room
        if (strcmp(system->courses[i].room, newCourse.room) == 0) 
		{
            //check if day overlap
            j = 0;
            while (newCourse.days[j] != '\0' && !conflictFound) 
			{
                k = 0;
                while (system->courses[i].days[k] != '\0' && !conflictFound) 
				{
                    if (newCourse.days[j] == system->courses[i].days[k]) 
					{
                        //sscanf current time being checked into curcstart and curcend as integers
                        sscanf(system->courses[i].time, "%4d-%4d", &curCStart, &curCEnd);
                        
                        //check time overlap
                        if ((newCStart >= curCStart && newCStart < curCEnd) ||   //new starts during existing
                            (newCEnd > curCStart && newCEnd <= curCEnd) ||       //new ends during existing
                            (newCStart <= curCStart && newCEnd >= curCEnd) ||    //new completely overlaps existing
                            (newCStart >= curCStart && newCEnd <= curCEnd)) {    //new completely within existing
                            
                            conflictFound = 1;
                            strcpy(conflictReason, "Invalid, time conflict with ");
                            strcat(conflictReason, system->courses[i].code);
                            strcat(conflictReason, " ");
                            strcat(conflictReason, system->courses[i].section);
                            strcat(conflictReason, " at ");
                            strcat(conflictReason, system->courses[i].room);
                            strcat(conflictReason, " (");
                            strcat(conflictReason, system->courses[i].time);
                            strcat(conflictReason, ")");
                        }
                    }
                    k++;
                }
                j++;
            }
        }
        i++;
    }

    if (!conflictFound) 
	{
        if (system->courseCount < MAX_COURSES_OFFERED) 
		{
            system->courses[system->courseCount] = newCourse;
            system->courseCount++;
            printf("Course added successfully!\n");
        } 
		else 
		{
            printf("Error: Maximum number of courses reached (%d).\n", MAX_COURSES_OFFERED);
        }
    } 
	else 
	{
        printf("%s\n", conflictReason);
    }
    pauseProgram();
}

void enterPrerequisites(System *system) {
    String10 courseCode, prereqCode;
    int courseExists = 0;
    int prereqExists = 0;
    int maxPrereqsReached = 0;
    int i, j;
    int flag = 1;
    
    printf("\n===== ADD PREREQUISITE =====\n");
    
    // Get course and prerequisite codes
    printf("Enter course code to add prerequisite to (e.g., CCPROG3): ");
    scanf("%s", courseCode);
    
    printf("Enter prerequisite course code (e.g., CCPROG1): ");
    scanf("%s", prereqCode);
    
    // Check if both courses exist
    for (i = 0; i < system->courseCount; i++) 
	{
        if (strcmp(system->courses[i].code, courseCode) == 0) 
		{
            courseExists = 1;
        }
        if (strcmp(system->courses[i].code, prereqCode) == 0) 
		{
            prereqExists = 1;
        }
    }
    
    if (courseExists == 0 || prereqExists == 0) {
        printf("Error: One or both courses do not exist in the system.\n");
        flag = 0;
    }
    
    // Check if any instance of the course has max prerequisites
    i = 0;
    for (i = 0; i < system->courseCount && maxPrereqsReached == 0 && flag; i++) 
	{
        if (strcmp(system->courses[i].code, courseCode) == 0) 
		{
            if (system->courses[i].prereqCount >= 3) 
			{
                maxPrereqsReached = 1;
            }
        }
    }
    
    if (maxPrereqsReached) 
	{
        printf("Error: %s already has the maximum number of prerequisites (3).\n", courseCode);
        flag = 0;
    }
    
    //add prerequisite to all instances of the course
    for (i = 0; i < system->courseCount && flag; i++) {
        if (strcmp(system->courses[i].code, courseCode) == 0) {
            //check if prerequisite already exists
            j = 0;
            int alreadyExists = 0;
            for (j = 0; j < system->courses[i].prereqCount && alreadyExists == 0; j++) 
			{
                if (strcmp(system->courses[i].prereqCourse[j], prereqCode) == 0) 
				{
                    alreadyExists = 1;
                }
            }
            
            if (alreadyExists == 0) 
			{
                if (system->courses[i].prereqCount < 3) 
				{
                    strcpy(system->courses[i].prereqCourse[system->courses[i].prereqCount], prereqCode);
                    system->courses[i].prereqCount++;
                    printf("Added %s as prerequisite to %s %s\n", prereqCode, courseCode, system->courses[i].section);
                }
            } 
			else 
			{
                printf("%s is already a prerequisite for %s %s\n", prereqCode, courseCode, system->courses[i].section);
            }
        }
    }
    pauseProgram();
}

void scheduleAndRoomAssignment(System *system) 
{
	int i, j;
    printf("\n===== ROOM SCHEDULE ASSIGNMENT =====\n");
    
    for (i = 0; i < system->roomCount; i++) 
	{
        printf("\nROOM: %s\n", system->rooms[i].roomNumber);
        printf("-----------------------------------------------------------\n");
        printf("| %-10s | %-6s | %-6s | %-11s | %-9s |\n", 
               "Course", "Section", "Days", "Time", "Occupancy");
        printf("-----------------------------------------------------------\n");

        for (j = 0; j < system->rooms[i].classCount; j++) 
		{
            printf("| %-10s | %-7s | %-6s | %-11s | %-9d |\n",
                   system->rooms[i].courseCode[j],
                   system->rooms[i].section[j],
                   system->rooms[i].days[j],
                   system->rooms[i].time[j],
                   system->rooms[i].occupancy[j]);
        }
        
        printf("-----------------------------------------------------------\n");

        if (system->rooms[i].classCount == 0) 
		{
            printf("| %46s |\n", "No classes scheduled");
            printf("-----------------------------------------------------------\n");
        }
    }
    
    printf("\n===================================================\n");
    
    pauseProgram();
}
void academicAssistantMenu(System *system) 
{
    int choice = 0;
    clearScreen();
    
    while (choice != 7) 
	{ 
        
        printf("========================================\n");
        printf("|       ACADEMIC ASSISTANT MENU        |\n");
        printf("========================================\n");
        printf("|                                      |\n");
        printf("|                                      |\n");
        printf("| Choose your transaction:             |\n");
        printf("|                                      |\n");
        printf("| [1] Enter student information        |\n");
        printf("| [2] Enter faculty information        |\n");
        printf("| [3] Enter courses                    |\n");
        printf("| [4] Enter prerequisites              |\n");
        printf("| [5] Schedule and Room Assignment     |\n");
        printf("| [6] Display Faculty List             |\n");
        printf("| [7] Exit to Return to Main Menu      |\n");
        printf("|                                      |\n");
        printf("========================================\n");
        printf("\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                //enterStudentInformation(system);
                clearScreen();
                break;
            case 2:
                //enterFacultyInformation(system);
                break;
            case 3:
                enterCourses(system);
                clearScreen();
                break;
            case 4:
                enterPrerequisites(system);
                clearScreen();
                break;
            case 5:
                scheduleAndRoomAssignment(system);
                clearScreen();
                break;
            case 6:
                //displayFacultyList(system);
                break;
            case 7:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// FACULTY FUNCTIONS

void PrintList (System system){
	int i,j,k,x; 
	String10 id; 
	int success = 0; 
		printf("Please input ID: ");
		scanf("%s", id);
		for (i = 0; i < system.facultyCount;i++){
			if (strcmp(id,system.faculties[i].id)==0){
				for (j = 0; j <system.faculties[i].courseCount;j++){
					printf("\n%s %s\n",system.faculties[i].teachingLoad[j].code,system.faculties[i].teachingLoad[j].section);
					for (k=0; k<system.studentCount;k++){
						for (x=0; x<system.students[k].enrolledCount;x++){
							if (strcmp(system.faculties[i].teachingLoad[j].code,system.students[k].enrolledCourses[x].code)==0){
								if (strcmp(system.faculties[i].teachingLoad[j].section,system.students[k].enrolledCourses[x].section)==0){	
							}
						}
					}
				}
			}
		}
	}
}

void DeloadingUnits (System *system){
	int i;
	String10 id;
	printf("Please input ID:");
	scanf("%s", id);
		for (i = 0; i < system->facultyCount;i++){
			if (strcmp(id,system->faculties[i].id)==0){
				printf("Enter deloading units: ");
				scanf("%d", &system->faculties[i].deloadingUnits);
				printf ("Total units needed: %d\n", 12 - system->faculties[i].deloadingUnits);
			}
		}
}


void StudentList(System *system){
	int i,j,k,x; 
		for (i=0; i < system->courseCount;i++){
			x=0;
			for (j = 0; j<system->studentCount;j++){
				for (k=0; k < system->students[j].enrolledCount;k++){
					if (strcmp(system->courses[i].code,system->students[j].enrolledCourses[k].code)==0){
						if (strcmp(system->courses[i].section,system->students[j].enrolledCourses[k].section)==0){
							strcpy(system->courses[i].studentList[x].first,system->students[j].name.first);
							strcpy(system->courses[i].studentList[x].last,system->students[j].name.last);			
							x++;
						}
					}
				}
			}
		}
	}
		

void AvailableCourses(System system) {
    int i, j;
            for (j = 0; j < system.courseCount; j++) {
                if (strcmp(system.courses[j].facultyName.first, "") == 0) {  
                    printf("%s %s %s %s %s\n",
                           system.courses[j].code,
                           system.courses[j].section,
                           system.courses[j].days,  
                           system.courses[j].time,
                           system.courses[j].room);
                }
            }
    }
  
void AddCourse(System *system){
	int i,j;
	String10 id,temp1,temp2; 
	printf("id: ");
	scanf("%s", id);
		for (i=0; i < system->facultyCount;i++){
			if (strcmp(id,system->faculties[i].id)==0){
				printf("Welcome! %s %s\n",system->faculties[i].name.first,system->faculties[i].name.last);
				AvailableCourses(*system);
				
				printf("Enter Course code and Section: ");
				scanf("%s %s", temp1,temp2);
				
				for (j = 0; j < system->courseCount; j++) {
               		 if (strcmp(temp1, system->courses[j].code) == 0){
					  	if (strcmp(temp2, system->courses[j].section)==0){
					  		if (strcmp(system->courses[j].facultyName.first, "")==0){
					  			strcpy(system->faculties[i].teachingLoad[system->faculties[i].courseCount].code,temp1);
					  			strcpy(system->faculties[i].teachingLoad[system->faculties[i].courseCount].section,temp2);
					  			printf ("%s %s\n",system->faculties[i].name.first,system->faculties[i].name.last);
					  			printf("%s %s\n",system->faculties[i].teachingLoad[system->faculties[i].courseCount].code,
								  			   system->faculties[i].teachingLoad[system->faculties[i].courseCount].section);
								system->faculties[i].courseCount++;
							  }
						  }
           			}
    			}
			}
		}
}

void PrintCourse (System system){
	int i,j,k,x;
	int TotalUnits = 0;
	String10 id,temp1,temp2; 
	printf("id: ");
	scanf("%s", id);
		for (i=0; i < system.facultyCount;i++){
			if (strcmp(id,system.faculties[i].id)==0){
				printf("Welcome! %s %s\n",system.faculties[i].name.first,system.faculties[i].name.last);
				for (j=0; j < system.faculties[i].courseCount;j++){
					for (k=0; k < system.courseCount;k++){
						if (strcmp(system.faculties[i].teachingLoad[j].code,system.courses[k].code)==0){
							if (strcmp(system.faculties[i].teachingLoad[j].section,system.courses[k].section)==0){
								printf("\n%s %s %d %s %s %s\n", system.courses[k].code,system.courses[k].section,system.courses[k].units,
															system.courses[k].days,system.courses[k].time,system.courses[k].room);
															TotalUnits += system.courses[k].units;
							}
						}
					}	
				}
				if (12-system.faculties[i].deloadingUnits>TotalUnits){
				printf("\nYou are currently underloaded\n");
				}
				else if (12-system.faculties[i].deloadingUnits<TotalUnits){
				printf("\nYou are currently overloaded\n");
				}else{
					printf("Equal\n");
				}
			}
		}
}


void DeleteFacultyCourse(System *system) {
    int i, j, k,x, found = 0;
    String10 id, temp1, temp2;

    printf("Enter faculty ID: ");
    scanf("%9s", id);

    int facultyIndex = -1;
    for (i = 0; i < system->facultyCount; i++) {
        if (strcmp(id, system->faculties[i].id) == 0) {
            facultyIndex = i;
        }
    }

    if (facultyIndex == -1) {
        printf("Faculty ID not found.\n");
    } else {
        if (system->faculties[facultyIndex].courseCount == 0) {
            printf("No courses to delete.\n");
        } else {
            // Display available courses
            printf("Enter course code and section to delete:\n");
            for (j = 0; j < system->faculties[facultyIndex].courseCount; j++) {
                printf("%s %s\n", system->faculties[facultyIndex].teachingLoad[j].code,
                                   system->faculties[facultyIndex].teachingLoad[j].section);
            }

            scanf("%s %s", temp1, temp2);

            // Search for the course
            int courseIndex = -1;
            for (j = 0; j < system->faculties[facultyIndex].courseCount; j++) {
                if (strcmp(temp1, system->faculties[facultyIndex].teachingLoad[j].code) == 0 &&
                    strcmp(temp2, system->faculties[facultyIndex].teachingLoad[j].section) == 0) {
                    courseIndex = j;
                }
            }

            if (courseIndex == -1) {
                printf("Course not found.\n");
            } else {
                // Shift elements left
                for (k = courseIndex; k < system->faculties[facultyIndex].courseCount - 1; k++) {
                    system->faculties[facultyIndex].teachingLoad[k] = system->faculties[facultyIndex].teachingLoad[k + 1];
                }

                system->faculties[facultyIndex].courseCount--; // Reduce count
                printf("Course deleted successfully.\n");
                
                for (x = 0 ; x < system->courseCount; x++){ //Updates the faculty name of the course to an empty string
                	 if (strcmp(temp1,system->courses[x].code)==0);
                	 	if (strcmp(temp2,system->courses[x].section)==0){
                	 		strcpy (system->courses[x].facultyName.first,"");
                	 		strcpy (system->courses[x].facultyName.last,"");
						 }
				}
            }
        }
    }
}
