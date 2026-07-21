#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "course.h"

void printCourse(Course current){
    printf("|%s |%s | %s |%d |%d | %d| %s| %s|\n",
    current.course_name, current.course_id, current.units);
    if (current.num_prequisites == 0) printf(" -| ");
    else {
        for (int i = 0; i < current.num_prequisites; i++){
            if(i) printf(",");
            printf(" %s", current.prerequisites[i]);
        }
    }
    printf("|%s |%s|%s|\n", current.section,
    current.field, current.department);
}

void searchCourse(){
    FILE* file = fopen("courses.json", "rb");
    Course current;
    system("cls");
    int opt;
    char phrase[50];
    printf("1. Search by course name\n2. Search by ID\n");
    printf("3. Search by section\n4. Search by field\n");
    printf("5. Search by department\nEnter an option: ");
    scanf("%d", &opt);
    printf("Ther phrase to search: ");
    scanf("%s", phrase);
    while (fread(&current, sizeof(Course), 1, file)){
        switch(opt){
            case 1:
                if (strcmp(current.course_name, phrase) == 0) printCourse(current);
                break;
            case 2:
                if (strcmp(current.course_id, phrase) == 0) printCourse(current);
                break;
            case 3:
                if (strcmp(current.section, phrase) == 0) printCourse(current);
                break;
            case 4:
                if (strcmp(current.field, phrase) == 0) printCourse(current);
                break;
            case 5:
                if (strcmp(current.department, phrase) == 0) printCourse(current);
                break;
        }
    }
}

void coursesListGeneral(){
    FILE* file = fopen("courses.json", "rb");
    Course current;
    int opt;
    while(1){
        rewind(file);
        printf("List of courses\n| course name | course id | units | prerequisites |");
        printf("section | field |department |\n|-------------|-----------|-------|");
        printf("-----------------------------------|---------|-------|");

        while(fread(&current, sizeof(Course), 1, file)) printCourse(current);
        printf("1. Search\n2. Go back\nEnter an option:");
        scanf("%d", &opt);
        if (opt == 1) searchCourse();
        else break;
    }
    fclose(file);
}

void addCourse(){}
void removeCourse(){}

void coursesListAdmin(){
    FILE* file = fopen("courses.json", "rb");
    Course current;
    int opt;
    while(1){
        rewind(file);
        printf("List of courses\n| course name | course id | units | prerequisites |");
        printf("section | field |department |\n|-------------|-----------|-------|");
        printf("-----------------------------------|---------|-------|");

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
