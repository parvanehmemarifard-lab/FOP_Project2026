#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faculty.h"
#include "course.h"
#include "admin.h"

void facultyDashboard(int pos){
    FILE* file = fopen("faculty.json", "rb");
    fseek(file, pos * sizeof(Faculty), SEEK_SET);
    Faculty faculty;
    fread(&faculty, sizeof(Faculty), 1, file);
    fclose(file);

    int option;
    while(1){
            system("cls");
        printf("Welcome %s %s\n", faculty.first_name, faculty.last_name);
        printf("1. My offerings\n");
        printf("2. List of offerings in semester\n");
        printf("3. List of courses\n");
        printf("4. Offer a course \n");
        printf("5. Log out\n");
        printf("Enter an option: ");
        scanf("%d", &option);
        switch(option){
            case 1:
                facultyMyOfferings(faculty);
                break;
            case 2:
                searchOffering(-1);
                break;
            case 3:
                coursesListGeneral();
                break;
            case 4:
                if (calender[0] == 0) {printf("Offering is disabled."); getchar(); getchar();}
                else offerCourse(faculty);
                break;
            case 5:
                return;
        }
    }
}


void offerCourse(Faculty faculty){
    FILE* file = fopen("courses.json", "rb");
    Course current;
    char course_id[COURSE_ID_LEN];
    printf("Enter course id: ");
    scanf("%s", course_id);

    while(fread(&current, sizeof(Course), 1, file)){
        if (strcmp(course_id, current.course_id) == 0){
            printCourse(current);
            int capacity, semester;
            char place[PLACE_LEN];
            printf("Enter capacity: ");
            scanf("%d", &capacity);
            printf("Enter semester: ");
            scanf("%d", &semester);
            printf("Enter place: ");
            scanf("%s", place);

            Offering new_offering;
            new_offering.capacity = capacity;
            new_offering.course = current;
            new_offering.enrollments = 0;
            new_offering.faculty = faculty;
            strcpy(new_offering.place, place);
            new_offering.semester = semester;

            Request* new_request = malloc(sizeof(Request));
            new_request->new_capacity = capacity;
            new_request->offering = new_offering;
            new_request->type = 1;

            requests[request_count] = new_request;
            request_count++;
            fclose(file);
            return;
        }
    }

    printf("Course not found.");
    getchar();
    getchar();
    fclose(file);
    return;
}


void printFaculty(Faculty current){
    printf("| %s | %s | %s | %s | %s | %s | %s |\n",
    current.first_name, current.last_name, current.faculty_id,
    current.national_code, current.field, current.entrance_year,
    current.section);
}


void facultySearch(FILE* file){
    int opt;
    char phrase[50];
    Faculty current;
    system("cls");
    rewind(file);
    printf("1. Search by first name\n");
    printf("2. Search by last name\n");
    printf("3. Search by faculty id\n");
    printf("4. Search by national code\n");
    printf("5. Search by field\n");
    printf("6. Search by entrance year\n");
    printf("7. Search by section\n");
    printf("Enter an option: ");
    scanf("%d", &opt);

    printf("The phrase to search: ");
    scanf("%s", phrase);

    while(fread(&current, sizeof(Faculty), 1, file)){
        switch(opt){
            case 1:
                if (strcmp(current.first_name, phrase) == 0) printFaculty(current);
                break;
            case 2:
                if (strcmp(current.last_name, phrase) == 0) printFaculty(current);
                break;
            case 3:
                if (strcmp(current.faculty_id, phrase) == 0) printFaculty(current);
                break;
            case 4:
                if (strcmp(current.national_code, phrase) == 0) printFaculty(current);
                break;
            case 5:
                if (strcmp(current.field, phrase) == 0) printFaculty(current);
                break;
            case 6:
                if (strcmp(current.entrance_year, phrase) == 0) printFaculty(current);
                break;
            case 7:
                if (strcmp(current.section, phrase) == 0) printFaculty(current);
                break;
        }
    }
    getchar();
    getchar();
    return;
}


void facultyList(){
    int opt;
    FILE* file = fopen("faculty.json", "rb");
    Faculty current;
    while(1){
            system("cls");
        printf("Faculty list\n");
        printf("|first name |last name |faculty id |national code ");
        printf("|field |entrance year | section|\n");
        printf("|-----------|----------|-----------|--------------");
        printf("|------|--------------|--------|\n");

        rewind(file);
        while(fread(&current, sizeof(Faculty), 1, file)) printFaculty(current);

        printf("1. search\n2. go back\nEnter an option: ");
        scanf("%d", &opt);

        if (opt == 1) facultySearch(file);
        else break;
    }
    fclose(file);
}


void registerFaculty(){
    system("cls");
    FILE* file = fopen("faculty.json", "ab");
    int opt;
    Faculty new_faculty;
    printf("Register faculty(s)\n");
    printf("1. Register one faculty\n");
    printf("2. Register a group of faculties\n");
    printf("Enter an option: ");
    scanf("%d", &opt);
    if (opt == 1){

        printf("Enter first name: ");
        scanf("%s", new_faculty.first_name);
        printf("Enter last name: ");
        scanf("%s", new_faculty.last_name);
        printf("Enter faculty id: ");
        scanf("%s", new_faculty.faculty_id);
        printf("Enter national code: ");
        scanf("%s", new_faculty.national_code);
        printf("Enter field: ");
        scanf("%s", new_faculty.field);
        printf("Enter entrance year: ");
        scanf("%s", new_faculty.entrance_year);
        printf("Enter section: ");
        scanf("%s", new_faculty.section);
        printf("Enter password: ");
        scanf("%s", new_faculty.password);

        fwrite(&new_faculty, sizeof(Faculty), 1, file);
        printf("Registered faculty successfully.\n");
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
        while(fread(&new_faculty, sizeof(Faculty),1, imported_file)){
            fwrite(&new_faculty, sizeof(Faculty), 1, file);
        }
        printf("Registered faculties successfully.\n");
        printf("press any key to go back.");
        fclose(imported_file);
        getchar();
        getchar();
    }
    fclose(file);
}


void removeFaculty(){
    system("cls");
    printf("Remove faculty\n");
    printf("Enter username: ");

    char username[ID_LEN];
    scanf("%s", username);

    FILE* file = fopen("faculty.json", "rb");
    Faculty updated_list[10];
    int count = 0, found = 0;

    while(fread(&updated_list[count], sizeof(Faculty), 1, file)){
        if(strcmp(updated_list[count].faculty_id, username) == 0){
            printFaculty(updated_list[count]);
            char input;
            found = 1;
            printf("Remove faculty?[y/n] ");
            while(1){
                scanf("%c", &input);
                if (input == 'n') {fclose(file); return;}
                else if (input == 'y') break;
            }
        } else count ++;
    }
    fclose(file);

    if (found == 1){
        FILE* file = fopen("faculty.json", "wb");
        fwrite(updated_list, sizeof(Faculty), count, file);
        printf("Faculty removed successfully.");
        fclose(file);
    }
    else printf("User not found.");
    getchar();
    getchar();
    return;
}


void searchMyOfferings(Faculty faculty){
    system("cls");
    int opt, num = 1, found = 0;
    char phrase[50];
    printf("1. Search by course name\n");
    printf("2. Search by course id\n");
    printf("3. Search by department\n");
    printf("Enter an option: ");
    scanf("%d", &opt);
    printf("Enter the phrase: ");
    scanf("%s", phrase);

    FILE* file = fopen("offerings.json", "rb");
    Offering current;
    while(fread(&current, sizeof(Offering), 1, file)){
        switch(opt){
            case 1:
                if(strcmp(phrase, current.course.course_name) == 0 &&
                   strcmp(current.faculty.faculty_id, faculty.faculty_id)) {printOfferingAdmin(current, num++); found = 1;}
                break;
            case 2:
                if(strcmp(phrase, current.course.course_id) == 0 &&
                   strcmp(current.faculty.faculty_id, faculty.faculty_id)) {printOfferingAdmin(current, num++); found = 1;}
                break;
            case 3:
                if(strcmp(phrase, current.course.department) == 0 &&
                   strcmp(current.faculty.faculty_id, faculty.faculty_id)) {printOfferingAdmin(current, num++); found = 1;}
                break;
        }
    }
    if (found == 0) printf("No offerings found.");
    fclose(file);
    getchar();
    getchar();
}


void goToOffering(Faculty faculty){
    FILE* file = fopen("offerings.json", "rb");
    Offering current;
    int num = 1, opt, input, found = 0;
    printf("Enter offering number: ");
    scanf("%d", &input);

    while(fread(&current, sizeof(Offering), 1, file)){
        if (strcmp(current.faculty.faculty_id, faculty.faculty_id) == 0){
            if (num == input) {
                printOfferingAdmin(current, num);
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

    printf("1. Add capacity\n");
    printf("2. Record grades\n");
    printf("3. Remove offering\n");
    //printf("4. Publish a homework\n");
    //printf("5. Publish an exam\n");
    printf("4. Go back\n");
    printf("Enter an option: ");
    scanf("%d", &opt);

    if (opt == 1){
        int new_capacity;
        printf("Enter new capacity: ");
        scanf("%d", &new_capacity);
        Request* new_request = malloc(sizeof(Request));
        new_request->new_capacity = new_capacity;
        new_request->offering = current;
        new_request->type = 3;
        requests[request_count] = new_request;
        request_count++;
        printf("Request sent to admin.");
        getchar();
        getchar();
    } else if(opt == 2) {
        if (calender[3] == 0) {
            printf("Recording grades is disabled.\n");
            printf("Press any key to go back.\n");
            getchar();
            getchar();
        }
        else recordGrades(current);
    } else if (opt == 3){
        if (calender[0] == 0) {
            printf("Offering is disabled.\n");
            printf("Press any key to go back.\n");
            getchar();
            getchar();
        } else {
            Request* new_request = malloc(sizeof(Request));
            new_request->type = 2;
            new_request->offering = current;
            new_request->new_capacity = 0;
            requests[request_count] = new_request;
            request_count++;
            printf("Request sent to admin.");
            getchar();
            getchar();
        }
    } else return;
}


void facultyMyOfferings(Faculty faculty){

    FILE* file = fopen("offerings.json", "rb");
    Offering current;
    int num, opt;
    while(1){
            system("cls");
        num = 1;
        rewind(file);
        printf("List of my offerings\n");
        printf("| number | course name | course id | faculty id | ");
        printf("semester | capacity | no. enrollments | department | place |\n");
        printf("|--------|-------------|-----------|------------|-");
        printf("---------|----------|-----------------|------------|-------|\n");

        while(fread(&current, sizeof(Offering), 1, file)){
            if (strcmp(current.faculty.faculty_id, faculty.faculty_id) == 0){
                printOfferingAdmin(current, num);
                num++;
            }
        }

        printf("1. Go to offering\n");
        printf("2. Search\n");
        printf("3. Go back\n");
        printf("Enter an option: ");
        scanf("%d", &opt);

        if (opt == 1) goToOffering(faculty);
        else if (opt == 2) searchMyOfferings(faculty);
        else {
            fclose(file);
            return;
        }
    }
}
