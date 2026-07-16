#ifndef FACULTY_H
#define FACULTY_H

typedef struct{
    char first_name[NAME_LEN];
    char last_name[LAST_NAME_LEN];
    char faculty_id[ID_LEN];
    char national_code[NATIONAL_CODE_LEN];
    char field[FIELD_LEN];
    int enterance_year;
    char section[SECTION_LEN];
}Faculty;

#endif // FACULTY_H
