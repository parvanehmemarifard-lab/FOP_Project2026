#include <stdio.h>
#include <stdlib.h>
#include "login.h"
#include "student.h"

int main(){
    FILE* file = fopen("students.json", "ab+");
    Student s1 = {"pari", "memari","404106377", "52641625", "d", 404, "s", " ", " ", "tehran", "hola", "red"};
    Student s2 = {"parivaneh", "mari","4043378", "52641625", "d", 403, "s", " ", " ", "boroj", "adios", "pink"};
    fwrite(&s1, sizeof(Student), 1, file);
    fwrite(&s2, sizeof(Student), 1, file);
    loginPage();
}
