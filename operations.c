#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "prakseis.h"
#define ROWS_MAX 20
#define COLS_MAX 20
#define NOT_DEBUG

struct matrix define_matrix();

/* Calculating adjugate matrix:
 *
 * Parameter struct matrix A: the matrix of which the adjugate matrix is calculated
 * Returns: structure with adjugate matrix
 */
struct matrix adj(struct matrix A) {
    struct matrix y = define_matrix();
    y.cols = A.cols;
    y.rows = A.rows;

    for(int i = 0; i < y.rows; i++) {
        for(int j = 0; j < y.cols; j++)
            y.mat[i][j] = pow(-1, (i + 1) + (j + 1)) * det(cofactor(A, i, j));
    }
    return transpose_matrix(y);
}

/* Calculating inverse matrix:
 * Checking if matrix given is inversable
 *
 * Paramenter struct matrix A: τετραγωνικός matrix of which the inverse matrix is calculated
 * Returns: structure with inverse matrix
 */
struct matrix inverse_matrix(struct matrix A) {
    struct matrix X = define_matrix();
    X.cols = X.rows = A.rows;

    if(det(A) == 0) {
        puts("Matrix is not inversable");
        X.invalid = true;
    } else {
        for(int i = 0; i < X.rows; i++)
            for(int j = 0; j < X.rows; j++)
                //Α^-1 = (1/|Α|) * adj(A)
                X.mat[i][j] = (1 / det(A)) * adj(A).mat[i][j];
    }
    return X;
}

/* Matrix in power:
 * Multiplying square matrix with itself n times
 *
 * Parameter struct matrix A: the base matrix
 * Parameter int power: integer which functions as exponent
 * Return: structure exponent matrix
 */
struct matrix exp_matrix(struct matrix A, int power) {
    struct matrix y = A;

    if(A.cols == A.rows) {
        for(int i = 0; i < power; i++)
            y = multiplication_matrix(y, A);
    } else {
        y.invalid = true;
        puts("Matrix is not square");
    }

    return y;
}

/* Trace:
 * Check if matrix is square
 * Add elements of primary diagonal
 *
 * Parameter struct matrix A: structure with matrix
 * Returns: trace as double
 */
double trace_mat(struct matrix A) {

    double trace = 0;
    if(A.rows == A.cols) {
        for(int i = 0; i < A.rows; i++) {
            for(int j = 0; j < A.cols; j++) {
                if(i == j)
                    trace += A.mat[i][j];
            }
        }
    } else
        puts("Matrix is not square");
    return trace;
}

/* Dot Product:
 * Check if vectors have the same dimensions
 * Calculating the dot product of vector A and B
 *
 * Parameter struct matrix A: structure with vector
 * Parameter struct matrix Β: structure with vector
 * Returns: dot product as double
 */
double dot_product(struct matrix A, struct matrix B) {
    double p = 0;
    if(A.rows == B.rows) {
        for(int i = 0; i < A.rows; i++)
            p += A.mat[i][0] * B.mat[i][0];
    } else
        puts("Vectors dont have same dimensions");

    return p;
}

/* Calculating transpose matrix:
 * Reversing lines and collumns 
 *
 * Παράμετρος struct matrix A: structure with matrix
 * Returns: structure with transpose
 */
struct matrix transpose_matrix(struct matrix A) {
    struct matrix At = define_matrix();
    At.rows = A.cols;
    At.cols = A.rows;

    for(int i = 0; i < A.rows; i++) {
        for(int j = 0; j < A.cols; j++)
            At.mat[j][i] = A.mat[i][j];
    }
    return At;
}

/* Πολλαπλασιασμός 2 πινάκων:
 * Έλεγχος αν ορίζεται ο πολλαπλασιασμός
 *
 * Παράμετρος struct matrix A: structure με πίνακα
 * Παράμετρος struct matrix Β: structure με πίνακα
 * Επιστρέφει: structure με τον πίνακα γινομένου
 */
struct matrix multiplication_matrix(struct matrix A, struct matrix B) {
    struct matrix C = define_matrix();

    if(A.cols == B.rows) {
        int k = A.cols;
        C.rows = A.rows;
        C.cols = B.cols;

        int sum = 0;
        for(int i = 0; i < C.rows; i++) {
            for(int j = 0; j < C.cols; j++) {
                for(int l = 0; l < k; l++)
                    sum += A.mat[i][l] * B.mat[l][j];
                C.mat[i][j] = sum;
                sum = 0;
            }
        }
    } else {
        C.invalid = true;
        puts("O πολλαπλασιασμός δεν ορίζεται");
    }

    return C;
}

/* Aφαίρεση 2 πινάκων:
 * Έλεγχος αν ορίζεται η αφαίρεση
 *
 * Παράμετρος struct matrix A: structure με πίνακα
 * Παράμετρος struct matrix Β: structure με πίνακα
 * Επιστρέφει: structure με τον πίνακα διαφοράς
 */
struct matrix subtraction_matrix(struct matrix A, struct matrix B) {
    struct matrix C = define_matrix();

    if(A.rows == B.rows && A.cols == B.cols) {
        C.rows = A.rows;
        C.cols = A.cols;
        if(C.cols == 1)
            C.vect = true;

        for(int i = 0; i < A.rows; i++)
            for(int j = 0; j < A.cols; j++)
                C.mat[i][j] = A.mat[i][j] - B.mat[i][j];
    } else C.invalid = true;

    return C;
}

/* Πρόσθεση 2 πινάκων:
 * Έλεγχος αν ορίζεται η πρόσθεση
 *
 * Παράμετρος struct matrix A: structure με πίνακα
 * Παράμετρος struct matrix Β: structure με πίνακα
 * Επιστρέφει: structure με τον πίνακα αθροίσματος
 */
struct matrix sum_matrix(struct matrix A, struct matrix B) {

    struct matrix C = define_matrix();

    if(A.rows == B.rows && A.cols == B.cols) {
        C.rows = A.rows;
        C.cols = A.cols;
        if(C.cols == 1)
            C.vect = true;

        for(int i = 0; i < A.rows; i++)
            for(int j = 0; j < A.rows; j++)
                C.mat[i][j] = A.mat[i][j] + B.mat[i][j];
    } else {
        C.invalid = true;
        puts("Οι πίνακες δεν έχουν τις ίδιες διαστάσεις");
    }

    return C;
}

/* Yπολογισμός ορίζουσας:
 *
 * Παράμετρος struct matrix Α: structure με τετραγωνικό πίνακα
 * Επιστρέφει: την ορίζουσα ως δεκαδικό
 */
double det(struct matrix A) {
    int n = A.rows;
    double d = 0;
    double *pd = &d;

    if(n == 1)
        *pd = A.mat[0][0];
    else {
        for(int j = 0; j < n; j++) {
            int sign = pow(-1, (j + 1) + 1);
            *pd += sign * A.mat[0][j] * det(cofactor(A, 0, j));
        }
    }
    return *pd;
}

/* Συμπαράγοντας Αxy:
 *
 * Παράμετρος struct matrix Α: structure με τετραγωνικό πίνακα
 * Παράμετρος int x: ακέραιος που δηλώνει την γραμμή που θα αφαιρεθεί
 * Παράμετρος int y: ακέραιος που δηλώνει την στήλη που θα αφαιρεθεί
 * Επιστρέφει: structure με τον πίνακα αθροίσματος
 */
struct matrix cofactor(struct matrix A, int x, int y) {
#ifdef DEBUG
    puts("αρχη της cofactor");
#endif // DEBUG

    int n = A.rows;
    //δημιουργία πίνακα αποτελέσματος
    struct matrix B = define_matrix();
    B.rows = B.cols = n - 1;
    int a = 0, b = 0;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i != x && j != y) {
                B.mat[a][b] = A.mat[i][j];
                if(++b == n - 1) {
                    a++;
                    b = 0;
                }
            }
        }
    }
    return B;
}

/* Eξωτερικό γινόμενο:
 * Έλεγχος ότι τα διανύσματα ανήκουν στον Διανυσματικό Χώρο R^3
 *
 * Παράμετρος struct matrix Α: structure με διάνυσμα
 * Παράμετρος struct matrix Β: structure με διάνυσμα
 * Επιστρέφει: structure με το διάνυσμα αποτελέσματος
 */
struct matrix vector_product(struct matrix A, struct matrix B) {
    //δημιουργία πίνακα αποτελέσματος
    struct matrix C = define_matrix();
    C.vect = true;
    C.cols = 1;
    C.rows = 3;

    //αν οι συστοιχίες είναι διανύσματα και έχουν 3 διαστάσεις
    if(A.cols == 1 && B.cols == 1 && A.rows == 3 && B.rows == 3) {

        C.mat[0][0] = A.mat[1][0] * B.mat[2][0] - A.mat[2][0] * B.mat[1][0];
        C.mat[1][0] = A.mat[2][0] * B.mat[0][0] - A.mat[0][0] * B.mat[2][0];
        C.mat[2][0] = A.mat[0][0] * B.mat[1][0] - A.mat[1][0] * B.mat[0][0];
    } else {
        puts("Το εξωτερικό γινόμενο επιτρέπει μόνο διανύσματα 3 διαστάσεων");
        C.invalid = true;
    }
    return C;
}

/* Πολλαπλασιασμός πίνακα με αριθμό:
 * Πολλαπλασιασμός κάθε στοιχείου του πίνακα με σταθερό αριθμό
 *
 * Παράμετρος struct matrix Α: structure με πίνακα
 * Παράμετρος double c: σταθερός αριθμός
 * Επιστρέφει: structure με τον πολλαπλασιασμένο πίνακα
 */
struct matrix multiply_byNumber(struct matrix A, double c) {
    for(int i = 0; i < A.rows; i++) {
        for(int j = 0; j < A.cols; j++)
            A.mat[i][j] = A.mat[i][j] * c;
    }
    return A;
}