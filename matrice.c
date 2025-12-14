#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int **creationMatrice(int dim) {
    int **mat = malloc(dim * sizeof(int *));
    for (int i = 0; i < dim; i++) {
        mat[i] = malloc(dim * sizeof(int));
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            mat[i][j] = rand() % 100;
        }
    }
    return mat;
}

void sauvegardeMatriceBinary(int **mat, int dim, const char *nomfichier) {
    FILE *fp = fopen(nomfichier, "wb");
    if (fp==NULL)
    {
        perror("Erreur ouverture fichier binaire");
        exit(1);
    }

    fwrite(&dim, sizeof(int), 1, fp);
    for (int i = 0; i < dim; i++) {
        fwrite(mat[i], sizeof(int), dim, fp);
    }
    fclose(fp);
}

void sauvegardeMatriceText(int **mat, int dim, const char *nomfichier) {
    FILE *fp = fopen(nomfichier, "w");
    if (fp==NULL)
    {
         perror("Erreur ouverture fichier texte");
         exit(1);
     }
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fprintf(fp, "%d ", mat[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void chargementBinary(const char *nomfichier, int dim) {

    FILE *fp = fopen(nomfichier, "rb");
    if (!fp)
    {
        perror("Erreur ouverture fichier binaire");
        exit(1);
     }
    int d;
    fread(&d, sizeof(int), 1, fp);
    if (d != dim) {
        printf("Erreur : dimension du fichier %d different a celle option -d %d\n", d, dim);
        fclose(fp);
        exit(1);
    }
    int val;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fread(&val, sizeof(int), 1, fp);
            printf("%4d ", val);
        }
        printf("\n");
    }
    fclose(fp);
}


void chargementText(const char *nomfichier, int dim) {
    FILE *fp = fopen(nomfichier, "r");
    if (fp==NULL) {
         perror("Erreur ouverture fichier texte\n");
         exit(1);
         }
    int val;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fscanf(fp, "%d", &val);
            printf("%4d ", val);
        }
        printf("\n");
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {

    if (argc<2) {
        printf("Argument insufisant\n");
        printf( "Usage : %s du fichier\n", argv[0]);
        return 1;
    }
    int opt;
    int create = 0, affiche = 0;
    int dim = 0;
    int modeTexte = 0;
    char *nomfichier = NULL;

    while ((opt = getopt(argc, argv, ":cad:btf:")) != -1) {
        switch (opt) {
            case 'c': create = 1; break;
            case 'a': affiche = 1; break;
            case 'd': dim = atoi(optarg);
                     if (dim<0)
                      {
                        perror("la dimension doit etre > 0\n");
                        return 0;
                      }
                      break;

            case 'b': modeTexte = 0;
                      break;
            case 't': modeTexte = 1;
                      break;
            case 'f': nomfichier = optarg;
                       break;
            case ':': printf("Erreur : l'option -%c attend un argument\n", optopt);
                      return 1;
            case '?': printf("Erreur : option -%c inconnue\n", optopt);
                       return 1;
        }

    }

    if (create) {
        int **mat = creationMatrice(dim);
        if (modeTexte!=0)
             sauvegardeMatriceText(mat, dim, nomfichier);
        else
            sauvegardeMatriceBinary(mat, dim, nomfichier);
        for (int i = 0; i < dim; i++)
                 free(mat[i]);
        free(mat);
    }

    if (affiche) {
        if (modeTexte!=0)
            chargementText(nomfichier, dim);
        else
            chargementBinary(nomfichier, dim);
    }

    return 0;
}
