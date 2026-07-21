#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "consts.h"
#include "offering.h"
#include "admin.h"

int studentLogin(){
    char username[ID_LEN];
    FILE* file = fopen("students.json", "rb");
    Student current;
    int pos = 0;
    while (1){
            system("cls");
        printf("Enter your username: ");
        scanf("%s", username);

        int found = 0;
        rewind(file);
        while (fread(&current, sizeof(Student), 1, file)){
            if (strcmp(current.student_id, username) == 0){
                found = 1;
                break;
            }
            pos++;
        }
        if (found == 0){
            int opt;
            printf("Username not found.\n1. Retry\n2. Go back to main menu\n");
            printf("Enter an option: ");
            scanf("%d", &opt);
            if (opt == 1) continue;
            if (opt == 2) {fclose(file); return -1;}
        } else break;
    }

    fclose(file);
    char password[PASSWORD_LEN];
    //system("cls");
    //printf("Enter your username: %s\n", username);
    printf("Enter your password: ");
    scanf("%s", password);
    if (strcmp(current.password, password) == 0){
        printf("Login successful.");
        return pos;
    } else {
        printf("Incorrect password.\n");
        printf("Press any key to go back to main menu.");
        getchar();
        getchar();
        return -1;
    }
}

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

void enrollCourse(Student student, int semester, int pos){
    FILE* file = fopen("offerings.json", "rb");
    Offering current;
    int num = 1, input, found = 0;
    printf("Enter number of offering: ");
    scanf("%d", &input);
    while(fread(&current, sizeof(Offering), 1, file)){
        if (semester == current.semester){
            if (num == input){
                printOfferingStudent(current, num);
                found = 1;
                break;
            }
            num++;
        }
    }
    if (found == 0) {
        printf("Offering not found.");
        getchar();
        getchar();
        return;
    }
    if (calender[1] == 0) {
        printf("unit selection is disabled");
        return;
    }
    if (current.capacity <= current.enrollments) {printf("Capasity is full"); return;}
    if (passedPrerequisities(student, current.course) == 0){
        printf("You have not passed the prerequisities.");
        return;
    }
    Enrolled_offering new_offering = {current, -1, -1};
    student.reports[student.num_offering++] = new_offering;
}

void offeringListStudent(Student student, int pos){
    int semester, option, num = 0;
    printf("Enter semester number: ");
    scanf("%d", &semester);
    printf("List of offerings - %d\n| number | ", semester);
    printf("course name | course id |faculty name | semester | ");
    printf("capacity | no.enrollments | department| place |\n");
    FILE* file = fopen("offerings.json", "rb");
    Offering current;
    while(fread(&current, sizeof(Offering), 1, file)){
        num++;
        if (current.semester == semester) printOfferingStudent(current, num);
    }
    fclose(file);
    printf("1. Search\n2. Enroll in course\n");
    printf("3. Withdraw course\n4. Go back\n");
    printf("Enter an option:  ");
    scanf("%d", &option);
    switch(option){
        case 1:
            //searchOffering();
            break;
        case 2:
            enrollCourse(student, semester, pos);
            break;
        case 3:
            //withdrawCourse(student);
            break;
        case 4:
            return;
    }
}

float calculateGPA(Student student){
    float sum = 0;
    int units = 0;
    for (int i = 0; i < student.num_offering; i++){
        sum += student.reports[i].grade * student.reports[i].offering.course.units;
        units += student.reports[i].offering.course.units;
    }
    return sum/units;
}

void reportCardStudent(Student student){
    int gpa = calculateGPA(student);
    while(1){
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
            int enrolled = 0, passed = 0, units = 0;
            float sum = 0;
            int semester;
            printf("Enter semester number: ");
            scanf("%d", &semester);
            printf("Report card -%s %s- %d", student.first_name, student.last_name, semester);
            printf("| course name | course id | units | grade | passed | instructor's name |\n");
            printf("|-------------|-----------|-------|-------|--------|-------------------|\n");
            for (int i = 0; i < student.num_offering; i++){
                if (student.reports[i].offering.semester == semester){
                    printf("| %s | %s | %d | %d | %s | %s %s |\n",
                    student.reports[i].offering.course.course_name,
                    student.reports[i].offering.course.course_id,
                    student.reports[i].offering.course.units, student.reports[i].grade,
                    (student.reports[i].grade >= 10 ? "Yes" : "No"),
                    student.reports[i].offering.faculty.first_name,
                    student.reports[i].offering.faculty.last_name);
                    enrolled++;
                    sum += student.reports[i].offering.course.units * student.reports[i].grade;
                    units += student.reports[i].offering.course.units;
                    if (student.reports[i].grade >= 10) passed++;
                }
            }
            printf("Enrolled courses: %d\n", enrolled);
            printf("Passed courses: %d\n", passed);
            printf("Failed courses: %d", enrolled - passed);
            printf("GPA: %.2f", sum/units);
            printf("Press any key to go back...");
            return;
        } else return;
    }
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

