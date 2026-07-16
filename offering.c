#include <stdio.h>
#include "offering.h"

void printOfferingStudent(Offering current, int num){
    printf("|%d|%s |%s |", num, current.course.course_name, current.course.course_id);
    printf("%s %s |", current.faculty.first_name, current.faculty.last_name);
    printf("%d |%d | %d| ", current.semester, current.capacity, current.enrollments);
    printf("%s| %s| \n" ,current.course.department, current.place);
}

void printOfferingAdmin(Offering current, int num){
    printf("|%d|%s |%s |", num, current.course.course_name, current.course.course_id);
    printf("%s |", current.faculty.faculty_id);
    printf("%d |%d | %d| ", current.semester, current.capacity, current.enrollments);
    printf("%s| %s| \n" ,current.course.department, current.place);
}
