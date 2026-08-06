#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "course.h"
#include "student.h"
#include "faculty.h"
#include "offering.h"

int calender[4] = {0, 0, 0, 0};
Request* requests[100];
int request_count = 0;


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
                studentsList();
                break;
            case 2:
                registerStudent();
                break;
            case 3:
                removeStudent();
                break;
            case 4:
                return;
        }
    }
}

void adminFaculty(){
    int opt;
    while(1){
        system("cls");
        printf("1. Faculty list\n2. Register Faculty(s)\n");
        printf("3. Remove Faculty(s)\n4. Go back\n");
        printf("Enter an option: ");
        scanf("%d", &opt);
        switch(opt){
            case 1:
                facultyList();
                break;
            case 2:
                registerFaculty();
                break;
            case 3:
                removeFaculty();
                break;
            case 4:
                return;
        }
    }
}

void adminOfferings(){
    system("cls");
    FILE* file = fopen("offerings.json", "rb");
    Offering current;
    int semester;
    printf("Enter semester number: ");
    scanf("%d", &semester);
    while(1){
        system("cls");
        printf("Enter semester number: %d\n", semester);
        rewind(file);
        int opt, num = 0;

        printf("List of offerings - %d\n", semester);
        printf("| number | course name | course id | faculty id | semester");
        printf(" | capacity | no. enrollments | department | place |\n");
        printf("|--------|-------------|-----------|------------|----------|");
        printf("----------|-----------------|------------|-------|\n");

        while(fread(&current, sizeof(Offering), 1, file)){
            if (current.semester == semester) printOfferingAdmin(current, ++num);
        }

        printf("1. Search\n");
        printf("2. Add student to an offering\n");
        printf("3. Remove student from an offering\n");
        printf("4. Go back\n");
        printf("Enter an option: ");
        scanf("%d", &opt);

        if (opt == 1) searchOffering(semester);
        else if (opt == 4) {fclose(file); return;}
        else {
            int input, found = 0;
            num = 1;
            printf("Enter number of offering: ");
            scanf("%d", &input);
            rewind(file);
            while(fread(&current, sizeof(Offering), 1, file)){
                if (current.semester == semester){
                    if (num == input) {found = 1; break;}
                    num++;
                }
            }

            if (found == 0) {printf("Offering not found."); getchar(); getchar();}
            else if (opt == 2) addStudentToOffering(current);
            else removeStudentFromOffering(current);
        }
    }
    fclose(file);
}



int addNewOffering(Offering offering){
    FILE* file = fopen("offerings.json", "ab");
    fwrite(&offering, sizeof(Offering), 1, file);
    fclose(file);
    return 1;
}

int removeOffering(Offering offering){
    Offering temp;
    FILE* file = fopen("offerings.json", "rb");
    int count = 0;
    while(fread(&temp, sizeof(Offering), 1, file)) count++;
    Offering* updated_list = malloc(sizeof(Offering) * count);
    int found = 0;
    count = 0;
    rewind(file);
    while(fread(&updated_list[count], sizeof(Offering), 1, file)){
        if (updated_list[count].semester == offering.semester &&
            strcmp(updated_list[count].course.course_id, offering.course.course_id) == 0 &&
            strcmp(updated_list[count].faculty.faculty_id, offering.faculty.faculty_id) == 0) found = 1;
        else count++;
    }
    fclose(file);

    if (found == 1){
        FILE* file = fopen("offerings.json", "wb");
        fwrite(updated_list, sizeof(Offering), count, file);
        printf("Offering removed successfully.");
        getchar();
        getchar();
        free(updated_list);
        fclose(file);
        return 1;
    }
    else printf("Offering not found.");
    free(updated_list);
    getchar();
    getchar();
    return 0;
}

int increaseCapacity(Offering offering, int new_capacity){
    FILE* file = fopen("offerings.json", "rb+");
    Offering current;
    int pos = 0, found = 0;
    while(fread(&current, sizeof(Offering), 1, file)){
        if (current.semester == offering.semester &&
            strcmp(current.course.course_id, offering.course.course_id) == 0 &&
            strcmp(current.faculty.faculty_id, offering.faculty.faculty_id) == 0){
                found = 1;
                break;
        }
        pos++;
    }

    if (found == 0){
        printf("Offering not found.");
        getchar();
        getchar();
        fclose(file);
        return 0;
    } else {
        current.capacity = new_capacity;
        fseek(file, pos * sizeof(Offering), SEEK_SET);
        fwrite(&current, sizeof(Offering), 1, file);
        fclose(file);
        return 1;
    }
}

void adminRequests(){
    while(1){
            system("cls");
        printf("List of requests\n");
        if (request_count == 0){printf("No requests."); getchar(); getchar(); return;}

        for (int i = 0; i < request_count; i++){
            if (requests[i]->type == 1){
                printf("%d. course offering\n", (i+1));
                printf("\tCourse: %s\n", requests[i]->offering.course.course_name);
                printf("\tFaculty: %s\n", requests[i]->offering.faculty.faculty_id);
                printf("\tDepartment: %s\n", requests[i]->offering.course.department);
                printf("\tCapacity:  %d\n", requests[i]->offering.capacity);
            } else if(requests[i]->type == 2){
                printf("%d. course removing\n", (i+1));
                printf("\tCourse: %s\n", requests[i]->offering.course.course_name);
                printf("\tFaculty: %s\n", requests[i]->offering.faculty.faculty_id);
                printf("\tDepartment: %s\n", requests[i]->offering.course.department);
                printf("\tCapacity:  %d\n", requests[i]->offering.capacity);
            } else {
                printf("%d.  capacity increasement\n", (i+1));
                printf("\tCourse: %s\n", requests[i]->offering.course.course_name);
                printf("\tFaculty: %s\n", requests[i]->offering.faculty.faculty_id);
                printf("\tDepartment: %s\n", requests[i]->offering.course.department);
                printf("\tCapacity:  %d\n", requests[i]->new_capacity);
                printf("\tNo. enrollments: %d\n", requests[i]->offering.enrollments);
            }
        }
        printf("1. Go to request number\n");
        printf("2. Go back\nEnter an option: ");
        int opt;
        scanf("%d", &opt);
        if (opt == 2) return;

        printf("Enter the request number: ");
        int num;
        scanf("%d", &num);
        if (num > request_count) printf("Invalid number.");
        else {
            int done;
            switch(requests[num-1]->type){
                case 1:
                    done = addNewOffering(requests[num-1]->offering);
                    break;
                case 2:
                    done = removeOffering(requests[num-1]->offering);
                    break;
                case 3:
                    done = increaseCapacity(requests[num-1]->offering, requests[num-1]->new_capacity);
                    break;
            }
            if (done == 1){
                for (int i = num - 1; i < request_count - 1; i++){
                    requests[i] = requests[i + 1];
                }
                request_count--;
            }
        }
    }
}
