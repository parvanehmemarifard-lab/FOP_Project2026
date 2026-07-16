#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "consts.h"
#include "offering.h"

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

void offeringListStudent(Student student){
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
    printf("1. Search\n2. Enroll in course\n");
    printf("3. Withdraw course\n4. Go back\n");
    printf("Enter an option:  ");
    scanf("%d", &option);
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
                //offeringListStudent(student);
                break;
            case 2:
                //coursesListGeneral();
                break;
            case 3:
                //reportCardStudent(student);
                break;
            case 4:
                return;
        }
    }
}

