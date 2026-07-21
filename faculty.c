#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faculty.h"
#include "course.h"

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

void facultyDashboard(int pos){
    FILE* file = fopen("faculty.json", "rb");
    fseek(file, pos * sizeof(Faculty), SEEK_SET);
    Faculty faculty;
    fread(&faculty, sizeof(Faculty), 1, file);
    fclose(file);

    int option;
    while(1){
            system("cls");
        printf("Welcome %s %s", faculty.first_name, faculty.last_name);
        printf("1. My offerings\n2. List of offerings in semester\n");
        printf("3. List of courses\n4. Offer a course \n5. Log out\n");
        printf("Enter an option: ");
        scanf("%d", &option);
        switch(option){
            case 1:
                //
                break;
            case 2:
                //
                break;
            case 3:
                coursesListGeneral();
                break;
            case 4:
                //
                break;
            case 5:
                return;
        }
    }
}
