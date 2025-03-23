#define MAX_COURSES 10
#define MAX_FACULTY_COURSES 5
#define MAX_COURSES_OFFERED 50
#define MAX_ROOMS 10
#define MAX_STUDENTS 20
#define MAX_FACULTY 20

typedef char String30[31];
typedef char String10[11];
typedef char String5[6];

typedef struct
{
	String30 first;
	String30 last;
} Name;

typedef struct 
{
    String10 code;
    String5 section;
    int units;
    char days[3]; 
    String10 time; 
    String10 room;
    Name facultyName;
	Name studentList[MAX_STUDENTS];
    String10 prereqCourse[3];
    int prereqCount;
} Course;

typedef struct
{
	String10 code;
	String10 section;
} CourseID;

typedef struct
{
	String10 id;
    Name name;
    String10 program;
    int eafNumber;
    int totalUnits;
    String10 subjectsTaken[MAX_COURSES]; 
    int completedCount;
    CourseID enrolledCourses[MAX_COURSES]; 
    int enrolledCount;	
} Student;

typedef struct {
    String10 id;
	Name name;
    String10 dept;
    int deloadingUnits;
    CourseID teachingLoad[MAX_FACULTY_COURSES];
    int courseCount;
} Faculty;

typedef struct {
    String10 roomNumber;
    String10 days[7];
    String10 time[7];
    String10 courseCode[7];
    String5 section[7];
    int classCount;
    int occupancy[7];
} Room;


typedef struct {
    Student students[20];
    int studentCount;

    Faculty faculties[20];
    int facultyCount;

    Course courses[MAX_COURSES_OFFERED];
    int courseCount;

    Room rooms[MAX_ROOMS];
    int roomCount;
} System;
