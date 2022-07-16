#include <iostream>
#include <list>
#include <vector>
#include <set>

#define EPS 0.0000001

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

bool testaEmparelhamento(bool **matriz, int n, int m, vector<int> &caminho, vector<int> &folhas)
{
    caminho.clear();
    folhas.clear();
    bool *emparelhamento_M = new bool[m];
    bool *emparelhado = new bool[2 * n];
    int *antecessor = new int[2 * n];
    bool *visitado = new bool[2 * n];
    int v = -1;
    bool e_folha = true;
    list<int> fila;

    /* EMPARELHAMENTO M VAZIO */
    for (int i = 0; i < m; i++)
    {
        emparelhamento_M[i] = 0;
    }
    for (int i = 0; i < 2 * n; i++)
    {
        emparelhado[i] = 0;
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
    e_folha = false;
    /* VERIFICA SE HÁ VÉRTICE v EM A AINDA NÃO EMPARELHADO */
    int contador = 0;
    while (true)
    {
        /* LEMBRAR DE VERIFICAR NOVAMENTE OS EMPARELHAMENTOS */
        cout << "Matriz H: " << endl;
        imprimeMatriz(matriz, n, m);
        imprimeEmparelhamento(emparelhamento_M, emparelhado, n, m);
        v = -1;
        for (int i = 0; i < n; i++)
        {
            if (!emparelhado[i])
            {
                v = i;
                cout << "Vértice " << v << " não emparelhado" << endl;
                break;
            }
        }
        if (v == -1)
        {
            return true;
        }

        /* VISITA v */
        antecessor[v] = -1;
        visitado[v] = 1;

        // cout << "Adicionando " << v << " ao caminho" << endl;
        // caminho.push_back(v);

        /* EXECUTAR BFS A PARTIR DE v */
        e_folha = true;
        for (int i = 0; i < m; i++)
        {
            /* ENCONTRA VIZINHO EM B */
            if (matriz[v][i] == 1)
            {
                e_folha = false;

                for (int j = n; j < 2 * n; j++)
                {
                    /* ENCONTRA VÉRTICE j VIZINHO DE v */
                    if (matriz[j][i] == 1)
                    {
                        /* INSERE j NA FILA */
                        // cout << "Inserindo Vértice " << j << " na fila" << endl;
                        antecessor[j] = v;
                        visitado[j] = 1;
                        // cout << "Adicionando " << j << " na fila" << endl;
                        fila.push_back(j);
                    }
                }
            }
        }

        if (e_folha)
        {
            // cout << "Adicionando " << v << " às folhas" << endl;
            folhas.push_back(v);
        }

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
                            visitado[j] = 1;
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
                            visitado[j] = 1;
                            fila.push_back(j);
                        }
                    }
                }
            }
            if (e_folha)
            {
                // cout << "Folha " << v_atual << " encontrada, inserindo nas folhas" << endl;
                folhas.push_back(v_atual);
            }
            fila.pop_front();
        }

        for (int i = 0; i < (int)folhas.size(); i++)
        {
            int j = folhas[i];
            while (j != -1)
            {
                caminho.push_back(j);
                j = antecessor[j];
            }
        }
        // cout << "Fim da BFS" << endl;

        cout << "Caminho encontrado: " << endl;
        for (auto const &i : caminho)
        {
            cout << i << " ";
        }
        cout << endl;

        cout << "Folhas: " << endl;
        for (auto const &i : folhas)
        {
            cout << i << " ";
        }
        cout << endl;

        int folha_em_b = -1;

        /* VERIFICANDO SE ALGUMA FOLHA ESTÁ EM B */
        for (int i = 0; i < (int)folhas.size(); i++)
        {
            if (folhas[i] >= n)
            {
                folha_em_b = (int)folhas[i];
            }
        }
        /* SE NÃO HÁ FOLHA EM B */
        if (folha_em_b == -1)
        {
            cout << "Todas folhas em A, retornando falso." << endl;
            return false;
        }
        cout << "Alguma folha está em B, calculando caminho" << endl;
        cout << "Caminho encontrado: " << endl;
        caminho.clear();
        int j = folha_em_b;
        while (j != -1)
        {
            caminho.push_back(j);
            j = antecessor[j];
        }
        for (auto const &i : caminho)
        {
            cout << i << " ";
        }
        cout << endl;

        cout << "Folhas: " << endl;
        for (auto const &i : folhas)
        {
            cout << i << " ";
        }
        /* Daı́ você inverte as arestas de M que estão em P com as que não
        estão. O resultado será um emparelhamento maior (por que?). Daı́
        você repete, etc. */
        cout << "Executando inversão das arestas" << endl;
        for (int i = 0; i < (int)caminho.size() - 1; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (emparelhamento_M[j])
                {
                    if (matriz[caminho[i]][j] and matriz[caminho[i + 1]][j])
                    {
                        emparelhamento_M[j] = false;
                        for (int k = 0; k < 2 * n; k++)
                        {
                            matriz[k][j] = 0;
                        }
                    }
                }
                else if (matriz[caminho[i]][j] and matriz[caminho[i + 1]][j]){
                    emparelhamento_M[j] = true;
                }
            }
        }
        cout << endl;
        /* FIM DA BFS */
        contador++;
    }
    return false;
}

int main()
{
    int n = 0;
    int m = 0;
    cin >> n;
    cin >> m;

    bool **matriz_N = new bool *[2 * n];
    bool **matriz_H = new bool *[2 * n];
    bool *isinS = new bool[2 * n];
    bool *nhS = new bool[2 * n];
    vector<int> caminho;
    vector<int> folhas;
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

    int contador = 0;

    while (true)
    {
        cout << "y: " << endl;
        for (int i = 0; i < 2 * n; i++)
        {
            cout << array_y[i] << " ";
        }
        cout << endl
             << "Iteração " << contador << endl;

        cout << "Matriz N: " << endl;
        imprimeMatriz(matriz_N, n, m);

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
            if (abs(custos[i] - soma_y) < EPS)
            {
                for (int j = 0; j < 2 * n; j++)
                {
                    matriz_H[j][i] = matriz_N[j][i];
                }
            }
        }

        cout << "Custos: " << endl;
        for (int i = 0; i < m; i++)
        {
            cout << custos[i] << " ";
        }
        cout << endl;

        /*
        TESTANDO SE HÁ EMPARELHAMENTO PERFEITO
        */
        cout << "Inicio do teste de emparelhamento perfeito" << endl;
        if (!testaEmparelhamento(matriz_H, n, m, caminho, folhas))
        {
            set_S.clear();
            vizinhos_S_G.clear();
            for (auto const &i : caminho)
            {
                if (i < n)
                {
                    // cout << "Adicionando " << i << " Ao subset S " << endl;
                    set_S.insert(i);
                }
            }
            // Criando vetores para saber se um vértice está em S ou nos vizinhos de S em H
            for (int i = 0; i < 2 * n; i++)
            {
                isinS[i] = false;
                nhS[i] = false;
            }

            for (auto const &i : caminho)
            {
                if (i < n)
                {
                    isinS[i] = true;
                }
                else
                {
                    nhS[i] = true;
                }
            }
            /* Encontrando vizinhos do subset S em G  */
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
                                // cout << "Vértice " << *it << " tem vizinho em G " << j << endl;
                                vizinhos_S_G.insert(j);
                            }
                        }
                    }
                }
            }

            cout << "Conjunto S: ";

            for (it = set_S.begin(); it != set_S.end(); it++)
            {
                cout << *it << " ";
            }

            cout << endl;

            /*

            cout << "Vizinhos de S em G: ";

            for (it = vizinhos_S_G.begin(); it != vizinhos_S_G.end(); it++)
            {
                cout << *it << " ";
            }
            cout << endl;*/
            if (vizinhos_S_G.size() < set_S.size())
            {
                cout << "Não há emparelhamento perfeito" << endl;
                return 0;
            }

            /* Escolha eps maior possı́vel que permita aumentar a variável y de cada vértice de S por
            eps, diminuir a variável y de cada vértice de NH (S) por eps, e deixar as demais constantes;
            mas garantindo que y permaneça viável em G.
            */

            double epsilon = 10000000000;
            int k = 0;
            double lado_dir = 0;

            for (int i = 0; i < m; i++)
            {
                k = 0;
                lado_dir = custos[i];
                for (int j = 0; j < 2 * n; j++)
                {

                    if (matriz_N[j][i])
                    {
                        lado_dir -= array_y[j];
                        if (isinS[j])
                            k++;
                        if (nhS[j])
                            k--;
                    }
                }
                if (k > 0)
                {
                    if (lado_dir < epsilon)
                        epsilon = lado_dir;
                }
            }
            cout << "Eps: " << epsilon << endl;
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
        }
        else
        {
            cout << "O emparelhamento é perfeito" << endl;
            return 0;
        }
        contador++;
    }
}
