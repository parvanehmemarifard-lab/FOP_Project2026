#ifndef OFFERING_H
#define OFFERING_H
#include "course.h"
#include "faculty.h"
//#include "faculty.h"

typedef struct{
	Course course;
	Faculty faculty;
	int semester;
    int capacity;
    int enrollments;
    char place[PLACE_LEN];
} Offering;

void printOfferingAdmin(Offering current, int num);
void printOfferingStudent(Offering current, int num);

#endif // OFFERING_H
