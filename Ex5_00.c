#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
// Defines
#define MAX_COURSES 10
#define MAX_EXERCISES 13
#define UNDEFINED -1

// Declerations
typedef struct CourseInfo
{
	int courseNum;
	int grade;
}COURSE_INFO;

int GetSemesterCourses(COURSE_INFO SemesterA[],char Semester);
void sortCourses(COURSE_INFO data[], int size);
void printCourses(COURSE_INFO data[], int size);
void swap(COURSE_INFO data[], int src, int dest);
int unite(COURSE_INFO dataA[], int sizeA, COURSE_INFO dataB[], int sizeB, int uniteCourse[]);
int intersect(COURSE_INFO dataA[], int sizeA, COURSE_INFO dataB[], int sizeB, int interCourse[]);
void printCNum(int data[], int size);
COURSE_INFO findMinGrade(COURSE_INFO data[], int size);
int getGradesForCourse(int courseNum, int ExGrades[]);
void printExercisesGrades(int ExGrades[], int numEx);
int findFailPass(int grades[], int size);
void reorderGrades(int grades[], int size, int limit);
void printGradesBelowLimit(int ExercisesGrades[], int logSizeGrades, int limit);


// Functions
void main()
{
	COURSE_INFO SemesterA[MAX_COURSES];
	COURSE_INFO SemesterB[MAX_COURSES];
	int unionCourses[2 * MAX_COURSES]; // union is sum of MAX_COURSES in A and B
	int intersectionCourses[MAX_COURSES]; // intersection of A and B can be max MAX_COURSES
	int sizeSemesterA, sizeSemesterB; // logic size of Arry Semesters A and B
	int logSizeUnion, logSizeIntersection, logSizeGrades, limit;
	int ExercisesGrades[MAX_EXERCISES];
	COURSE_INFO worstCourse;
	
	printf(" Welcome students!!\nand bye bye Pizzeria\n\n");
	// Get grades and get back logic size
	sizeSemesterA = GetSemesterCourses(SemesterA, 'A');
	sizeSemesterB = GetSemesterCourses(SemesterB, 'B');
	// sorting courses
	sortCourses(SemesterA, sizeSemesterA);
	sortCourses(SemesterB, sizeSemesterB);
	// print the courses
	printf("\nSorted courses of semester A: \n");
	printCourses(SemesterA, sizeSemesterA);
	printf("\nSorted courses of semester B: \n");
	printCourses(SemesterB, sizeSemesterB);
	// make union and intersection of course numbers:
	printf("\ncourses taken in semester A or semester B: ");
	logSizeUnion = unite(SemesterA, sizeSemesterA, SemesterB, sizeSemesterB, unionCourses);
	printCNum(unionCourses, logSizeUnion);
	printf("courses taken in semester A and semester B: ");
	logSizeIntersection = intersect(SemesterA, sizeSemesterA, SemesterB, sizeSemesterB, intersectionCourses);
	printCNum(intersectionCourses, logSizeIntersection);
	
	// check worst grade
	worstCourse = findMinGrade(SemesterA, sizeSemesterA);
	printf("Minimum grade in semester A is: %d in course #%d \n", worstCourse.grade, worstCourse.courseNum);
	// get grade in exercies of the worst course 
	logSizeGrades = getGradesForCourse(worstCourse.courseNum,ExercisesGrades);
	// search for index of fail that next grade by the order is pass
	printf("Index of Fail-Pass is: %d\n", findFailPass(ExercisesGrades, logSizeGrades));
	// make new order in array by limit
	printf("Please enter a limit grade: ");
	scanf("%d", &limit);
	printf("\n");
	reorderGrades(ExercisesGrades, logSizeGrades, limit);
	printGradesBelowLimit(ExercisesGrades, logSizeGrades, limit);

}

int GetSemesterCourses(COURSE_INFO data[], char Semester) // ask the user for how many courses in semester and get grades and course number for all the courses in the semester
{
	int i,num,courseNum,courseGrade;
	printf("Please enter number of courses in semester %c: ",Semester);
	scanf("%d", &num);
	printf("\n");
	for (i = 0; i < num && i < MAX_COURSES; i++)
	{
		printf("Enter course number and grade: ");
		scanf("%d %d", &courseNum, &courseGrade);
		printf("\n");
		data[i].courseNum = courseNum;
		data[i].grade = courseGrade;
	}
	return num;
}

void sortCourses(COURSE_INFO data[], int size) // bubble sort for course info struct array - efficiency (size)^2 
{
	int i,j;
	for(i=0;i<size-1;i++)
		for (j = 0; j < size - i - 1; j++)
		{
			if (data[j].courseNum > data[j + 1].courseNum)
				swap(data, j, j + 1);		
		}
}

void printCourses(COURSE_INFO data[], int size) // function for print all the courses and grades
{
	int i,count_digits = 1,check_number,j;
	printf("Course# Grade\n");
	printf("======= =====\n");
	
	for(i=0;i<size&&i<MAX_COURSES;i++)
	{
		check_number = data[i].courseNum;
		while ( check_number / 10 != 0)
		{
			count_digits++;
			check_number = check_number / 10;
		}
		printf("%d", data[i].courseNum);
		for (j = 0; j <= 8 - count_digits; j++)
			printf(" ");
			printf("%d", data[i].grade);
		printf("\n");
	}
}

void swap(COURSE_INFO data[], int src, int dest) // function for swapping two structs in array
{
	COURSE_INFO temp = data[src];
	data[src] = data[dest];
	data[dest] = temp;
}

int unite(COURSE_INFO dataA[], int sizeA, COURSE_INFO dataB[], int sizeB, int uniteCourse[]) // function that check which courses the student study in both semester and put in united array for course numbers -  efficiency in worst case is (2*MAX_COURSES)
{ // in the function we 
	int indexA = 0, indexB = 0,indexUnion = 0;
	while (indexA < sizeA && indexB < sizeB) // loop while the index of both arrays lower than logic size
	{
		if(dataA[indexA].courseNum == dataB[indexB].courseNum) // if the course number is same in the both of arrays, put this number in the new array of union and skip to next index in arrays
		{
			uniteCourse[indexUnion] = dataA[indexA].courseNum;
			indexUnion++;
			indexA++;
			indexB++;
		}
		else if (dataB[indexB].courseNum < dataA[indexA].courseNum) // if the course number in dataB smaller than in dataA, put the course number in dataB in the union and skip to next indexB
		{
			uniteCourse[indexUnion] = dataA[indexB].courseNum;
			indexB++;
			indexUnion++;
		}
		else if (dataA[indexA].courseNum < dataB[indexB].courseNum) // if the course number in dataA smaller than in dataB, put the course number in dataA in the union and skip to next indexA
		{
			uniteCourse[indexUnion] = dataA[indexA].courseNum;
			indexA++;
			indexUnion++;
		}
	}
	while (indexA < sizeA) // this loop copy the values that left in the array if the course number of  array B smaller than in array A
	{
		uniteCourse[indexUnion] = dataA[indexA].courseNum;
		indexUnion++;
		indexA++;
	}
	while (indexB < sizeB) // this loop copy the values that left in the array if the course number of  array A smaller than in array B
	{
		uniteCourse[indexUnion] = dataB[indexB].courseNum;
		indexUnion++;
		indexB++;
	}
	return indexUnion; // return logic size
}
int intersect(COURSE_INFO dataA[], int sizeA, COURSE_INFO dataB[], int sizeB, int interCourse[]) // the function check the two arrays and put in new array the course number that found in both arrays - efficiency in worst case is (2*MAX_COURSES)
{
	int indexA = 0, indexB = 0, indexInter = 0;
	while (indexA <= sizeA && indexB <= sizeB) // loop while the index of both arrays smaller than the logic size
	{
		if (dataA[indexA].courseNum == dataB[indexB].courseNum) // if the course number same in this indexs in both arrays it copy the coursenumber for the new array
		{
			interCourse[indexInter] = dataA[indexA].courseNum;
			indexInter++;
			indexA++;
			indexB++;
		}
		if (dataA[indexA].courseNum > dataB[indexB].courseNum)
			indexB++;
		if (dataA[indexA].courseNum < dataB[indexB].courseNum)
			indexA++;
	}
	return indexInter; // return logic size
}
void printCNum(int data[], int size) // print the numbers in array
{ 
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d ", data[i]);
	}
	printf("\n");
}

COURSE_INFO findMinGrade(COURSE_INFO data[], int size) // function scan all the grades in the array and save the lowest grade and his index 
{
	int i,worstGrade=100,worstGradeIndex=0;
	for (i = 0; i < size; i++)
	{
		if (data[i].grade < worstGrade)
		{
			worstGrade = data[i].grade;
			worstGradeIndex = i;
		}
	}
	return data[worstGradeIndex];
}
int getGradesForCourse(int courseNum, int ExGrades[]) // ask for the course with the lowest grade for how many exercises and get all of them and change the first to 30 and the last to 100
{
	int i, numEx,grade;
	printf("How many exercises were given in course #%d? \n", courseNum);
	scanf("%d", &numEx);
	printf("Enter exercises grades: ");
	for (i = 0; i < numEx; i++)
	{
		scanf("%d", &grade);
		ExGrades[i] = grade;
	}
	printf("\n");
	ExGrades[numEx - 1] = 100; // change last grade to 100
	ExGrades[0] = 30; // change first grade to 30
	printExercisesGrades(ExGrades, numEx);
	printf("\n");
	return numEx;
}
void printExercisesGrades(int ExGrades[],int numEx) // print all the grades in the array
{
	int i;
	printf("Exercise grades: ");
	for (i = 0; i < numEx; i++)
		printf("%d ", ExGrades[i]);
	printf("\n");
}

int findFailPass(int grades[], int size) // check the grades untill it find one grades that fail and the next grade is pass and return the index - efficiency - (size)
{
	int i;
	for (i = 0; i < size-1; i++)
	{
		if (grades[i] < 60 && grades[i + 1] >= 60)
			return i;
	}
	return UNDEFINED;
}

void reorderGrades(int grades[], int size, int limit) // function re ordering the grades below limit in left and the rest right of the array - efficiency in worst case is (size)
{
	int read=0, write=0,temp;
	while (read < size)
	{
		if (grades[write] <= limit)
			write++;
		else if (grades[read] <= limit)
		{
			
			temp = grades[write];
			grades[write] = grades[read];
			grades[read] = temp;
			write++;

		}
		read++;
	}
}

void printGradesBelowLimit(int ExercisesGrades[], int logSizeGrades,int limit)
{
	int i;
	printf("\nAfter reordering grades, the grades smaller than %d are: ", limit);
	for (i = 0; ExercisesGrades[i] < limit && i < logSizeGrades; i++)
		printf("%d ", ExercisesGrades[i]);
	printf("\n");
}