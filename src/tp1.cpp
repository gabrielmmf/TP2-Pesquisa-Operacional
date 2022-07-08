#include <iostream>
#include <iomanip>
#define EPS 0.00000001
using namespace std;

/*void ImprimeTableau(double *tableau[], int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << tableau[i][j] << "  ";
            if (j == n - 2 or j == m - 2)
            {
                cout << "|";
            }
        }
        cout << endl;
    }
}*/

void PreparaPivot(double *tableau[], int n, int m, int linha, int coluna)
{

    if (abs(tableau[linha][coluna]) > 0.00000001)
    {
        return;
    }
    for (int i = 1; i < n; i++)
    {
        if (tableau[i][coluna] != 0)
        {
            for (int j = 0; j < m; j++)
            {
                tableau[linha][j] = tableau[linha][j] + tableau[i][j];
            }
            return;
        }
    }
}

void Pivoteia(double *tableau[], int n, int m, int linha, int coluna)
{
    double d = 0;

    if (abs(tableau[linha][coluna]) <= EPS)
        return;

    for (int i = 0; i < n; i++)
    {
        if (tableau[i][coluna] != 0)
        {
            d = tableau[i][coluna] / tableau[linha][coluna];
            if (i == linha)
            {
                d = 1 / tableau[linha][coluna];
                for (int j = 0; j < m; j++)
                {
                    tableau[i][j] = tableau[i][j] * d;
                }
            }
            else
            {
                for (int j = 0; j < m; j++)
                {
                    if (tableau[linha][j] != 0)
                        tableau[i][j] = tableau[i][j] - tableau[linha][j] * d;
                }
            }
        }
    }
}

int EncontraMenorRazao(double *tableau[], int n, int m, int coluna)
{
    int indice_b = m - 1;
    double val_min = -1;
    int indice_min = -1;
    for (int i = 1; i < n; i++)
    {
        if (tableau[i][coluna] > 0.000000001)
        {
            if (val_min < 0 or tableau[i][indice_b] / tableau[i][coluna] < val_min)
            {
                val_min = tableau[i][indice_b] / tableau[i][coluna];
                indice_min = i;
            }
        }
    }
   /* if (indice_min != -1)
    {
        cout << "Menor razão encontrada: " << tableau[indice_min][indice_b] << "/" << tableau[indice_min][coluna] << endl;
    }*/
    return indice_min;
}

void MontaAux(double *auxiliar[], double *tableau[], int n, int m)
{

    for (int i = 0; i < n; i++)
        auxiliar[i] = new double[m + n - 1];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n + m - 1; j++)
        {
            auxiliar[i][j] = 0;
        }
    }

    for (int j = m - 1; j < m - 2 + n; j++)
        auxiliar[0][j] = 1;

    for (int i = 1; i < n; i++)
    {
        auxiliar[i][m - 2 + i] = 1;
        for (int j = 0; j < m - 1; j++)
        {
            auxiliar[i][j] = tableau[i][j];
        }
        auxiliar[i][m - 2 + n] = tableau[i][m - 1];
    }

    for (int i = 1; i < n; i++)
    {
        Pivoteia(auxiliar, n, m + n - 1, i, m - 2 + i);
    }
}

void adicionaFolga(double *auxiliar[], double *tableau[], int n, int m)
{

    for (int i = 0; i < n; i++)
        auxiliar[i] = new double[m + n - 1];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n + m - 1; j++)
        {
            auxiliar[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++)
    {
        auxiliar[i][m - 2 + i] = 1;
        for (int j = 0; j < m - 1; j++)
        {
            auxiliar[i][j] = tableau[i][j];
        }
        auxiliar[i][m - 2 + n] = tableau[i][m - 1];
    }
}

void Positiva_B(double *tableau[], int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        if (tableau[i][m - 1] < 0)
        {
            for (int j = 0; j < m; j++)
            {
                if (tableau[i][j] != 0)
                    tableau[i][j] = -tableau[i][j];
            }
        }
    }
}

void CanonizaComAuxiliar(double *tableau[], double *auxiliar[], int n, int m)
{
    int soma_0 = 0;
    int soma_1 = 0;
    int indice_1 = -1;
    bool base = false;

    for (int j = n - 1; j < m - 1; j++)
    {
        soma_0 = 0;
        soma_1 = 0;
        indice_1 = -1;
        base = false;
        for (int i = 0; i < n; i++)
        {
            if (abs(auxiliar[i][j] - 1) <= EPS || abs(auxiliar[i][j]) <= EPS)
            {
                if (abs(auxiliar[i][j] - 1) <= EPS)
                {
                    soma_1++;
                    indice_1 = i;
                }
                else
                {
                    soma_0++;
                }
            }
            else
            {
                base = false;
            }
        }

        if (soma_0 == n - 1 and soma_1 == 1)
        {
            base = true;
        }
        else
        {
            base = false;
        }

        if (auxiliar[0][j] != 0)
            base = false;

        if (base)
        {
            Pivoteia(tableau, n, m, indice_1, j);
        }
    }
}

void SimplexCanonica(double *tableau[], int n, int m, int *indice_ilimitada)
{
    int indice_min;
    for (int j = n - 1; j < m - 1; j++)
    {
        if (tableau[0][j] < 0)
        {
            indice_min = EncontraMenorRazao(tableau, n, m, j);
            if (indice_min > 0)
            {
                Pivoteia(tableau, n, m, indice_min, j);
                j = n - 2;
            }
            else
            {
                *indice_ilimitada = j;
                return;
            }
        }
    }
}

double *EncontraCertificado(double *tableau[], int n, int m, int *indice_ilimitada)
{
    double *certificado = new double[m - n]; // certificado[0] = i-n+1
    for (int i = 0; i < m - n; i++)
    {
        certificado[i] = 0;
    }
    certificado[*indice_ilimitada - n + 1] = 1;
    int soma_0 = 0;
    int soma_1 = 0;
    int indice_1 = -1;
    bool base = false;

    for (int j = n - 1; j < m - 1; j++)
    {
        soma_0 = 0;
        soma_1 = 0;
        indice_1 = -1;
        base = false;
        for (int i = 0; i < n; i++)
        {
            if (abs(tableau[i][j] - 1) <= EPS || abs(tableau[i][j]) <= EPS)
            {
                if (abs(tableau[i][j] - 1) <= EPS)
                {
                    soma_1++;
                    indice_1 = i;
                }
                else
                {
                    soma_0++;
                }
            }
            else
            {
                base = false;
            }
        }

        if (soma_0 == n - 1 and soma_1 == 1)
        {
            base = true;
        }
        else
        {
            base = false;
        }

        if (tableau[0][j] != 0)
            base = false;

        if (base)
        {
            certificado[j - n + 1] = -(tableau[indice_1][*indice_ilimitada]);
        }
    }
    return certificado;
}

double *EncontraSolucao(double *tableau[], int n, int m)
{
    double *solucao = new double[m - n];
    for (int i = 0; i < m - n; i++)
    {
        solucao[i] = 0;
    }
    int soma_0 = 0;
    int soma_1 = 0;
    int indice_1 = -1;
    bool base = false;

    for (int j = n - 1; j < m - 1; j++)
    {
        soma_0 = 0;
        soma_1 = 0;
        indice_1 = -1;
        base = false;
        for (int i = 0; i < n; i++)
        {
            if (abs(tableau[i][j] - 1) <= EPS || abs(tableau[i][j]) <= EPS)
            {
                if (abs(tableau[i][j] - 1) <= EPS)
                {
                    soma_1++;
                    indice_1 = i;
                }
                else
                {
                    soma_0++;
                }
            }
            else
            {
                base = false;
            }
        }

        if (soma_0 == n - 1 and soma_1 == 1)
        {
            base = true;
        }
        else
        {
            base = false;
        }

        if (tableau[0][j] != 0)
            base = false;

        if (base)
        {
            solucao[j - n + 1] = tableau[indice_1][m - 1];
        }
    }
    return solucao;
}

void Simplex(double *tableau[], int n, int m)
{
    double **tableauFolgas = new double *[n];
    adicionaFolga(tableauFolgas, tableau, n, m);
    int m_folga = m + n - 1;

    double **auxiliar = new double *[n];
    int n_auxiliar = n;
    int m_auxiliar = m_folga + n - 1;
    Positiva_B(tableauFolgas, n, m_folga);
    MontaAux(auxiliar, tableauFolgas, n, m_folga);

    int *indice_ilimitada = new int();
    *indice_ilimitada = -1;
    SimplexCanonica(auxiliar, n_auxiliar, m_auxiliar, indice_ilimitada);
    if (abs(auxiliar[0][m_auxiliar - 1]) < 0.000000001)
    {
        CanonizaComAuxiliar(tableauFolgas, auxiliar, n, m_folga);
        *indice_ilimitada = -1;
        SimplexCanonica(tableauFolgas, n, m_folga, indice_ilimitada);
        double *solucao = EncontraSolucao(tableauFolgas, n, m_folga);
        if (*indice_ilimitada > -1)
        {
            double *certificado = EncontraCertificado(tableauFolgas, n, m_folga, indice_ilimitada);
            cout << "ilimitada " << endl;
            for (int i = 0; i < m-n; i++)
            {
                cout << solucao[i] << " ";
            }
            cout << endl;
            for (int i = 0; i < m-n; i++)
            {
                cout << certificado[i] << " ";
            }
            cout << endl;
        }
        else
        {
            cout << "otima" << endl
                 << tableauFolgas[0][m_folga - 1] << endl;
            for (int i = 0; i < m-n; i++)
            {
                cout << solucao[i] << " ";
            }
            cout << endl;
            for (int i = 0; i < n-1; i++)
            {
                cout << tableauFolgas[0][i] << " ";
            }
            cout << endl;
        }
    }
    else
    {
        cout << "inviavel" << endl;
        for (int i = 0; i < n - 1; i++)
        {
            cout << auxiliar[0][i] << " ";
        }
        cout << endl;
        return;
    }
}

int main()
{
    int n = 0;
    int m = 0;
    cin >> n;
    cin >> m;
    cout << setprecision(7) << fixed;

    double **tableau = new double *[n + 1];
    for (int i = 0; i < n + 1; i++)
        tableau[i] = new double[n + m + 1];

    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < n + m + 1; j++)
        {
            tableau[i][j] = 0;
        }
    }

    for (int i = 0; i < n + 1; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < m; j++)
            {
                cin >> tableau[i][j + n];
                if (tableau[i][j + n] != 0)
                    tableau[i][j + n] = -tableau[i][j + n];
            }
        }
        else
        {
            for (int j = 0; j < m + 1; j++)
            {
                cin >> tableau[i][j + n];
                tableau[i][i - 1] = 1;
            }
        }
    }

    Simplex(tableau, n + 1, n + m + 1);
}