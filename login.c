#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consts.h"
#include "student.h"

void adminLogin(){
    const char ADMIN_USERNAME[] = "admin";
    const char ADMIN_PASSWORD[] = "password";
    char username[20];
    char password[20];
    while(1){
        system("cls");
        printf("Enter your username: ");
        scanf("%s", username);
        if (strcmp(username, ADMIN_USERNAME)) {
            printf("Username not found.");
            getchar();
            getchar();
        }
        else break;
    }

    while(1){
        system("cls");
        printf("Enter your username: %s\n", username);
        printf("Enter password: ");
        scanf("%s", password);
        if (strcmp(password, ADMIN_PASSWORD)) {
            printf("Incorrect password.");
            getchar();
            getchar();
        } else break;
    }
}

int authentication(Student stu){
    char answer1[CITY_LEN];
    char answer2[BOOK_LEN];
    char answer3[COLOR_LEN];

    while(1){
        system("cls");
        printf("Enter your username: %s\n", stu.student_id);
        printf("Where were you born? ");
        scanf("%s", answer1);
        if (stricmp(answer1, stu.answer1) == 0) break;
        printf("Incorrect answer.\n");
        printf("1. Retry\n");
        printf("2. Go to login menu\n");
        printf("Enter an option: ");

        int option;
        scanf("%d", option);
        if (option == 2) return -1;
    }

    while(1){
        system("cls");
        printf("Enter your username: %s\n", stu.student_id);
        printf("Where were you born? %s\n", answer1);
        printf("What was the title of the first book you read? ");
        scanf("%s", answer2);
        if (stricmp(answer2, stu.answer2) == 0) break;
        printf("Incorrect answer.\n");
        printf("1. Retry\n");
        printf("2. Go to login menu\n");
        printf("Enter an option: ");

        int option;
        scanf("%d", option);
        if (option == 2) return -1;
    }

    while(1){
        system("cls");
        printf("Enter your username: %s\n", stu.student_id);
        printf("Where were you born? %s\n", answer1);
        printf("What was the title of the first book you read? %s\n", answer2);
        printf("What was the color of your first bicycle? ");
        scanf("%s", answer3);
        if (stricmp(answer3, stu.answer3) == 0) break;
        printf("Incorrect answer.\n");
        printf("1. Retry\n");
        printf("2. Go to login menu\n");
        printf("Enter an option: ");

        int option;
        scanf("%d", option);
        if (option == 2) return -1;
    }
    return 1;
}

void forgotPassword(){
    while(1){
        system("cls");

        printf("Enter your username: ");
        char username[ID_LEN];
        scanf("%s", username);

        Student current;
        int pos = 0, found = 0;
        FILE* studentFile = fopen("students.json", "rb+");
        while (fread(&current, sizeof(Student), 1, studentFile)){
            if (strcmp(current.student_id, username) == 0){
                if (authentication(current) == -1) return;
            }
        }
        printf("User not found.\n");
        printf("1. Retry\n");
        printf("2. Go to login menu\n");
        printf("Enter an option: ");
        int input;
        scanf("%d", &input);
        if (input==2) return;
    }
}

void loginPage(){
    int input;
    while(1){
        system("cls");

        printf("1. Login as student\n");
        printf("2. Login as faculty\n");
        printf("3. Login as admin\n");
        printf("4. Forgot password\n");
        printf("5. Exit\n");
        printf("Enter an option: ");
        scanf("%d", &input);

        switch(input){
            case 1:
                //studentLogin();
                break;
            case 2:
                //facultyLogin();
                break;
            case 3:
                adminLogin();
                break;
            case 4:
                forgotPassword();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid number!");
                getchar();
                getchar();
        }
    }
}
