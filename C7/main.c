#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        scanf("%lf", x++);
    }
}

void print_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        printf("%.4f ", x[i]);
    }
    printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            scanf("%lf", &A[i][j]);
        }
    }
}

void print_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            printf("%.4f ", A[i][j]);
        }
        printf("\n");
    }
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]){

}

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n){
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            double sum = 0;
            for (int k = 0; k < p; k++){
                sum += A[i][k] * B[k][j];
            }
            AB[i][j] = sum;
        }
    }
}

// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n){

}

void backward_substit(double A[][SIZE], double x[], int n){

}


void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n){

}

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n){
    for (int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            for (int k = i+1; k < n; ++k) {
                A[j][k] -= A[i][k] * A[j][i] / A[i][i];
            }
        }
    }
    if(fabs(A[n-1][n-1]) < 0.0001)
        return NAN;
    double det = 1.0;
    for(int i = 0; i < n; i++)
        det *= A[i][i];
    return det;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps){
    double b_cpy[n];
    for (int i = 0; i < n; i++)
        b_cpy[i] = b[i];

    double det = 1.0;
    for (int i = 0; i < n - 1; i++){
        int isDet = -1;
        for(int j = i; j < n; j++){
            if(fabs(A[j][i]) > eps && isDet == -1)
                isDet = j;
            else if(fabs(A[j][i]) > fabs(A[isDet][i]))
                isDet = j;
        }
        if (isDet == -1){
            return 0;
        }
        else{
            if (isDet != i)
                det *= -1.0;
            for (int j = 0; j < n; j++){
                double tmp = A[i][j];
                A[i][j] = A[isDet][j];
                A[isDet][j]=tmp;
            }
            double tmp = b_cpy[i];
            b_cpy[i] = b_cpy[isDet];
            b_cpy[isDet]=tmp;
        }

        for(int j = i + 1; j < n; j++){
            for (int k = i+1; k < n; ++k) {
                A[j][k] -= A[i][k] * A[j][i] / A[i][i];
            }
            b_cpy[j] -= b_cpy[i] * A[j][i] / A[i][i];
        }
    }

    for(int i = 0; i < n; i++)
        det *= A[i][i];

    if(fabs(det) < eps || b == NULL  || x == NULL)
        return 0;

    for (int i = n-1; i > -1; i--){
        double res = b_cpy[i];
        for(int j = n-1; j > i; j--){
            res -= x[j] * A[i][j];
        }
        x[i]= res / A[i][i];
    }
    return det;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps){

    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            B[i][j] = 0.0;
    for (int i = 0; i < n; i++)
        B[i][i] = 1.0;

    double det = 1.0;
    for (int i = 0; i < n - 1; i++){
        int isDet = -1;
        for(int j = i; j < n; j++){
            if(fabs(A[j][i]) > eps && isDet == -1)
                isDet = j;
            else if(fabs(A[j][i]) > fabs(A[isDet][i]))
                isDet = j;
        }
        if (isDet == -1){
            return 0;
        }
        else{
            if (isDet != i)
                det *= -1.0;
            for (int j = 0; j < n; j++){
                double tmp = A[i][j];
                A[i][j] = A[isDet][j];
                A[isDet][j]=tmp;
                tmp = B[i][j];
                B[i][j] = B[isDet][j];
                B[isDet][j] = tmp;
            }
        }

        for(int j = i + 1; j < n; j++){
            for (int k = i+1; k < n; ++k) {
                A[j][k] -= A[i][k] * A[j][i] / A[i][i];
            }
            for(int k = 0; k < n; k++)
                B[j][k] -= B[i][k] * A[j][i] / A[i][i];
        }
    }

    for(int i = 0; i < n; i++)
        det *= A[i][i];

    if(fabs(det) < eps)
        return 0;

    for (int i = n-1; i > -1; i--){
        for(int k = 0; k < n; k++) {
            double res = B[i][k];
            for (int j = n - 1; j > i; j--) {
                res -= B[j][k] * A[i][j];
            }
            B[i][k] = res / A[i][i];
        }
    }
    return det;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    int m, n, p;

    scanf ("%d", &to_do);

    switch (to_do) {
        case 1:
            scanf("%d %d %d", &m, &p, &n);
            read_mat(A, m, p);
            read_mat(B, p, n);
            mat_product(A, B, C, m, p, n);
            print_mat(C, m, n);
            break;
        case 2:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", gauss_simplified(A, n));
            break;
        case 3:
            scanf("%d", &n);
            read_mat(A,n, n);
            read_vector(b, n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n", det);
            if(det) print_vector(x, n);
            break;
        case 4:
            scanf("%d", &n);
            read_mat(A,n,n);
            printf("%.4f\n",matrix_inv(A, B, n, eps));
            print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

