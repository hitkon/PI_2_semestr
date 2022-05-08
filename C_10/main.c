#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2){
    if (((Person *)p1)->age < ((Person *)p2)->age){
        return 1;
    }
    else if(((Person *)p1)->age > ((Person *)p2)->age){
        return -1;
    }
    else{
        if(strcmp(((Person *)p1)->first_name, ((Person *)p2)->first_name) == 0)
            return strcmp(((Person *)p1)->last_name, ((Person *)p2)->last_name);
        return strcmp(((Person *)p1)->first_name, ((Person *)p2)->first_name);
    }
}

// Read data to Person array (till EOF)
int read_person_array(Person *persons){
    //FILE *F = fopen("in.txt", "r");
    int n = 0;
    Person temp;
    while (!feof(stdin)){
        fscanf(stdin, "%d %s %s", &temp.age, temp.first_name, temp.last_name);
        persons[n] = temp;
        //fread(&temp, sizeof (struct Person), 1, F);
        //printf("%d %s %s\n", temp.age, temp.first_name, temp.last_name);
        n++;
    }
    //fclose(F);
    return n - 1;
}

// Print Person array
void print_person_array(Person *persons, int n){
    for (int i = 0; i < n; i++)
        printf("%d %s %s\n", persons[i].age, persons[i].first_name, persons[i].last_name);

}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2){
    char str1[MAX_STR_LEN];
    strcpy(str1, (char*)l1);
    char str2[MAX_STR_LEN];
    strcpy(str2, (char*)l2);
    char *temp = strtok(str1, " ");
    char age1[MAX_STR_LEN], age2[MAX_STR_LEN], name1[MAX_STR_LEN], name2[MAX_STR_LEN], last_name1[MAX_STR_LEN], last_name2[MAX_STR_LEN];
    strcpy(age1, temp);
    temp = strtok(NULL, " ");
    strcpy(name1, temp);
    temp = strtok(NULL, " ");
    strcpy(last_name1, temp);

    temp = strtok(str2, " ");
    strcpy(age2, temp);
    temp = strtok(NULL, " ");
    strcpy(name2, temp);
    temp = strtok(NULL, " ");
    strcpy(last_name2, temp);

    //printf("%s %s %s", age1, name1, last_name1);
    int name_len1 = strlen(name1);
    int name_len2 = strlen(name2);
    if (name1[name_len1-1] == 'a' && name2[name_len2-1] == 'a'){
        return strcmp(age1, age2);
    }
    else if(name1[name_len1-1] == 'a' && name2[name_len2-1] != 'a'){
        return -1;
    }
    else if(name1[name_len1-1] != 'a' && name2[name_len2-1] == 'a'){
        return 1;
    }
    else{
        return strcmp(last_name1, last_name2);
    }
}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]){
    //FILE *F = fopen("in.txt", "r");
    int n = 0;
    char temp1[MAX_STR_LEN];
    while (fgets(&lines[n], MAX_STR_LEN, stdin)){
        if (strlen(lines[n]) < 2)
            break;
//        fscanf(F, "%s", temp1);
//        strcpy(lines[n], temp1);
//        fscanf(F, "%s", temp1);
//        strcat(lines[n],temp1);
//        fscanf(F, "%s", temp1);
//        strcat(lines[n],temp1);


        //fscanf(F, "%d %s %s", &lines[n], &line[n].first_name, &persons[n].last_name);
        //fread(&persons[n], sizeof (struct Person), 1, F);
        //printf("%s\n", lines[n]);
        n++;
    }

    //fclose(F);
    return n;

}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n){
    for (int i = 0; i < n; i++)
        printf("%s", lines[i]);
}

// -------------------------------------------------

int read_int() {
    char buf[MAX_STR_LEN];
    int n;
    fgets(buf, MAX_STR_LEN, stdin);
    sscanf(buf, "%d", &n);
    return n;
}

int main(void) {
    int to_do = read_int();
    int n;
    Person persons[MAX_PERSONS];
    char lines[MAX_PERSONS][MAX_STR_LEN];
    switch (to_do) {
        case 1:
            n = read_person_array(persons);
            qsort(persons, (size_t)n, sizeof(Person), cmp_person);
            print_person_array(persons, n);
            break;
        case 2:
            n = read_lines(lines);
            qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
            print_lines(lines, n);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }
}

