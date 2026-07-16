#ifndef STUDENT_H
#define STUDENT_H
#include "consts.h"
#include "offering.h"

typedef struct{
	Offering offering;
	int grade;
    int score;
} Enrolled_offering;

typedef struct{
    Enrolled_offering reports[100];
    int num_offering;
    char first_name[NAME_LEN + 1];
    char last_name[LAST_NAME_LEN + 1];
    char student_id[ID_LEN + 1];
    char national_code[NATIONAL_CODE_LEN + 1];
    char field[FIELD_LEN + 1];
    char entrance_year[YEAR];
    char section[SECTION_LEN + 1];
    char mentor[MENTOR_LEN + 1];
    char department[DEPARTMENT_LEN + 1];
    char answer1[CITY_LEN + 1];
    char answer2[BOOK_LEN + 1];
    char answer3[COLOR_LEN + 1];
    char password[PASSWORD_LEN + 1];
} Student;

int studentLogin();
void studentDashboard(int pos);

#endif // STUDENT_H
