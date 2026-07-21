#ifndef COURSE_H
#define COURSE_H
#include "consts.h"

typedef struct{
    char course_name[COURSE_NAME_LEN];
    char course_id[COURSE_ID_LEN];
    int units;
    int num_prequisites;
    char prerequisites[MAX_PREREQUISITES][COURSE_ID_LEN];
    char section[SECTION_LEN];
    char field[FIELD_LEN];
    char department[DEPARTMENT_LEN];
} Course;

void printCourse(Course current);
void searchCourse();
void coursesListGeneral();

#endif // COURSE_H
