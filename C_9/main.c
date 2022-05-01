#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A){
    return A[row * cols + col];
}

void set(int cols, int row, int col, double *A, double value){
    A[row * cols + col] =  value;
}


void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB){
    for (int i = 0; i < rowsA; i++){
        for(int j = 0; j < colsB; j++){
            double sum = 0;
            for(int k = 0; k < colsA; k++)
                sum += get(colsA, i, k, A) * get(colsB, k, j, B);
            set(colsB, i, j, AB, sum);
        }
    }
}

void read_mat(int rows, int cols, double *t){
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            double read;
            scanf("%lf", &read);
            set(cols, i, j, t, read);
        }
    }
}

void print_mat(int rows, int cols, double *t){
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%.2f ", get(cols, i, j, t));
        }
        printf("\n");
    }
}

int read_char_lines(char *tab[]){
    int n = 0;
    char str[TAB_SIZE];
    fgets(str, 1000, stdin);
    while(fgets(str, 1000 , stdin)){
        tab[n] = malloc((strlen(str) + 10) * sizeof (char));
        strcpy(tab[n], str);
        n++;
    }
    return n;
}

void write_char_line(char *tab[], int n){
    printf("%s\n", tab[n-1]);
}

void delete_lines(char *tab[], int line_count){
    for(int i = 0; i < line_count; i++){
        free(tab[i]);
    }
}

int read_dbl_lines_v1(double *ptr_tab[]){
    int n = 0;
    char str[TAB_SIZE];
    //gets(str);
    int i = 0;
    char c;
    while(!feof(stdin)){
        c = getc(stdin);
        if( c != '\n') {
            str[i++] = c;
            continue;
        }
        else{
            if(i == 0)
                continue;
        }
        str[i++] = '\0';
        double *tab = malloc(TAB_SIZE * sizeof(double));
        int j = 0;
        double x;
        int len = 0, size = 0;
        while(sscanf(str + len, "%lf%n", &x, &size) != EOF) {
            tab[j+1] = x;
            j++;
            len += size;
//            if(len >= strlen(str))
//                break;
        }
        tab[0] = j;
        ptr_tab[n++] = tab;
        if(c == '\n')
            i=0;
        else
            break;
    }
//    while(!feof(stdin)){
//        gets(str);
//        double *tab = malloc(TAB_SIZE * sizeof(double));
//        int j = 0;
//        int len = 0, size = 0;
//        while(sscanf(str + len, "%lf%n", &tab[j+1], &size)) {
//            j++;
//            len += size;
//            if(len >= strlen(str))
//                break;
//        }
//        tab[0] = j - 1;
//        ptr_tab[n] = tab;
//        n++;
//    }
//    gets(str);
    return n;
}

void write_dbl_line_v1(double *ptr_tab[], int n){
    n--;
    for(int i = 0; i < (int)((double)ptr_tab[n][0]); i++){
        double x = ptr_tab[n][i + 1];
        printf("%.2lf ", x);
    }
    printf("\n");
}

int main(void) {
    int to_do;

    scanf ("%d", &to_do);

    double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
    int n, lines_counter, rowsA,colsA,rowsB,colsB;
    char *char_lines_table[TAB_SIZE];
    double series_table[TAB_SIZE];
    double *ptr_table[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d",&rowsA,&colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d",&rowsB,&colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA,colsA,colsB,A,B,C);
            print_mat(rowsA,colsB,C);
            break;
        case 2:
            scanf("%d",&n);
            ptr_table[0] = series_table;
            lines_counter = read_dbl_lines_v1(ptr_table);
            write_dbl_line_v1(ptr_table,n);
            //delete_lines(ptr_table, lines_counter);
            break;
        case 3:
            scanf("%d", &n);
            lines_counter = read_char_lines(char_lines_table);
            write_char_line(char_lines_table,n);
            delete_lines(char_lines_table,lines_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
