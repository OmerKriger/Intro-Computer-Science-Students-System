#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/*
Pogram summary:
The pogram is system that get information about 3 group of students that 6 student in each group
the pogram get Full name, ID, courses and grades.
ask the user for specific course number and give him the students in this group and change specific name (Lois) to different name (Lane)
and remove from the list students with the name "Lex"
and sum up the best grade and second of the course from each group
*/

// Defines

#define GROUP_A 0
#define GROUP_B 1
#define GROUP_C 2
#define GROUPS 3
#define NUM_OF_STUDENTS 6
#define MAX_COURSES 10
#define MAX_NAME_LEN 29
#define MAX_CHAR_NAME 11
#define SIZE_ARR 0
#define UNDEFINED -1
#define changeName "Lois"
#define toName "Lane"
#define nameToDelete "Lex"



// Declaration

typedef struct CourseInfo
{
	int courseNum;
	int grade;
} COURSE_INFO;
typedef struct Student
{
	char name[MAX_NAME_LEN];
	int identity;
	int numOfCourses; //number of courses taken in semesterA
	COURSE_INFO course_info[MAX_COURSES];
} STUDENT;

void printWelcome();
void getStudentsDetails(STUDENT Groups[GROUPS][NUM_OF_STUDENTS], int numOfGroups, int studentsInGroup);
void getFullName(char StudentName[], char FirstName[], char SurName[]);
int getStudentNames(STUDENT stuData[][NUM_OF_STUDENTS], int rows, int cols, int courseNum, char stuNames[][MAX_NAME_LEN]);
void printStudentsNames(char names[][MAX_NAME_LEN], int numOfStudents);
void changeString(char desString[], char srcString[], int indexStart, int amountOfChar);
void changeStringFromTo(char stuNames[][MAX_NAME_LEN], int numOfStudents, char changeFrom[], char changeTo[]);
int removeNameFromList(char stuNames[][MAX_NAME_LEN], int numOfStudents, char nameToRemove[]);
void removeName(char stuNames[][MAX_NAME_LEN], int IndexRemove, int numOfStudents);
void getGradesInCourse(STUDENT stuData[][NUM_OF_STUDENTS], int rows, int cols, int courseNum, int Grades[][GROUPS * NUM_OF_STUDENTS + 1]);
void printGrades(int Grades[][GROUPS * NUM_OF_STUDENTS + 1], int courseNum, int size);
void bestInGroups(int Grades[][GROUPS * NUM_OF_STUDENTS + 1], int courseNum, int size);
bool searchInString(char string[], char find[]);

// Functions

void main()
{
	int courseNum, numStudentsInCourse;
	char studentNames[GROUPS * NUM_OF_STUDENTS][MAX_NAME_LEN]; //  GROUPS * NUM_OF_STUDENTS the maximum amount of students able to be in one course.
	STUDENT Students[GROUPS][NUM_OF_STUDENTS];
	int courseGrades[GROUPS][GROUPS * NUM_OF_STUDENTS + 1]; // +1 for place to size of arr in first cell

	printWelcome();
	getStudentsDetails(Students, GROUPS, NUM_OF_STUDENTS); // call for function to get students details, ID, grades and ect.
	printf("\nEnter a course number: ");
	scanf("%d", &courseNum);
	numStudentsInCourse = getStudentNames(Students, GROUPS, NUM_OF_STUDENTS, courseNum, studentNames); // get list of students that participant in specific course and the amount of students in this course
	printf("Names of students in course#%d: \n", courseNum);
	printStudentsNames(studentNames, numStudentsInCourse); // print the students list in this course
	printf("\nNames after changing \"Lois\" to \"Lane\": \n");
	changeStringFromTo(studentNames, numStudentsInCourse, changeName, toName); // call to function that change part of strings it search string and change it to other string
	printStudentsNames(studentNames, numStudentsInCourse); // print the students list after change names
	numStudentsInCourse = removeNameFromList(studentNames, numStudentsInCourse, nameToDelete); // call to function that remove strings from the array if they have specific string inside of them
	printf("\nNames after deleting names with \"%s\": \n", nameToDelete);
	printStudentsNames(studentNames, numStudentsInCourse);// print the students list after delete names
	getGradesInCourse(Students, GROUPS, NUM_OF_STUDENTS, courseNum, courseGrades); // put the grades of all the students in specific course in a matrix
	printGrades(courseGrades, courseNum, GROUPS); // print the grades in the matrix
	bestInGroups(courseGrades, courseNum, GROUPS); // search for the best grade and the second in all the groups

}

void printWelcome() // function for say welcome to the user
{
	printf(" ********************\n");
	printf("* Welcome Students *\n");
	printf("********************\n\n");

}
void getStudentsDetails(STUDENT Groups[GROUPS][NUM_OF_STUDENTS], int numOfGroups, int studentsInGroup) // function that get all groups students names, id, and course details
{
	char FirstName[MAX_CHAR_NAME], Surname[MAX_CHAR_NAME];
	int groupIndex, studentIndex, i;
	char group = 'A'; // var for print the group letter, set to the first group "A"
	for (groupIndex = 0; groupIndex < numOfGroups; groupIndex++) // Run for all the groups 
	{
		printf("Enter students data for GROUP %c:\n", group);
		printf("________________________________\n");
		for (studentIndex = 0; studentIndex < studentsInGroup; studentIndex++) // run for all the students in this group we work on right now
		{
			printf("\nEnter student first name and last name (seperated by spaces): ");
			scanf("%s %s", FirstName, Surname); // get firstname and surname from the user
			getFullName(Groups[groupIndex][studentIndex].name, FirstName, Surname); // send the first name and surname and put it togther in Student details name

			printf("Enter student ID: ");
			scanf("%d", &Groups[groupIndex][studentIndex].identity); // get the student ID from user

			printf("\nEnter number of courses taken in semester A: ");
			scanf("%d", &Groups[groupIndex][studentIndex].numOfCourses); // get the amount of courses that this student studying in this semester

			for (i = 0; i < Groups[groupIndex][studentIndex].numOfCourses; i++) // run the amount of courses that this student took and get the course numbers and grades
			{
				printf("Enter Course number and grade: ");
				scanf("%d %d", &Groups[groupIndex][studentIndex].course_info[i].courseNum, &Groups[groupIndex][studentIndex].course_info[i].grade); // get the course number and the grade
			}

		}
		group++; // move to the next letter in ABC for the next group
	}
}
void getFullName(char StudentName[], char FirstName[], char SurName[]) // Function that take first name and Surname and get it togther for Full Name
{
	strcpy(StudentName, FirstName); // copy First name for new string
	strcat(StudentName, " "); // add space after firstname
	strcat(StudentName, SurName); // add the surname to the string of the student name
}
int getStudentNames(STUDENT stuData[][NUM_OF_STUDENTS], int rows, int cols, int courseNum, char stuNames[][MAX_NAME_LEN]) // function that check all the groups and students if they in specific course and if they are put thier name in array of names (list)
{
	int groupIndex, studentIndex, i, NameIndex = 0;

	for (groupIndex = 0; groupIndex < rows; groupIndex++) // run for all the groups
	{
		for (studentIndex = 0; studentIndex < cols; studentIndex++) // run for all the students in the group
		{
			for (i = 0; i < stuData[groupIndex][studentIndex].numOfCourses; i++) // check for all courses that the student registered
				if (stuData[groupIndex][studentIndex].course_info[i].courseNum == courseNum) // check if the course is the same course we searching students who registered
				{
					switch (groupIndex) // check which group the student belong to A/B/C
					{
					case GROUP_A: // if belong Group A so put in the list "GroupA" and after it his name
						strcpy(stuNames[NameIndex], "GroupA ");
						break;
					case GROUP_B: // if belong Group B so put in the list "GroupB" and after it his name
						strcpy(stuNames[NameIndex], "GroupB ");

						break;
					case GROUP_C: // if belong Group B so put in the list "GroupC" and after it his name
						strcpy(stuNames[NameIndex], "GroupC ");
						break;
					}
					strcat(stuNames[NameIndex], stuData[groupIndex][studentIndex].name); // adding the full name of the student after the group name in the student array in this course
					NameIndex++;
				}
		}
	}
	return NameIndex;
}
void printStudentsNames(char names[][MAX_NAME_LEN], int numOfStudents) // function for print names array
{
	int i;
	for (i = 0; i < numOfStudents; i++) // run for all names MAT and print the strings
		printf("%s \n", names[i]);
}
void changeStringFromTo(char stuNames[][MAX_NAME_LEN], int numOfStudents, char changeFrom[], char changeTo[])
{
	int nameIndex, letterIndex = 0, i, len = strlen(changeTo);
	bool stop;
	for (nameIndex = 0; nameIndex < numOfStudents; nameIndex++) // run for all the names in the array
	{
		letterIndex = 0;
		while (stuNames[nameIndex][letterIndex] != 0) // run on all the letters in the string
		{
			if (stuNames[nameIndex][letterIndex] == changeFrom[0]) // if find first letter of "changeFrom" check if the rest of letters in the string
			{
				i = 1;
				stop = false;
				while (stuNames[nameIndex][letterIndex + i] != 0 && stop == false) // run for the all letters of name we checking till the String finish or something force the while stop (bool stop)
				{
					if (changeFrom[i] != stuNames[nameIndex][letterIndex + i]) // checking letters if they are same if isnt same stop the while loop from start next loop
						stop = true;
					else if (i == (len - 1) && stop == false) // if we in the last letters and still the letters same the we found the changeFrom name
					{
						changeString(stuNames[nameIndex], changeTo, letterIndex, len); // function that change string 
						stop = true; // force break the while loop because the name is we found the name we want and the job is done
						letterIndex = letterIndex + i; // countinue the search from next name in the string from where we stopped the comparing
					}
					i++;
				}

			}
			letterIndex++;
		}
	}
}
void changeString(char desString[], char srcString[], int indexStart, int amountOfChar) // function that get two strings and replace one string in part of the second one
{
	int i;
	for (i = 0; i < amountOfChar; i++) // run from indexStart for replace for amount of chars needed
	{
		desString[indexStart + i] = srcString[i]; // replacing the chars
	}
}
int removeNameFromList(char stuNames[][MAX_NAME_LEN], int numOfStudents, char nameToRemove[])
{
	int readIndex = 0, writeIndex = 0, countDelete=0;
	bool nameFoundInRead,nameFoundInWrite;
	nameFoundInWrite = searchInString(stuNames[writeIndex], nameToRemove); // check if the name is in writeIndex in list 
	for (readIndex = 0; readIndex < numOfStudents; readIndex++) // check all the names in the array
	{	
		nameFoundInRead = searchInString(stuNames[readIndex], nameToRemove); // check if the name is in readIndex in list 
		if (nameFoundInRead == true) // check if need to remove the name we checking now for counting
			countDelete++;// counting how much we deleted for decrease at the end
		if (nameFoundInWrite == false && readIndex == writeIndex) // if the name isnt found in writeIndex skip for the next
		{
			writeIndex++; // next index for check
			nameFoundInWrite = searchInString(stuNames[writeIndex], nameToRemove); // check if the name is in the new index of writing
		}
		else if (nameFoundInRead == false) // if the name in writeIndex and not in read Index overwrite the name
		{
			strcpy(stuNames[writeIndex], stuNames[readIndex]); // overwrite the name
			writeIndex++; // skip to next index for writing
			nameFoundInWrite = searchInString(stuNames[writeIndex], nameToRemove); // check if the name is in the new index
		}
	}
	return (numOfStudents-countDelete);
}
bool searchInString(char string[], char find[])
{
	int letterIndex = 0,index;
	bool stop;
	while (string[letterIndex] != 0) // check all the letters in string
	{
		if (string[letterIndex] == find[0]) // check if the letter is same like first letter of name to remove
		{
			index = 0;
			stop = false;
			while (find[index] != '\0' && stop == false) // check the rest of the letters
			{
				if (find[index] != string[letterIndex + index])
					stop = true;
				else if (find[index+1] == '\0' && stop == false)
					return true;
				index++;
			}

		}
		letterIndex++;
	}
	return false;
}
void getGradesInCourse(STUDENT stuData[][NUM_OF_STUDENTS], int rows, int cols, int courseNum, int Grades[][GROUPS * NUM_OF_STUDENTS + 1]) // function that check all the groups and students if they in specific course and if they are put thier name in array of names (list)
{
	int groupIndex, studentIndex, i, IndexInGrades;

	for (groupIndex = 0; groupIndex < rows; groupIndex++) // run for all the groups
	{
		IndexInGrades = 0;
		for (studentIndex = 0; studentIndex < cols; studentIndex++) // run for all the students in the group
		{
			for (i = 0; i < stuData[groupIndex][studentIndex].numOfCourses; i++) // check for all courses that the student registered
				if (stuData[groupIndex][studentIndex].course_info[i].courseNum == courseNum) // check if the course is the same course we searching students who registered
				{
					IndexInGrades++;
					Grades[groupIndex][IndexInGrades] = stuData[groupIndex][studentIndex].course_info[i].grade;

				}

		}
		Grades[groupIndex][SIZE_ARR] = IndexInGrades; // set the number in the first cell for the amount of students from this GROUP
	}
}
void printGrades(int Grades[][GROUPS * NUM_OF_STUDENTS + 1], int courseNum, int size)
{
	int i, j;
	char Group = 'A';
	printf("\nGrades in course#%d: \n", courseNum);
	for (i = 0; i < size; i++)
	{
		printf("Group%c: ", Group);
		for (j = 1; j <= Grades[i][SIZE_ARR]; j++)
		{
			printf("%d ", Grades[i][j]);
		}
		printf("\n");
		Group++;
	}
}

void bestInGroups(int Grades[][GROUPS * NUM_OF_STUDENTS + 1], int courseNum, int size)
{
	char group = 'A';
	int i, j;
	int bestGrade = UNDEFINED, secondGrade = UNDEFINED;
	printf("\nMaximum grades in course#%d: \n", courseNum);
	for (i = 0; i < size; i++)
	{
		for (j = 1; j <= Grades[i][SIZE_ARR]; j++)
		{
			if (Grades[i][j] > bestGrade)
			{
				if (bestGrade != UNDEFINED) // move 1st grade to 2nd grade in the course
					secondGrade = bestGrade;
				bestGrade = Grades[i][j]; // set the new best grade
			}
			else if (Grades[i][j] > secondGrade)
				secondGrade = Grades[i][j]; // set secondry best grade
		}
		printf("Group%c: ", group);
		if (bestGrade != UNDEFINED && secondGrade != UNDEFINED) // if this two vars is defined print two grades
			printf("max grade is: %d and second max is: %d \n", bestGrade, secondGrade);
		else if (bestGrade != UNDEFINED && secondGrade == UNDEFINED) // if one grade define print one grade
			printf("max grade is: %d (no second max found) \n", bestGrade);
		else if (bestGrade == UNDEFINED && secondGrade == UNDEFINED) // if two grades undefined print no grades found
			printf("no grades found in group\n");
		// move to the next group letter in ABC
		group++;
		// reset the grades for the next groups
		bestGrade = UNDEFINED;
		secondGrade = UNDEFINED;
	}

}
