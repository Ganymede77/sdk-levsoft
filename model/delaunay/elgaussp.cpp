#include "elgaussp.h"

void Eliminacao_Gauss( double  ** coeficientes, double * x, int n_linhas)
{
    int i, j, l, k;
    int n_col;

    double m, s, temp;

    n_col    = n_linhas + 1;
    for (k =0; k < n_linhas - 1; k++)
    {
// ========================
// o trecho a seguir implementa o pivotamento
// ========================
        i = k + 1;
        l = k;
        for (j = i; j < n_linhas; j++)
            if (fabs(coeficientes[j][k]) > fabs(coeficientes[l][k]))  l = j;

        if  ( j  !=  k )
        {
            for (j = k; j < n_col; j++)
            {
                 temp =   coeficientes[k][j];
                 coeficientes[k][j] =  coeficientes[l][j];
                 coeficientes[l][j] = temp;
            }
        }
// ========================

        for (i = k+1; i < n_linhas; i++)
        {
            m = - coeficientes[i][k] / coeficientes[k][k];
            coeficientes[i][k] = 0;
            for (j = k + 1; j < n_col; j++)
               coeficientes[i][j] +=  m * coeficientes[k][j] ;

        }
    }

    x[n_linhas-1] = coeficientes[n_linhas-1][n_col-1] / coeficientes[n_linhas-1][n_linhas-1];

    for (i = n_linhas - 2; i>=0 ; i--) {
         s = 0;
         for (j = i + 1 ; j < n_linhas; j++)  s += coeficientes[i][j] * x[j];
         x[i] = ( coeficientes[i][n_col-1] - s ) / coeficientes[i][i];
    }
}
