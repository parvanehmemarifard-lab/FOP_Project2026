#include <stdlib.h>
#include <stdio.h>
#include "admin.h"
#include "course.h"

int calender[4] = {0, 0, 0, 0};
Request requests[100];

void adminCalender(){
    int opt;
    while(1){
        system("cls");
        printf("1. offering:        %s\n", (calender[0] ? "enabled" : "disabled"));
        printf("2. unit selection:  %s\n", (calender[1] ? "enabled" : "disabled"));
        printf("3. class & exams:   %s\n", (calender[2] ? "enabled" : "disable"));
        printf("4. grade recording: %s\n", (calender[3] ? "enabled" : "disabled"));
        printf("5. go to main menu\nEnter a time to trigger: ");
        scanf("%d", &opt);
        switch(opt){
            case 1:
                calender[0] = (calender[0] ? 0 : 1);
                break;
            case 2:
                if(calender[0] == calender[1]){
                    printf("offering should be triggered first.");
                    getchar();
                    getchar();
                } else calender[1] = (calender[1] ? 0 : 1);
                break;
            case 3:
                if(calender[1] == calender[2]){
                    printf("unit selection should be triggered first.");
                    getchar();
                    getchar();
                } else calender[2] = (calender[2] ? 0 : 1);
                break;
            case 4:
                if(calender[2] == calender[3]){
                    printf("class & exams should be triggered first");
                    getchar();
                    getchar();
                } else calender[3] = (calender[3] ? 0 : 1);
                break;
            case 5:
                return;
        }
    }
}

void adminStudents(){
    int opt;
    while(1){
        system("cls");
        printf("1. Student list\n2. Register student(s)\n");
        printf("3. Remove student(s)\n4. Go back\n");
        printf("Enter an option: ");
        scanf("%d", &opt);
        switch(opt){
            case 1:
                //studentsList();
                break;
            case 2:
                //registerStudent();
                break;
            case 3:
                //deleteStudent();
                break;
            case 4:
                return;
        }
    }
}

void adminFaculty(){}

void adminRequests(){}

void adminOfferings(){}

void adminDashboard(){
    int opt;
    while(1){
            system("cls");
        printf("Welcome admin\n");
        printf("1. Calender\n2. Students\n");
        printf("3. Faculty members\n4. Requests\n");
        printf("5. Offering\n6. Courses\n");
        printf("7. Log out\nEnter an option: ");
        scanf("%d", &opt);
        switch(opt){
            case 1:
                adminCalender();
                break;
            case 2:
                adminStudents();
                break;
            case 3:
                adminFaculty();
                break;
            case 4:
                adminRequests();
                break;
            case 5:
                adminOfferings();
                break;
            case 6:
                 coursesListAdmin();
                 break;
            case 7:
                return;
        }
    }
}
