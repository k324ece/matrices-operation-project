#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define ROWS_MAX 100
#define COLS_MAX 100
#define NOT_DEBUG
#define NOT_DEBUGSTR

char *prime_menu = "\tΠΡΑΞΕΙΣ ΜΕΤΑΞΥ ΠΙΝΑΚΩΝ\n\n\
1.Δημιουργία συστυχίας\n\
2.Προβολή διαθέσιμων συστοιχιών\n\
3.Φόρτωση συστυχίας\n\
4.Διαγραφή συστυχίας\n\
5.Πράξεις πινάκων\n\
6.Πράξεις διανυσμάτων\n\
7.Έξοδος\n\n";

struct matrix{
    double mat[ROWS_MAX][COLS_MAX];
    int rows,cols;
};

struct matrix getmatrix();
bool save_matrix();
bool show_matrixes();
bool load_matrix();
int menu(char* menu);
void print_elements();
int delete_matrixName(char* name);
int delete_matrix();

int main()
{
    enum {create_matrix, show_matrix, ld_matrix, del,
          matrix_operations, vector_operations, exit};

    bool stop = false;

    while(!stop)
    {
        switch(menu(prime_menu))
        {
        case create_matrix:
            save_matrix();
            break;

        case show_matrix:
            show_matrixes();
            print_elements();
            break;

        case ld_matrix:
            load_matrix();
            break;

        case matrix_operations:
            //πραξεις πινάκων

            break;
        case vector_operations:
            //πράξεις διανυσμάτων
            break;

        case del:
            delete_matrix();
            break;

        case exit:
            stop = true;
            break;

        default:
            printf("Η επιλογη που βάλατε δεν υπάρχει\n");
        }
    }

    return 0;
}

//ζητάει απο χρήστη πίνακα: διαστάσεις και στοιχεία
struct matrix getmatrix()
{
    struct matrix y;

    //διαστάσεις
    printf("\nΟρισμός διαστάσεων\nΠροσοχή! Μέγιστες διαστάσεις: 100x100\n\nΓραμμές: ");
    scanf("%d", &y.rows);
    printf("Στήλες: ");
    scanf("%d", &y.cols);

    //εισαγωγη στοιχειων
    printf("\nΣυμπλήρωση τιμών:\nΔώσε στοιχεία πίνακα χωρισμένα με κένο αλλαγή γραμμής\n");
    printf ("π.χ. ένας πίνακας 2x3:\n1 0 45.87\n2 5.8 89\n\n");

    for(int i=0;i<y.rows;i++)
    {
        for (int j=0; j<y.cols; j++)
        scanf("%lf", &y.mat[i][j]);
    }

    return y;
}

//δημιουργεί συστυχία και αποθηκεύει σε txt file
//επιστρέφει false αν δεν κατάφερει να ανοιξει το αρχείο
//επιστρέφει true αν εκτελεστεί ο κώδικας κανονικά
bool save_matrix(){

    #ifdef DEBUG
    printf("save_matrix test 1 line 135");
    #endif // DEBUG

    //αναγνωριστικο πίνακα
    printf("\nΟρισμός αναγνωριστικού: ");
    char name[50];
    scanf("%s", name);

#ifndef DEBUGSTR

    //δημιουργία πίνακα
    struct matrix x =getmatrix();

    //κατέγραψε το όνομα του πίνακα για να μπορει να παρουσιαστεί ως διαθέσιμος
    FILE* am_file;
    am_file = fopen("available_matrix.txt", "a");
    if (am_file == NULL) return false;

    fprintf (am_file,"%s\n",name);
    fclose(am_file);

    //πρόσθεσε την καταληξη ".txt" στο αναγνωριστικο
    strcat(name, ".txt");

    //δημιούργησε αρχείο "αναγνωριστικο".txt
    FILE *file;
    file = fopen(name, "w");

    //πρώτες 2 σειρες τύπωσε τις διαστάσεις
    fprintf(file, "%d\n%d\n", x.rows,x.cols);

    for (int i=0; i<x.rows; i++){
        for (int j=0; j<x.cols; j++){;
            fprintf(file, "%lf ", x.mat[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    #endif // DEBUGSTR
    return true;
}

//προβάλει τους διαθέσιμους πίνακες
bool show_matrixes(){
    FILE* AM;
    AM = fopen("available_matrix.txt", "r");

    char matrixName[50];
    //επανέλαβε μέχρι το τέλος του αρχείου
    while ((fscanf(AM,"%s",matrixName))!= EOF){
        //&& ελεγχος οτι δεν ειναι σφάλμα της fscanf

        printf("%s\n", matrixName);
     }

    return true;
}

//φορτώνει δισδιάστατη συστυχία από αρχείο txt
bool load_matrix()
{
    // οδηγίες
    printf("Για φόρτωση συστοιχίας από αρχείο πρέπει:\n");
    printf("-Το αρχείο πρέπει να έχει επέκταση txt και στο ίδιο directory\n");
    printf("-Ο αριθμός των γραμμών να αναγράφεται στην πρώτη γραμμή\n");
    printf("-Ο αριθμός των στηλών να αναγράφεται στην δεύτερη γραμμή\n");
    printf("-Μέγιστες δυνατές διαστάσεις: 100x100\n\n");

    //εισαγωγη ονοματος συστοιχιας απο το χρηστη
    char matrixName[50];
    char filename[50];
    printf("Εισαγωγή ονόματος συστοιχίας (χωρίς επέκταση txt): ");
    scanf(matrixName);

    strcpy(filename, matrixName);
    strcat(filename, ".txt");

    //ανοιγμα αρχείου σε read mode
    FILE *fp;
    fp = fopen(filename,"r");

    //αν υπάρξει αδυναμία άνοιγμα αρχείου
    if (fp == NULL)
        printf("Aδυναμία άνοιγμα αρχείου\n");
    else{
    //αλλιως αποθήκευσε το όνομα της συστοιχίας στο available_matrix.txt
    FILE* am_file;
    am_file = fopen("available_matrix.txt", "a");
    if (am_file == NULL)
        printf("Αδυναμία φόρτωσης συστοιχίας\n");

    fprintf (am_file,"%s\n",matrixName);
    fclose(am_file);
    }

    return 0;
}

//τυπώνει μενού με επιλόγές και ζητάει επιλογή απο τον χρήστη
//επιστρέφει την επιλογή του χρήστη -1
int menu(char* menu)
{
    printf(prime_menu);
    printf("Επιλογή: ");
    int choice;
    fflush(stdin);
    scanf("%d", &choice);

    return choice-1;
}

//τυπώνει τα στοιχεία ενος πίνακα επιλογής του χρήστη
void print_elements()
{
    printf("\nΕπιλογή πίνακα για προβολή: ");
    char name[50];
    scanf("%s", name);
    strcat(name, ".txt");

    FILE *fp;
    fp = fopen(name,"r");

    if (fp == NULL)
        printf("Ο πίνακας που επιλέξατε δεν υπάρχει\n");
    else
    {
        //οι πρώτες δύο γραμμές δίνουν τις διαστάσεις
        int r,c;
        fscanf(fp, "%d", &r);
        fscanf(fp, "%d", &c);

        for (int i=0; i<r; i++)
        {
            for (int j=0; j<c; j++)
            {
                double element;
                fscanf(fp, "%lf", &element);
                printf("%lf ", element);
            }
            printf("\n");
        }
        fclose (fp);
    }
}

//διαγράφει το όνομα της συστοιχίας απο το "available_matrix.txt"
int delete_matrixName(char* name)
{
    FILE *fp1, *fp2;
    char matrixName[50];

    //άνοιγμα αρχειου σε read mode
    fp1 = fopen("available_matrix.txt", "r");
    //άνοιγμα αρχειου σε write mode
    fp2 = fopen("copy.txt", "w");

    while ((fscanf(fp1,"%s",matrixName))!= EOF)
    {//&& ελεγχος οτι δεν ειναι σφάλμα της fscanf

        //εκτος απο την συστυχια για διαγραφη
        if (strcmp(name,matrixName) != 0)
            //αντέγραψε ολες τις υπόλοιπες γραμμες στο copy.txt
            fprintf(fp2, "%s\n",matrixName);
    }

    fclose(fp1);
    fclose(fp2);

    //διαγραφη αρχικου αρχειου
    remove("available_matrix.txt");

    //μετονομασια του copy.txt σε available_matrix.txt
    rename("copy.txt", "available_matrix.txt");

    return 0;
}

//διαγράφει αρχείο συστοιχίας
int delete_matrix()
{
    char name[50];

    printf ("Εισαγωγη συστυχίας για διαγραφη: ");
    scanf("%s", name);

    char filename[50];
    strcpy(filename, name);

    //προσθεσε την καταληξη ".txt"
    strcat(filename, ".txt");

    if (remove(filename) == 0)
        printf("Διαγράφτηκε επιτυχως\n");
    else
        printf("Αδυναμία διαγραφης\n");

    //για να μην προβάλεται στους διαθέσιμες συστοιχίες
    delete_matrixName(name);

    return 0;
}

