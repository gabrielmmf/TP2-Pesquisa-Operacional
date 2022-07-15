#include <iostream>
#include <list>
#include <vector>

#define EPS 0.000001

using namespace std;

void imprimeEmparelhamento(bool *emparelhamento_M, bool *emparelhado, int n, int m)
{
    cout << endl
         << "Emparelhamentos: " << endl;
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

bool testaEmparelhamento(bool **matriz, int n, int m, list<int> &caminho, vector<int> &folhas)
{
    bool *emparelhamento_M = new bool[m];
    bool *emparelhado = new bool[2 * n];
    int *antecessor = new int[2 * n];
    bool *visitado = new bool[2 * n];
    int tamanho_emparelhamento = 0;
    int v = -1;
    bool e_folha = true;
    bool v_folha = true;
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
                        tamanho_emparelhamento++;
                    }
                }
            }
        }
    }

    /* Agora você já tem um M maior, e assuma então que tem vértice
    v não emparelhado em A mas todos os seus vizinhos em B estão
    emparelhados por M com outros vértices de A. */
    e_folha = false;
    v_folha = false;
    /* VERIFICA SE HÁ VÉRTICE v EM A AINDA NÃO EMPARELHADO */
    while (true)
    {
        /* LEMBRAR DE VERIFICAR NOVAMENTE OS EMPARELHAMENTOS */
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

        cout << "Adicionando " << v << " ao caminho" << endl;
        caminho.push_back(v);

        /* EXECUTAR BFS A PARTIR DE v */
        v_folha = true;
        for (int i = 0; i < m; i++)
        {
            /* ENCONTRA VIZINHO EM B */
            if (matriz[v][i] == 1)
            {
                v_folha = false;

                for (int j = n; j < 2 * n; j++)
                {
                    /* ENCONTRA VÉRTICE j VIZINHO DE v */
                    if (matriz[j][i] == 1)
                    {
                        /* INSERE j NA FILA */
                        cout << "Inserindo Vértice " << j << " na fila" << endl;
                        antecessor[j] = v;
                        visitado[j] = 1;
                        cout << "Adicionando " << j << " na fila" << endl;
                        fila.push_back(j);
                    }
                }
            }
        }

        if (v_folha)
        {
            cout << "Adicionando " << v << " às folhas" << endl;
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
                cout << "Folha " << v << " encontrada, inserindo nas folhas" << endl;
                folhas.push_back(v_atual);
            }
            fila.pop_front();
        }
        cout << "Fim da BFS" << endl;
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

        int folha = -1;

        /* VERIFICANDO SE ALGUMA FOLHA ESTÁ EM B */
        for (int i = 0; i < (int)folhas.size(); i++)
        {
            if (folhas[i] >= n)
            {
                folha = folhas[i];
            }
        }
        /* SE NÃO HÁ FOLHA EM B */
        if (folha == -1)
        {
            cout << "Todas folhas em A, retornando falso." << endl;
            return false;
        }
        cout << "Alguma folha está em B, calculando caminho" << endl;
        int i = folha;
        while (antecessor[i] != -1)
        {
            caminho.push_back(i);
            i = antecessor[i];
        }

        
        /* FIM DA BFS */
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
    list<int> caminho;
    vector<int> folhas;
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

    for (int i = 0; i < 2 * n; i++)
    {
        cout << array_y[i] << " ";
    }
    cout << endl;

    /*
    SELECIONAMOS AS ARESTAS DE CUSTO MÍNIMO E COLOCAMOS NO GRAFO H
    */

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

    cout << endl;
    for (int i = 0; i < m; i++)
    {
        cout << custos[i] << " ";
    }
    cout << endl;
    imprimeMatriz(matriz_H, n, m);
    /*
    TESTANDO SE HÁ EMPARELHAMENTO PERFEITO
    */
    testaEmparelhamento(matriz_H, n, m, caminho, folhas);
}