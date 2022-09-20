/*
NOME: GABRIEL MARTINS MEDEIROS FIALHO
MATRÍCULA: 2020006540

ESCOLHIDA PRIMEIRA OPÇÃO: IMPLEMENTAR DIRETAMENTE UM ALGORITMO QUE ENCONTRA EMPARELHAMENTOS PERFEITOS EM GRAFOS BIPARTIDOS
*/

#include <iostream>
#include <list>
#include <vector>
#include <set>

#define EPS 0.000001

using namespace std;

void imprimeEmparelhamento(bool *emparelhamento_M, bool *emparelhado, int n, int m)
{
    cout << "Emparelhamentos: " << endl;
    for (int i = 0; i < m; i++)
    {
        cout << emparelhamento_M[i] << " ";
    }

    cout << endl
         << "Vértices emparelhados: " << endl;

    for (int i = 0; i < 2 * n; i++)
    {
        cout << emparelhado[i] << " ";
    }
    cout << endl;
}

void imprimeMatriz(bool **matriz, int n, int m)
{

    for (int i = 0; i < 2 * n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

int saoVizinhos(bool **matriz, int n, int m, int u, int v)
{
    int aresta = -1;
    for (int i = 0; i < m; i++)
    {
        if (matriz[u][i] and matriz[v][i] and u != v)
        {
            aresta = i;
            cout << u << " e " << v << " são vizinhos na aresta " << aresta << endl;
            return aresta;
        }
    }
    return aresta;
}

bool testaEmparelhamento(bool **matriz, int n, int m, bool *emparelhamento_M, bool *emparelhado, bool *isinS, bool *nhS)
{
    // Criando vetores para saber se um vértice está em S ou nos vizinhos de S em H
    vector<int> caminho;
    caminho.clear();
    for (int i = 0; i < 2 * n; i++)
    {
        isinS[i] = false;
        nhS[i] = false;
    }
    int *antecessor = new int[2 * n];
    bool *visitado = new bool[2 * n];
    int v = -1;
    int folha = -1;
    bool e_folha = false;
    bool folha_em_b = false;
    list<int> fila;
    /* EMPARELHAMENTO M VAZIO */
    for (int i = 0; i < m; i++)
    {
        emparelhamento_M[i] = 0;
    }

    // Resetando tudo
    for (int i = 0; i < 2 * n; i++)
    {
        emparelhado[i] = 0;
        antecessor[i] = 0;
        visitado[i] = 0;
        isinS[i] = 0;
        nhS[i] = 0;
    }

    /* Percorrendo cada vértice de A */
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            /*
            Verifica adjacência com algum vértice de B ainda não emparelhado
            */
            if (matriz[i][j] == 1)
            {
                for (int k = n; k < 2 * n; k++)
                {
                    /*
                    Se é adjacente a algum vértice de B:
                        Adiciona aresta no emparelhamento
                        e diz que os vértices de A e B estão emparelhados
                     */
                    if (matriz[k][j] == 1 and emparelhado[k] == 0 and emparelhado[i] == 0)
                    {
                        emparelhamento_M[j] = 1;
                        emparelhado[i] = 1;
                        emparelhado[k] = 1;
                    }
                }
            }
        }
    }

    /* Agora você já tem um M maior, e assuma então que tem vértice
    v não emparelhado em A mas todos os seus vizinhos em B estão
    emparelhados por M com outros vértices de A. */

    while (true)
    {
        // Limpando emparelhamentos
        for (int i = 0; i < 2 * n; i++)
        {
            emparelhado[i] = 0;
        }

        // Verificando emparelhamentos novamente
        for (int i = 0; i < m; i++)
        {
            if (emparelhamento_M[i])
            {
                for (int j = 0; j < 2 * n; j++)
                {
                    if (matriz[j][i])
                        emparelhado[j] = true;
                }
            }
        }

        /* VERIFICA SE HÁ VÉRTICE v EM A AINDA NÃO EMPARELHADO */
        v = -1;
        for (int i = 0; i < n; i++)
        {
            if (!emparelhado[i])
            {
                v = i;
                break;
            }
        }
        // Se todos os vértices estão emparelhedos, o emparelhamento é perfeito
        if (v == -1)
        {
            return true;
        }

        /* EXECUTAR BFS A PARTIR DE v CASO ESTEJA DESEMPARELHADO */
        antecessor[v] = -1;
        visitado[v] = true;
        fila.push_back(v);
        folha_em_b = false;
        while (fila.size() > 0)
        {
            int v_atual = fila.front();
            e_folha = true;
            for (int i = 0; i < m; i++)
            {
                /* SE O VÉRTICE PERTENCE A A, COLOCA TODOS OS VIZINHOS NAO VISITADOS NA FILA */
                if (matriz[v_atual][i] == 1 and v_atual < n)
                {
                    for (int j = n; j < 2 * n; j++)
                    {
                        if (matriz[j][i] == 1 and !visitado[j])
                        {
                            e_folha = false;
                            antecessor[j] = v_atual;
                            visitado[j] = true;
                            fila.push_back(j);
                        }
                    }
                }
                /* SE O VÉRTICE PERTENCE A B, COLOCA TODOS OS VIZINHOS EMPARELHADOS NAO VISITADOS NA FILA */
                else if (matriz[v_atual][i] == 1 and v_atual >= n)
                {
                    for (int j = 0; j < n; j++)
                    {
                        if (matriz[j][i] == 1 and !visitado[j] and emparelhamento_M[i])
                        {
                            e_folha = false;
                            antecessor[j] = v_atual;
                            visitado[j] = true;
                            fila.push_back(j);
                        }
                    }
                }
            }
            // ARMAZENA FOLHA CASO ELA ESTEJA EM B
            if (e_folha and v_atual >= n)
            {
                folha_em_b = true;
                folha = v_atual;
            }
            fila.pop_front();
        }
        /* VERIFICANDO SE ALGUMA FOLHA ESTÁ EM B */
        if (folha_em_b == false) // RETORNAR FALSO CASO TODAS AS FOLHAS ESTEJAM EM A
        {
            for (int i = 0; i < 2 * n; i++)
            {
                if (i < n)
                {
                    isinS[i] = visitado[i];
                }
                else
                {
                    nhS[i] = visitado[i];
                }
            }
            return false;
        }

        /* CRIANDO CAMINHO */
        int i = folha;
        while (i != -1)
        {
            caminho.push_back(i);
            i = antecessor[i];
        }

        /* Daı́ você inverte as arestas de M que estão em P com as que não
        estão. O resultado será um emparelhamento maior (por que?). Daı́
        você repete, etc. */

        /* EXECUTANDO INVERSÃO DAS ARESTAS */

        bool esta_emparelhada = false;
        for (int i = caminho.size() - 1; i > 0; i--)
        {
            for (int j = 0; j < m; j++)
            {
                if (matriz[caminho[i]][j] and matriz[caminho[i - 1]][j])
                {
                    esta_emparelhada = false;
                    if (emparelhamento_M[j])
                    {
                        emparelhamento_M[j] = false;
                        esta_emparelhada = true;
                        break;
                    }
                    if (!esta_emparelhada)
                    {
                        emparelhamento_M[j] = true;
                    }
                }
            }
        }
        /* FIM DA BFS */
    }
    return true;
}

int main()
{
    int n = 0;
    int m = 0;
    cin >> n;
    cin >> m;

    bool *emparelhamento_M = new bool[m];
    bool *emparelhado = new bool[2 * n];
    bool **matriz_N = new bool *[2 * n];
    bool **matriz_H = new bool *[2 * n];
    bool *isinS = new bool[2 * n];
    bool *nhS = new bool[2 * n];
    vector<int> caminho;
    set<int> set_S;
    set<int> vizinhos_S_G;
    double *custos = new double[m];
    double *array_y = new double[2 * n];
    double soma_y = 0;

    for (int i = 0; i < 2 * n; i++)
    {
        matriz_N[i] = new bool[m];
        matriz_H[i] = new bool[m];
    }

    for (int i = 0; i < 2 * n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            matriz_N[i][j] = 0;
            matriz_H[i][j] = 0;
        }
    }

    for (int i = 0; i < m; i++)
    {
        custos[i] = 0;
    }

    for (int i = 0; i < 2 * n; i++)
    {

        for (int j = 0; j < m; j++)
        {
            cin >> matriz_N[i][j];
        }
    }

    for (int i = 0; i < m; i++)
    {
        cin >> custos[i];
    }

    /*
    ENCONTRANDO CUSTO MÍNIMO
    */

    double custo_min = custos[0];

    for (int i = 1; i < m; i++)
    {
        if (custos[i] < custo_min)
            custo_min = custos[i];
    }

    for (int i = 0; i < 2 * n; i++)
    {
        array_y[i] = (double)custo_min / 2.0;
    }

    while (true)
    {
        /* Seja H um grafo com os mesmos vértices de G, e cujas arestas são aquelas uv ∈ E(G)
        tais que y u + y v = cuv . */
        for (int i = 0; i < m; i++)
        {
            soma_y = 0;
            for (int j = 0; j < 2 * n; j++)
            {
                if (matriz_N[j][i])
                {
                    soma_y = soma_y + array_y[j];
                }
            }
            if (abs(custos[i] - soma_y) <= EPS)
            {
                for (int j = 0; j < 2 * n; j++)
                {
                    matriz_H[j][i] = matriz_N[j][i];
                }
            }
        }
        /*
        TESTANDO SE HÁ EMPARELHAMENTO PERFEITO NO GRAFO H
        */
        if (!testaEmparelhamento(matriz_H, n, m, emparelhamento_M, emparelhado, isinS, nhS))
        {
            set_S.clear();
            vizinhos_S_G.clear();
            for (int i = 0; i < n; i++)
            {
                if (isinS[i])
                {
                    set_S.insert(i);
                }
            }

            /* Encontrando vizinhos do subset S no grafo G  */
            set<int>::iterator it;
            for (it = set_S.begin(); it != set_S.end(); it++)
            {
                for (int i = 0; i < m; i++)
                {
                    if (matriz_N[*it][i])
                    {
                        for (int j = n; j < 2 * n; j++)
                        {
                            if (matriz_N[j][i])
                            {
                                vizinhos_S_G.insert(j);
                            }
                        }
                    }
                }
            }

            /* Se |NG (S)| < |S|, PARE. Neste caso, não existe qualquer emparelhamento perfeito em
            G (por que?!) */
            if (vizinhos_S_G.size() < set_S.size())
            {
                cout << -1 << endl;
                for (int i = 0; i < n; i++)
                {
                    cout << isinS[i] << " ";
                }
                cout << endl;
                for (int i = n; i < 2 * n; i++)
                {
                    cout << nhS[i] << " ";
                }
                cout << endl;
                return 0;
            }

            /* Escolha eps maior possı́vel que permita aumentar a variável y de cada vértice de S por
            eps, diminuir a variável y de cada vértice de NH (S) por eps, e deixar as demais constantes;
            mas garantindo que y permaneça viável em G.
            */

            double epsilon = 1.7976931348623157E+308;
            int k = 0;
            double minimiza_eps = 0;

            for (int i = 0; i < m; i++)
            {
                k = 0;
                minimiza_eps = custos[i];
                for (int j = 0; j < 2 * n; j++)
                {
                    if (matriz_N[j][i])
                    {
                        minimiza_eps -= array_y[j];
                        if (isinS[j])
                            k++;
                        if (nhS[j])
                            k--;
                    }
                }
                if (k > 0)
                {
                    if (minimiza_eps < epsilon)
                        epsilon = minimiza_eps;
                }
            }

            for (int i = 0; i < 2 * n; i++)
            {
                if (isinS[i])
                {
                    array_y[i] += epsilon;
                }
                else if (nhS[i])
                {
                    array_y[i] -= epsilon;
                }
            }
            for (int i = 0; i < 2 * n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    matriz_H[i][j] = 0;
                }
            }
        }
        // POSSUI EMPARELHAMENTO PERFEITO:
        else
        {
            int soma = 0;
            for (int i = 0; i < m; i++)
            {
                if (emparelhamento_M[i])
                    soma += custos[i];
            }
            cout << soma << endl;

            for (int i = 0; i < m; i++)
            {
                cout << emparelhamento_M[i] << " ";
            }
            cout << endl;

            for (int i = 0; i < 2 * n; i++)
            {
                cout << array_y[i] << " ";
            }
            cout << endl;
            return 0;
        }
    }
}