#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "consts.h"
#include "offering.h"
#include "admin.h"

int passedPrerequisities(Student student, Course course){
    int found;
    for (int i = 0; i < course.num_prequisites; i++){
        found = 0;
        for (int j = 0; j < student.num_offering; j++){
            if (strcmp(course.prerequisites[i], student.reports[j].offering.course.course_id) == 0){
                found = 1;
                break;
            }
        }
        if (found == 0) return 0;
    }
    return 1;
}

float calculateGPA(Student student){
    if (student.num_offering == 0) return 0.0;
    int found = 0;
    float sum = 0;
    int units = 0;
    for (int i = 0; i < student.num_offering; i++){
        if (student.reports[i].grade == -1) continue;
        found = 1;
        sum += student.reports[i].grade * student.reports[i].offering.course.units;
        units += student.reports[i].offering.course.units;
    }
    if (found == 0) return 0.0;
    return sum/units;
}



void studentDashboard(int pos){
    FILE* file = fopen("students.json", "rb");
    fseek(file, pos * sizeof(Student), SEEK_SET);
    Student student;
    fread(&student, sizeof(student), 1, file);
    fclose(file);

    int option;
    while(1){
            system("cls");
        printf("1. Offerings\n2. Courses\n");
        printf("3. Report card\n4. Log out\n");
        printf("Enter an option: ");
        scanf("%d", &option);
        switch(option){
            case 1:
                offeringListStudent(student, pos);
                break;
            case 2:
                coursesListGeneral();
                break;
            case 3:
                reportCardStudent(student);
                break;
            case 4:
                return;
        }
    }
}

void enrollCourse(Student *student, int semester, int pos){
    FILE* fileOfferigs = fopen("offerings.json", "rb+");
    Offering current;
    int num = 1, input, found = 0, offeringPos = 0;
    printf("Enter number of offering: ");
    scanf("%d", &input);
    while(fread(&current, sizeof(Offering), 1, fileOfferigs)){
        if (semester == current.semester){
            if (num == input){
                printOfferingGeneral(current, num);
                found = 1;
                break;
            }
            num++;
        }
        offeringPos++;
    }
    if (found == 0) {
        printf("Offering not found.");
        getchar();
        getchar();
        fclose(fileOfferigs);
        return;
    }
    if (calender[1] == 0) {
        printf("unit selection is disabled.");
        getchar();
        getchar();
        fclose(fileOfferigs);
        return;
    }
    if (current.capacity <= current.enrollments) {
        printf("Capasity is full.");
        fclose(fileOfferigs);
        return;
    }
    if (passedPrerequisities(*student, current.course) == 0){
        printf("You have not passed the prerequisities.");
        getchar();
        getchar();
        fclose(fileOfferigs);
        return;
    }
    Enrolled_offering new_offering;
    new_offering.grade = -1;
    new_offering.score = -1;
    new_offering.offering = current;
    student->reports[student->num_offering] = new_offering;
    student->num_offering++;

    current.enrollments++;
    fseek(fileOfferigs, offeringPos * sizeof(Offering), SEEK_SET);
    fwrite(&current, sizeof(Offering), 1, fileOfferigs);
    fclose(fileOfferigs);

    FILE* fileStudents = fopen("students.json", "rb+");
    fseek(fileStudents, pos * sizeof(Student), SEEK_SET);
    fwrite(student, sizeof(Student), 1, fileStudents);
    fclose(fileStudents);
    printf("Enrolled in offering.");
    getchar();
    getchar();
}

void withdrawCourse(Student *student, int semester, int pos){
    FILE* fileOfferigs = fopen("offerings.json", "rb+");
    Offering current;
    int num = 1, input, found = 0, offeringPos = 0;
    printf("Enter number of offering: ");
    scanf("%d", &input);
    while(fread(&current, sizeof(Offering), 1, fileOfferigs)){
        if (semester == current.semester){
            if (num == input){
                printOfferingGeneral(current, num);
                found = 1;
                break;
            }
            num++;
        }
        offeringPos++;
    }

    if (found == 0) {
        printf("Offering not found.");
        getchar();
        getchar();
        fclose(fileOfferigs);
        return;
    }
    if (calender[1] == 0) {
        printf("unit selection is disabled.");
        getchar();
        getchar();
        fclose(fileOfferigs);
        return;
    }

    current.enrollments--;
    fseek(fileOfferigs, offeringPos * sizeof(Offering), SEEK_SET);
    fwrite(&current, sizeof(Offering), 1, fileOfferigs);
    fclose(fileOfferigs);

    found = 0;
    int i = 0;
    for ( ; i < student->num_offering; i++){
        if (strcmp(student->reports[i].offering.course.course_id, current.course.course_id) == 0 &&
            strcmp(student->reports[i].offering.faculty.faculty_id, current.faculty.faculty_id) == 0
                && student->reports[i].offering.semester == current.semester) {found = 1; break;}
    }

    if (found == 0) {
        printf("You do not have this offering.");
        getchar();
        getchar();
        return;
    }

    for ( ; i < student->num_offering - 1; i++){
        student->reports[i] = student->reports[i+1];
    }
    student->num_offering--;
    FILE* fileStudents = fopen("students.json", "rb+");
    fseek(fileStudents, pos * sizeof(Student), SEEK_SET);
    fwrite(student, sizeof(Student), 1, fileStudents);
    fclose(fileStudents);
    printf("Withdrew the offering.");
    getchar();
    getchar();
}

void offeringListStudent(Student student, int pos){
    FILE* file = fopen("offerings.json", "rb");
    Offering current;
    int semester, option, num = 0;
    system("cls");
    printf("Enter semester number: ");
    scanf("%d", &semester);
    while(1){
        system("cls");
        rewind(file);
        num = 0;
        printf("Enter semester number: %d\n", semester);

        printf("List of offerings - %d\n| number | ", semester);
        printf("course name | course id | faculty name | semester | ");
        printf("capacity | no.enrollments | department | place |\n");

        while(fread(&current, sizeof(Offering), 1, file)){
            if (current.semester == semester &&
                passedPrerequisities(student, current.course))
                    printOfferingGeneral(current, ++num);
        }
        printf("1. Search\n2. Enroll in course\n");
        printf("3. Withdraw course\n4. Go back\n");
        printf("Enter an option: ");
        scanf("%d", &option);
        switch(option){
            case 1:
                searchOffering(semester);
                break;
            case 2:
                enrollCourse(&student, semester, pos);
                break;
            case 3:
                withdrawCourse(&student, semester, pos);
                break;
            case 4:
                return;
        }
    }
    fclose(file);
}

void reportCardStudent(Student student){
    int gpa = calculateGPA(student);
    while(1){
            system("cls");
        printf("|student id    |%20s |\n", student.student_id);
        printf("|first name    |%20s |\n", student.first_name);
        printf("|last name     |%20s |\n", student.last_name);
        printf("|national code |%20s |\n", student.national_code);
        printf("|field         |%20s |\n", student.field);
        printf("|entrance year |%20s |\n", student.entrance_year);
        printf("|section       |%20s |\n", student.section);
        printf("|mentor        |%20s |\n", student.mentor);
        printf("|department    |%20s |\n", student.department);
        printf("|GPA           |%20d |\n", gpa);
        int opt;
        printf("1. Go to semester\n2. Go back\nEnter an option: ");
        scanf("%d", &opt);
        if (opt == 1){
                system("cls");
            int enrolled = 0, passed = 0, units = 0, failed = 0;
            float sum = 0;
            int semester;
            printf("Enter semester number: ");
            scanf("%d", &semester);
            printf("Report card -%s %s- %d\n", student.first_name, student.last_name, semester);
            printf("| course name | course id | units | grade | passed | instructor's name |\n");
            printf("|-------------|-----------|-------|-------|--------|-------------------|\n");
            for (int i = 0; i < student.num_offering; i++){
                if (student.reports[i].offering.semester == semester){
                    enrolled++;

                    printf("| %s | %s | %d | %.2f | %s | %s %s |\n",
                    student.reports[i].offering.course.course_name,
                    student.reports[i].offering.course.course_id,
                    student.reports[i].offering.course.units, student.reports[i].grade,
                    (student.reports[i].grade >= 10 ? "Yes" : "No"),
                    student.reports[i].offering.faculty.first_name,
                    student.reports[i].offering.faculty.last_name);

                    if (student.reports[i].grade == -1) continue;
                    sum += student.reports[i].offering.course.units * student.reports[i].grade;
                    units += student.reports[i].offering.course.units;
                    if (student.reports[i].grade >= 10) passed++;
                    else failed++;
                }
            }
            float gpaSemester;
            if (enrolled == 0) gpaSemester = 0;
            else if (passed == 0 && failed == 0) gpaSemester = 0;
            else gpaSemester = sum/units;
            printf("Enrolled courses: %d\n", enrolled);
            printf("Passed courses: %d\n", passed);
            printf("Failed courses: %d\n", failed);
            printf("GPA: %.2f\n", gpaSemester);
            printf("Press any key to go back...");
            getchar();
            getchar();
            return;
        } else return;
    }
}



void printStudent(Student current){
    printf("| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |\n",
    current.first_name, current.last_name, current.student_id,
    current.national_code, current.field, current.entrance_year,
    current.section, current.mentor, current.department,
    current.answer1, current.answer2, current.answer3);
}

void studentsSearch(FILE* file){
    int opt, found = 0;
    char phrase[50];
    Student current;
    system("cls");
    rewind(file);
    printf("1. Search by first name\n");
    printf("2. Search by last name\n");
    printf("3. Search by student id\n");
    printf("4. Search by national code\n");
    printf("5. Search by field\n");
    printf("6. Search by entrance year\n");
    printf("7. Search by section\n");
    printf("8. Search by mentor\n");
    printf("9. Search by department\n");
    printf("Enter an option: ");
    scanf("%d", &opt);

    printf("The phrase to search: ");
    scanf("%s", phrase);

    while(fread(&current, sizeof(Student), 1, file)){
        switch(opt){
            case 1:
                if (strcmp(current.first_name, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 2:
                if (strcmp(current.last_name, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 3:
                if (strcmp(current.student_id, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 4:
                if (strcmp(current.national_code, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 5:
                if (strcmp(current.field, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 6:
                if (strcmp(current.entrance_year, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 7:
                if (strcmp(current.section, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 8:
                if (strcmp(current.mentor, phrase) == 0) {printStudent(current); found = 1;}
                break;
            case 9:
                if (strcmp(current.department, phrase) == 0) {printStudent(current); found = 1;}
                break;
        }
    }
    if (found == 0) printf("No students found.");
    getchar();
    getchar();
    return;
}

void studentsList(){
    int opt;
    FILE* file = fopen("students.json", "rb");
    Student current;
    while(1){
            system("cls");
        printf("Students list\n");
        printf("|first name |last name |student id |national code |field |entrance year \n");
        printf("|section |mentor |department |answer 1 |answer 2 |answer 3|\n");
        printf("|-----------|----------|-----------|--------------|------|--------------\n");
        printf("|--------|-------|----------|----------|----------|---------|\n");

        rewind(file);
        while(fread(&current, sizeof(Student), 1, file)) printStudent(current);

        printf("1. search\n2. go back\nEnter an option: ");
        scanf("%d", &opt);

        if (opt == 1) studentsSearch(file);
        else break;
    }
    fclose(file);
}

void registerStudent(){
    system("cls");
    FILE* file = fopen("students.json", "ab");
    int opt;
    Student new_student;
    printf("Register student(s)\n");
    printf("1. Register one student\n");
    printf("2. Register a group of students\n");
    printf("Enter an option: ");
    scanf("%d", &opt);
    if (opt == 1){

        printf("Enter first name: ");
        scanf("%s", new_student.first_name);
        printf("Enter last name: ");
        scanf("%s", new_student.last_name);
        printf("Enter student id: ");
        scanf("%s", new_student.student_id);
        printf("Enter national code: ");
        scanf("%s", new_student.national_code);
        printf("Enter field: ");
        scanf("%s", new_student.field);
        printf("Enter entrance year: ");
        scanf("%s", new_student.entrance_year);
        printf("Enter section: ");
        scanf("%s", new_student.section);
        printf("Enter mentor: ");
        scanf("%s", new_student.mentor);
        printf("Enter department: ");
        scanf("%s", new_student.department);
        printf("Enter answer 1: ");
        scanf("%s", new_student.answer1);
        printf("Enter answer 2: ");
        scanf("%s", new_student.answer2);
        printf("Enter answer 3: ");
        scanf("%s", new_student.answer3);
        printf("Enter password: ");
        scanf("%s", new_student.password);

        new_student.num_offering = 0;
        fwrite(&new_student, sizeof(Student), 1, file);
        printf("Registered student successfully.\n");
        printf("press any key to go back.");
        getchar();
        getchar();
    } else {
        char file_name[30];
        printf("Enter file name: ");
        scanf("%s", file_name);
        FILE* imported_file = fopen(file_name, "rb");
        if (imported_file == NULL) {
                fclose(file);
                printf("File not found.\n");
                printf("press any key to go back.");
                getchar();
                getchar();
                return;
        }
        while(fread(&new_student, sizeof(Student),1, imported_file)){
            fwrite(&new_student, sizeof(Student), 1, file);
        }
        printf("Registered students successfully.\n");
        printf("press any key to go back.");
        fclose(imported_file);
        getchar();
        getchar();
    }
    fclose(file);
}

void removeStudent(){
    system("cls");
    printf("Remove student(s)\n");
    printf("Enter username: ");

    char username[ID_LEN];
    scanf("%s", username);

    FILE* file = fopen("students.json", "rb");
    int count = 0;
    Student temp;
    while(fread(&temp, sizeof(Student), 1, file)) count++;

    Student* updated_list = malloc(sizeof(Student) * count);
    int found = 0;
    count = 0;
    rewind(file);

    while(fread(&updated_list[count], sizeof(Student), 1, file)){
        if(strcmp(updated_list[count].student_id, username) == 0){
            printStudent(updated_list[count]);
            char input;
            found = 1;
            printf("Remove student?[y/n] ");
            while(1){
                scanf("%c", &input);
                if (input == 'n') {fclose(file);return;}
                else if (input == 'y') break;
            }
        } else count ++;
    }
    fclose(file);

    if (found == 1){
        FILE* file = fopen("students.json", "wb");
        fwrite(updated_list, sizeof(Student), count, file);
        printf("Student removed successfully.");
        fclose(file);
    }
    else printf("User not found.");
    getchar();
    getchar();
    free(updated_list);
    return;
}
