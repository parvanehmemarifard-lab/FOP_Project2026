#ifndef FACULTY_H
#define FACULTY_H
#include "consts.h"
//#include "offering.h"

typedef struct{
    char first_name[NAME_LEN];
    char last_name[LAST_NAME_LEN];
    char faculty_id[ID_LEN];
    char national_code[NATIONAL_CODE_LEN];
    char field[FIELD_LEN];
    char entrance_year[YEAR];
    char section[SECTION_LEN];
    char password[PASSWORD_LEN];
}Faculty;

void facultyDashboard(int pos);
void facultyMyOfferings(Faculty faculty);
void searchMyOfferings(Faculty faculty);
void goToOffering(Faculty faculty);
void offeringListFaculty();

void offerCourse(Faculty faculty);


void printFaculty(Faculty current);
void facultyList();
void facultySearch(FILE* file);
void registerFaculty();
void removeFaculty();

#endif // FACULTY_H
