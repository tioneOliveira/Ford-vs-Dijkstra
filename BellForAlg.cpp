#include <iostream>
#include <limits.h>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

// Estutura para as arestas.
struct Edge
{
    int source, destination, weight;
};

// Calculo da distância minima entre entre a fonte e o destino
int minDistance(const vector<int> &dist, const vector<bool> &finalizado, int V)
{
    // Inicia o valor mínimo como infinito
    int min = INT_MAX;
    // Armazena o índice do vértice com menor distância
    int min_index = -1;

    // Percorre todos os vértices do grafo
    for (int v = 0; v < V; v++)
    {
        // Se o vértice ainda não foi finalizado e tem distância menor ou igual ao mínimo atual
        if (!finalizado[v] && dist[v] <= min)
        {
            // Atualiza o valor mínimo e o índice correspondente
            min = dist[v];
            min_index = v;
        }
    }

    // Retorna o índice do vértice com a menor distância não finalizado
    // Retorna -1 se não houver (ou seja, todos foram finalizados)
    return min_index;
}

vector<int> dijkstra(const vector<vector<int>> &graph, int V, int src)
{
    // Vetor de distâncias: começa tudo com "infinito"
    vector<int> dist(V, INT_MAX);

    // Vetor de marcação: true se o vértice já foi finalizado (visitado)
    vector<bool> finalizado(V, false);

    // Distância da fonte até ela mesma é zero
    dist[src] = 0;

    // Loop principal: repete até processar todos os vértices
    for (int count = 0; count < V; count++)
    {
        // Encontra o vértice u com a menor distância ainda não finalizado
        int u = minDistance(dist, finalizado, V);
        // Se não existe mais vértice alcançável, encerra o loop
        if (u == -1)
            break;

        // Marca o vértice u como finalizado
        finalizado[u] = true;

        // Atualiza as distâncias dos vértices vizinhos de u
        for (int v = 0; v < V; v++)
        {
            if (!finalizado[v] && graph[u][v] != 0 &&
                dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
            }
            cout << "After iteration " << count + 1 << ": ";
            for (int i = 0; i < V; ++i)
            {
                if (dist[i] == INT_MAX)
                    cout << "INF";
                else
                    cout << dist[i];
                if (i < V - 1)
                    cout << " ";
            }
            cout << endl;
            {
            }
        }
    }

    cout << "Vertex   Distance from Source\n";
    for (int i = 0; i < V; i++)
    {
        cout << i << "\t\t";
        if (dist[i] == INT_MAX)
            cout << "INF\n";
        else
            cout << dist[i] << "\n";
    }
    return dist;
}

vector<int> bellmanFord(const vector<Edge> &edges, int V, int src)
{
    vector<int> distance(V, INT_MAX);
    distance[src] = 0;

    for (int i = 0; i < V - 1; ++i)
    {
        for (const Edge &e : edges)
        {
            if (distance[e.source] != INT_MAX &&
                distance[e.source] + e.weight < distance[e.destination])
            {
                distance[e.destination] = distance[e.source] + e.weight;
            }
            cout << "After iteration " << i + 1 << ": ";
            for (int k = 0; k < V; ++k)
            {
                if (distance[k] == INT_MAX)
                    cout << "INF";
                else
                    cout << distance[k];
                if (k < V - 1)
                    cout << " ";
            }
            cout << endl;
        }
    }

    for (const Edge &e : edges)
    {
        if (distance[e.source] != INT_MAX &&
            distance[e.source] + e.weight < distance[e.destination])
        {
            cout << "Graph contains a negative weight cycle.\n";
        }
    }

    cout << "Vertex   Distance from Source\n";
    for (int i = 0; i < V; ++i)
    {
        cout << i << "\t\t";
        if (distance[i] == INT_MAX)
            cout << "INF\n";
        else
            cout << distance[i] << "\n";
    }
    return distance;
}

vector<vector<int>> graphToMatrix(vector<Edge> graph, int vertices)
{
    vector<vector<int>> graphM(vertices, vector<int>(vertices, 0));
    for (const Edge &e : graph)
    {
        graphM[e.source][e.destination] = e.weight;
    }
    return graphM;
}

string diferences(vector<int> bellF, vector<int> dij)
{
    int sizeVec = bellF.size();
    string errs = "";

    for (int i = 0; i < sizeVec; i++)
    {
        if (bellF[i] != dij[i])
        {
            errs += to_string(i) + ", ";
        }
    }
    if (errs == "")
    {
        return "There is no diferences!";
    }
    else
    {
        return errs;
    }
}

void test(vector<vector<Edge>> graphs, vector<int> vertices)
{
    int sizeGraphs = graphs.size();
    for (int i = 0; i < sizeGraphs; i++)
    {
        cout << "===================================================\nGrafo " << i + 1 << endl;
        vector<vector<int>> graphM = graphToMatrix(graphs[i], vertices[i]);

        auto inicio = chrono::high_resolution_clock::now();
        cout << "===== Bellman-Ford =====\n";
        vector<int> bf = bellmanFord(graphs[i], vertices[i], 0);
        auto fim = chrono::high_resolution_clock::now();
        chrono::duration<double> duracao = fim - inicio;
        cout << "Tempo de execucao: " << duracao.count() << " segundos" << endl;

        inicio = chrono::high_resolution_clock::now();
        cout << "===== Dijkstra =====\n";
        vector<int> dij = dijkstra(graphM, vertices[i], 0);
        fim = chrono::high_resolution_clock::now();
        duracao = fim - inicio;

        cout << "Tempo de execucao: " << duracao.count() << " segundos" << endl;
        cout << "\nDiferencas: " << diferences(bf, dij) << "\n"
             << endl;
    }
}

int main()
{
    int vertices1 = 6;
    vector<Edge> graph1 = {
        {0, 1, 10},
        {0, 5, 8},{1, 3, 2}, {2, 1, 1}, {3, 2, -2}, {4, 1, -4},
        {4, 3, -1},
        {5, 4, 1}
        };

    // // ---------- GRAFO 2 ----------
    // int vertices2 = 10;
    // vector<Edge> graph2 = {
    //     {0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2}, {1, 4, 2}, {3, 2, 5}, {3, 1, 1}, {4, 3, -3}, {4, 5, 2}, {5, 6, -2}, {6, 7, 1}, {7, 8, 3}, {8, 9, -4}, {9, 6, 2}};

    // // ---------- GRAFO 3 ----------
    // int vertices3 = 15;
    // vector<Edge> graph3 = {
    //     {0, 1, 4}, {0, 2, 2}, {0, 3, 5}, {1, 4, -2}, {1, 5, 3}, {2, 1, -3}, {2, 6, 4}, {3, 2, -1}, {3, 7, 2}, {4, 8, 2}, {4, 9, -4}, {5, 4, -1}, {5, 10, 5}, {6, 5, -2}, {6, 11, 3}, {7, 6, -3}, {7, 12, 4}, {8, 13, 1}, {9, 13, 2}, {10, 9, -1}, {10, 14, 3}, {11, 10, -2}, {11, 14, 4}, {12, 11, -3}, {12, 14, 2}, {13, 14, -2}};

    // // ---------- GRAFO 4 (muito maior) ----------
    // int vertices4 = 20;
    // vector<Edge> graph4;
    // for (int i = 0; i < vertices4 - 1; i++)
    // {
    //     graph4.push_back({i, i + 1, (i % 3 == 0) ? -2 : i + 1});
    //     if (i + 2 < vertices4)
    //         graph4.push_back({i, i + 2, (i % 5 == 0) ? -3 : i + 2});
    // }

    // vector<vector<Edge>> graphs = {graph1, graph2, graph3, graph4};
    // vector<int> vertices = {vertices1, vertices2, vertices3, vertices4};

    vector<vector<Edge>> graphs = {graph1};
    vector<int> vertices = {vertices1};

    test(graphs, vertices);

    return 0;
}
