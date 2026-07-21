#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consts.h"
#include "student.h"
#include "faculty.h"
#include "admin.h"

int facultyLogin(){
    char username[ID_LEN];
    FILE* file = fopen("faculty.json", "rb");
    Faculty current;
    int pos = 0;
    while (1){
            system("cls");
        printf("Enter your username: ");
        scanf("%s", username);

        int found = 0;
        rewind(file);
        while (fread(&current, sizeof(Faculty), 1, file)){
            if (strcmp(current.faculty_id, username) == 0){
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
            printf("Username wrong.");
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
    adminDashboard();
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
        scanf("%d", &option);
        if (option == 1) continue;
        if (option == 2) return -2;
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
        scanf("%d", &option);
        if (option == 1) continue;
        if (option == 2) return -2;
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
        scanf("%d", &option);
        if (option == 1) continue;
        if (option == 2) return -2;
    }
    return 1;
}

void forgotPassword(){
    int pos, auth = 0;
    while(1){
        system("cls");

        printf("Enter your username: ");
        char username[ID_LEN];
        scanf("%s", username);
        Student current;
        pos = 0;
        FILE* studentFile = fopen("students.json", "rb+");
        while (fread(&current, sizeof(Student), 1, studentFile)){
            if (strcmp(current.student_id, username) == 0){
                auth = authentication(current);
                break;
            }
            pos++;
        }
        fclose(studentFile);
        if (auth == -2) return;
        if (auth == 0){
            printf("User not found.\n");
            printf("1. Retry\n");
            printf("2. Go to login menu\n");
            printf("Enter an option: ");
            int input;
            scanf("%d", &input);
            if (input == 1) continue;
            if (input == 2) return;
        }
        else if (auth == 1){
            while(1){
                char new_password[PASSWORD_LEN];
                char confirm[PASSWORD_LEN];
                printf("Enter your new password: ");
                scanf("%s", new_password);
                printf("Confirm your password: ");
                scanf("%s", confirm);
                if (strcmp(new_password, confirm) == 0){
                    Student current;
                    FILE* file = fopen("students.json", "rb+");
                    strcpy(current.password, new_password);
                    fseek(file, pos * sizeof(Student), SEEK_SET);
                    fwrite(&current, sizeof(Student), 1, file);
                    fclose(file);
                    printf("Password changed successfully.\n");
                    printf("Press any key to go to login menu...\n");
                    getchar();
                    getchar();
                    return;
                }
                else {
                    int opt;
                    printf("Passwords do not match.\n");
                    printf("1. Retry.\n2. Cancel (go to login menu).\n");
                    printf("Enter an option: ");
                    scanf("%d", &opt);
                    if (opt == 1) continue;
                    if (opt == 2) return;
                }
            }
        }
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
        int pos;
        switch(input){
            case 1:
                pos = studentLogin();
                if (pos == -1) continue;
                studentDashboard(pos);
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
                printf("Invalid number. ");
                getchar();
                getchar();
        }
    }
}
