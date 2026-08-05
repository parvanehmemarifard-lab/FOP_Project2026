#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "course.h"

void printCourse(Course current){
    printf("| %s | %s | %d | ",
    current.course_name, current.course_id, current.units);
    if (current.num_prequisites == 0) printf("-");
    else {
        for (int i = 0; i < current.num_prequisites; i++){
            if(i) printf(",");
            printf("%s", current.prerequisites[i]);
        }
    }
    printf(" | %s | %s | %s |\n", current.section,
    current.field, current.department);
}

void searchCourse(){
    FILE* file = fopen("courses.json", "rb");
    Course current;
    system("cls");
    int opt, found = 0;
    char phrase[50];
    printf("1. Search by course name\n2. Search by ID\n");
    printf("3. Search by section\n4. Search by field\n");
    printf("5. Search by department\nEnter an option: ");
    scanf("%d", &opt);
    printf("The phrase to search: ");
    scanf("%s", phrase);
    while (fread(&current, sizeof(Course), 1, file)){
        switch(opt){
            case 1:
                if (strcmp(current.course_name, phrase) == 0) {printCourse(current); found = 1;}
                break;
            case 2:
                if (strcmp(current.course_id, phrase) == 0) {printCourse(current); found = 1;}
                break;
            case 3:
                if (strcmp(current.section, phrase) == 0) {printCourse(current); found = 1;}
                break;
            case 4:
                if (strcmp(current.field, phrase) == 0) {printCourse(current); found = 1;}
                break;
            case 5:
                if (strcmp(current.department, phrase) == 0) {printCourse(current); found = 1;}
                break;
        }
    }
    if (found == 0) printf("No courses found.");
    getchar();
    getchar();
}

void coursesListGeneral(){

    FILE* file = fopen("courses.json", "rb");
    Course current;
    int opt;
    while(1){
        system("cls");
        rewind(file);
        printf("List of courses\n| course name | course id | units | prerequisites |");
        printf(" section | field | department |\n|-------------|-----------|-------|");
        printf("---------------|---------|-------|------------|\n");

        while(fread(&current, sizeof(Course), 1, file)) printCourse(current);
        printf("1. Search\n2. Go back\nEnter an option: ");
        scanf("%d", &opt);
        if (opt == 1) searchCourse();
        else break;
    }
    fclose(file);
}

void addCourse(){
    system("cls");
    FILE* file = fopen("courses.json", "ab");
    int opt;
    Course new_course;
    printf("Add course(s)\n");
    printf("1. Add one course\n");
    printf("2. Add a group of courses\n");
    printf("Enter an option: ");
    scanf("%d", &opt);
    if (opt == 1){
        printf("Enter course name: ");
        scanf("%s", new_course.course_name);
        printf("Enter course id: ");
        scanf("%s", new_course.course_id);
        printf("Enter field: ");
        scanf("%s", new_course.field);
        printf("Enter section: ");
        scanf("%s", new_course.section);
        printf("Enter department: ");
        scanf("%s", new_course.department);
        printf("Enter units: ");
        scanf("%d", &new_course.units);

        printf("Enter number of prerequisities: ");
        scanf("%d", &new_course.num_prequisites);
        if (new_course.num_prequisites) printf("Enter id of prerequisity courses: ");
        int i = 0;
        while(i < new_course.num_prequisites){
            scanf("%s", new_course.prerequisites[i]);
            i++;
        }
        fwrite(&new_course, sizeof(Course), 1, file);
        printf("Added course successfully.\n");
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
        while(fread(&new_course, sizeof(Course),1, imported_file)){
            fwrite(&new_course, sizeof(Course), 1, file);
        }
        printf("Added courses successfully.\n");
        printf("press any key to go back.");
        fclose(imported_file);
        getchar();
        getchar();
    }
    fclose(file);
}

void removeCourse(){
    system("cls");
    printf("Remove course(s)\n");
    printf("Enter course ID: ");

    char id[ID_LEN];
    scanf("%s", id);

    FILE* file = fopen("courses.json", "rb");
    Course updated_list[10];
    int count = 0, found = 0;

    while(fread(&updated_list[count], sizeof(Course), 1, file)){
        if(strcmp(updated_list[count].course_id, id) == 0){
            printCourse(updated_list[count]);
            char input;
            found = 1;
            printf("Remove course?[y/n] ");
            while(1){
                scanf("%c", &input);
                if (input == 'n') {fclose(file); return;}
                else if (input == 'y') break;
            }
        } else count++;
    }
    fclose(file);

    if (found == 1){
        FILE* file = fopen("courses.json", "wb");
        fwrite(updated_list, sizeof(Course), count, file);
        printf("Course removed successfully.");
        fclose(file);
    }
    else printf("Course not found.");
    getchar();
    getchar();
    return;
}

void coursesListAdmin(){
    FILE* file = fopen("courses.json", "rb");
    Course current;
    int opt;
    while(1){
        system("cls");
        rewind(file);

        printf("List of courses\n| course name | course id | units | prerequisites |");
        printf(" section | field | department |\n|-------------|-----------|-------|");
        printf("---------------|---------|-------|------------|\n");

        while(fread(&current, sizeof(Course), 1, file)) printCourse(current);
        printf("1. Search\n2. Add a course\n");
        printf("3. Remove a course\n4. Go back\n");
        printf("Enter an option: ");
        scanf("%d", &opt);
        switch(opt){
            case 1:
                searchCourse();
                break;
            case 2:
                addCourse();
                break;
            case 3:
                removeCourse();
                break;
            case 4:
                return;
        }
    }
    fclose(file);
}
