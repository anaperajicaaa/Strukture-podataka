#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _Student {
    char name[30];
    char lastname[30];
    int points;
} Student;

int main() {

    FILE* fp = fopen("data.txt", "r");

    if (!fp) {
        printf("Error opening file\n");
        return 1;
    }

    char buffer[50];
    int br = 0;

    while (fgets(buffer, 50, fp) != NULL) {
        br++;
    }

    printf("broj studenata: %d\n", br);

    Student* students = (Student*)malloc(br * sizeof(Student));

    if (!students) {
        fclose(fp);
        return 1;
    }

    rewind(fp);

    for (int i = 0; i < br; i++) {
        fscanf(fp, "%29s %29s %d",
            students[i].name,
            students[i].lastname,
            &students[i].points);
    }

    int maxPoints = students[0].points;

    for (int i = 1; i < br; i++) {
        if (students[i].points > maxPoints) {
            maxPoints = students[i].points;
        }
    }

    for (int i = 0; i < br; i++) {
        float relativepoints =
            (float)students[i].points / maxPoints * 100;

        printf("%s %s - %d, %.2f%%\n",
            students[i].name,
            students[i].lastname,
            students[i].points,
            relativepoints);
    }

    free(students);
    fclose(fp);

    return 0;
}