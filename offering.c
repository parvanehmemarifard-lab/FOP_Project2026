#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "offering.h"
#include "student.h"

void printOfferingGeneral(Offering current, int num){
    printf("|%d| %s | %s | ", num, current.course.course_name, current.course.course_id);
    printf("%s %s | ", current.faculty.first_name, current.faculty.last_name);
    printf("%d | %d | %d | ", current.semester, current.capacity, current.enrollments);
    printf("%s| %s| \n" ,current.course.department, current.place);
}

void printOfferingAdmin(Offering current, int num){
    printf("|%d| %s | %s | ", num, current.course.course_name, current.course.course_id);
    printf("%s | ", current.faculty.faculty_id);
    printf("%d | %d | %d | ", current.semester, current.capacity, current.enrollments);
    printf("%s | %s | \n" ,current.course.department, current.place);
}

void searchOffering(int semester){
    system("cls");

    int opt, num = 1, found = 0;
    char phrase[50];

    printf("1. Search by course name\n");
    printf("2. Search by course id\n");
    printf("3. Search by faculty id\n");
    printf("4. Search by department\n");
    printf("Enter an option: ");
    scanf("%d", &opt);
    printf("Enter the phrase: ");
    scanf("%s", phrase);

    FILE* fPtr = fopen("offerings.json", "rb");
    Offering current;
    while(fread(&current, sizeof(Offering), 1, fPtr)){
        if (semester != current.semester) continue;
        switch(opt){
            case 1:
                if(strcmp(phrase, current.course.course_name) == 0) {printOfferingAdmin(current, num++); found = 1;}
                break;
            case 2:
                if(strcmp(phrase, current.course.course_id) == 0) {printOfferingAdmin(current, num++); found = 1;}
                break;
            case 3:
                if(strcmp(phrase, current.faculty.faculty_id) == 0) {printOfferingAdmin(current, num++); found = 1;}
                break;
            case 4:
                if(strcmp(phrase, current.course.department) == 0) {printOfferingAdmin(current, num++); found = 1;}
                break;
        }
    }
    if (found == 0) printf("No offerings found.");
    fclose(fPtr);
    getchar();
    getchar();
}



void addStudentToOffering(Offering offering){
    char id[ID_LEN];
    int pos = 0, found = 0;
    printf("Enter student id: ");
    scanf("%s", id);

    FILE* file = fopen("students.json", "rb+");
    Student current;

    while(fread(&current, sizeof(Student), 1, file)){
        if (strcmp(current.student_id, id) == 0){
            found = 1;
            break;
        }
        pos++;
    }
    if (found == 0){
        printf("Student not found");
        getchar();
        getchar();
    } else {
        if (offering.capacity >= offering.enrollments) printf("Not enough capacity.");
        if (passedPrerequisities(current, offering.course) == 0) printf("Student has not passed the prerequisities.");
        else {
            Enrolled_offering new_enroll;
            new_enroll.offering = offering;
            new_enroll.grade = -1;
            new_enroll.score = -1;
            current.reports[current.num_offering] = new_enroll;
            current.num_offering++;
            fseek(file, pos * sizeof(Student), SEEK_SET);
            fwrite(&current, sizeof(Student), 1, file);
            printf("Added student to the offering.");
        }
        getchar();
        getchar();
    }
    fclose(file);
}

void removeStudentFromOffering(Offering offering){
    char id[ID_LEN];
    int pos = 0, found = 0;
    printf("Enter student id: ");
    scanf("%s", id);

    FILE* file = fopen("students.json", "rb+");
    Student current;

    while(fread(&current, sizeof(Student), 1, file)){
        if (strcmp(current.student_id, id) == 0){
            found = 1;
            break;
        }
        pos++;
    }
    if (found == 0){
        printf("Student not found");
        getchar();
        getchar();
    } else {
        found = 0;
        int i = 0;
        for ( ; i < current.num_offering; i++){
            if (strcmp(current.reports[i].offering.course.course_id, offering.course.course_id) == 0
                && current.reports[i].offering.semester == offering.semester) {found = 1; break;}
        }

        if (found == 0) {
            printf("Student does not have this offering.");
            getchar();
            getchar();
            fclose(file);
            return;
        }

        for ( ; i < current.num_offering - 1; i++){
            current.reports[i] = current.reports[i + 1];
        }
        fseek(file, pos*sizeof(Student), SEEK_SET);
        fwrite(&current, sizeof(Student), 1, file);
        printf("Removed student from the offering.");
        getchar();
        getchar();
        fclose(file);
        return;
    }
}



void recordGrades(Offering offering){
    char filename[100];
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE* gradesFile = fopen(filename, "r");
    if (gradesFile == NULL) {
        printf("File was not found.");
        getchar();
        getchar();
        return;
    }

    FILE* studentsFile = fopen("students.json", "rb+");
    char studentId[ID_LEN];
    float grade;

    while (fscanf(gradesFile, "%s %f", studentId, &grade) == 2) {
        Student student;
        rewind(studentsFile);

        while (fread(&student, sizeof(Student), 1, studentsFile)) {
            if (strcmp(student.student_id, studentId) == 0){
                int updated = 0;
                for (int i = 0; i < student.num_offering; i++){
                    if (strcmp(student.reports[i].offering.course.course_id, offering.course.course_id) == 0 &&
                        strcmp(student.reports[i].offering.faculty.faculty_id, offering.faculty.faculty_id) == 0 &&
                        student.reports[i].offering.semester == offering.semester) {
                            student.reports[i].grade = grade;

                            fseek(studentsFile, -sizeof(Student), SEEK_CUR);
                            fwrite(&student, sizeof(Student), 1, studentsFile);
                            updated = 1;
                            break;
                    }
                }

                if (!updated) {
                    printf("Student %s has not enrolled in this offering.\n",studentId);
                }
                break;
            }
        }
    }

    fclose(gradesFile);
    fclose(studentsFile);

    printf("Grades recorded successfully.\n");
    getchar();
    getchar();
}
