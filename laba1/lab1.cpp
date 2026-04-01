// ¬ходные данные:
// текстовый файл, в первой строке Ц целое число - количество вершин графа n (5 ? n ? 100).
//  ажда€ из последующих строк содержит данные внутреннего представлени€ графа (см. свой вариант задани€).

// –еализовать дл€ графа заданного типа (см. свой вариант задани€), представленным заданным видом (см. свой вариант задани€):
// 1. ќбход (DFS non-recursive)
// 2. јлгоритм (Prim's algorithm)

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

void print_graph(const vector<vector<int>> graph);
void print_vector(const vector<int> vect);

vector<vector<int>> read_graph(int &n, const string filename)
{
    vector<vector<int>> graph;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "An Error occured during file opening process" << endl;
        return {};
    }

    n = 0;
    file >> n;

    for (int i = 0; i < n; i++)
    {
        vector<int> line;
        for (int j = 0; j < n; j++)
        {
            int weight;
            file >> weight;
            line.push_back(weight);
        }
        graph.push_back(line);
    }

    return graph;
}

int sum_vec(const vector<int> &v)
{
    int s = 0;
    for (int x : v)
        s += x;
    return s;
}
vector<vector<int>> create_ostov(const vector<vector<int>> &graph)
{
    int n = graph.size();
    if (n == 0)
        return {};

    vector<vector<int>> ostov(n, vector<int>(n, -1));
    for (int i = 0; i < n; i++)
        ostov[i][i] = 0;

    set<int> added_vertices;
    set<int> free_vertices;

    for (int i = 0; i < n; i++)
    {
        free_vertices.insert(i);
    }

    // Ќачинаем с вершины 0
    added_vertices.insert(0);
    free_vertices.erase(0);

    for (int k = 0; k < n - 1; k++)
    {
        int min_weight = INT_MAX;
        int min_vertex_dest = -1;
        int min_vertex_src = -1;

        // ѕеребор всех ребер между множеством добавленных и свободных
        for (int u : added_vertices)
        {
            for (int v : free_vertices)
            {
                // ѕроверка: ребро должно существовать (>0) и быть меньше текущего минимума
                if (graph[u][v] > 0 && graph[u][v] < min_weight)
                {
                    min_weight = graph[u][v];
                    min_vertex_src = u;
                    min_vertex_dest = v;
                }
            }
        }

        // ≈сли граф несв€зный, мы не найдем ребра
        if (min_vertex_dest == -1)
        {
            cout << "Warning: Graph is disconnected." << endl;
            break;
        }

        // ƒобавл€ем ребро в остов
        ostov[min_vertex_src][min_vertex_dest] = min_weight;
        ostov[min_vertex_dest][min_vertex_src] = min_weight;

        // ѕереносим вершину из свободного множества в добавленное
        added_vertices.insert(min_vertex_dest);
        free_vertices.erase(min_vertex_dest);
    }

    return ostov;
}

int find_unvisited(const vector<int> &visited_vertices)
{
    for (int i = 0; i < visited_vertices.size(); ++i)
    {
        if (!visited_vertices[i])
            return i;
    }
    return -1;
}
void analyse_graph(const vector<vector<int>> graph)
{
    int n = graph.size();
    if (n == 0)
    {
        cout << "Empty graph." << endl;
        return;
    }

    bool connected = true;
    bool has_loop = false;

    vector<int> visited_vertices(n, 0); // 0 - не посещен, 1 - посещен
    int total_visited_count = 0;
    int start_vertex = 0;

    // ¬нешний цикл дл€ обработки компонент св€зности
    while (total_visited_count < n)
    {
        stack<pair<int, int>> stac; // ’раним {текуща€_вершина, родитель_вершина}

        // Ќаходим следующую непосещенную вершину, если это не первый запуск
        if (total_visited_count > 0)
        {
            start_vertex = find_unvisited(visited_vertices);
            if (start_vertex == -1)
                break;
        }

        // ѕомечаем как посещенную сразу при добавлении в стек, чтобы избежать дублей
        visited_vertices[start_vertex] = 1;
        total_visited_count++;
        stac.push({start_vertex, -1}); // -1 означает отсутствие родител€

        while (!stac.empty())
        {
            auto [current, parent] = stac.top();
            stac.pop();

            // cout << current << " "; // –аскомментировать дл€ трассировки

            for (int neighbor = 0; neighbor < n; ++neighbor)
            {
                if (graph[current][neighbor] > 0)
                { // ≈сли есть ребро
                    if (neighbor == parent)
                    {
                        continue; // »гнорируем ребро назад к родителю (это не цикл)
                    }

                    if (visited_vertices[neighbor])
                    {
                        // ≈сли сосед посещен и это не родитель -> найден цикл
                        has_loop = true;
                    }
                    else
                    {
                        // ≈сли не посещен -> добавл€ем в стек
                        visited_vertices[neighbor] = 1;
                        total_visited_count++;
                        stac.push({neighbor, current});
                    }
                }
            }
        }

        // ≈сли после завершени€ обхода компоненты мы посетили меньше N вершин,
        // значит граф несв€зный (но мы продолжим цикл, чтобы проверить остальные)
        if (total_visited_count < n)
        {
            connected = false;
            // —ледующа€ итераци€ while сама найдет новый start_vertex через find_unvisited
        }
    }

    cout << "Graph is " << (connected ? "connected" : "disconnected") << endl;
    cout << "Graph has " << (has_loop ? "loop(s)" : "no loops") << endl;
}

void print_vector(const vector<int> vect)
{
    int n = vect.size();
    for (int i = 0; i < n; i++)
    {
        cout << vect[i] << ' ';
    }
    cout << endl;
}

void print_graph(const vector<vector<int>> graph)
{
    int n = graph.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {

            if (graph[i][j] == -1)
                cout << "__ ";
            else
            {
                if (graph[i][j] <= 9)
                    cout << ' ';
                cout << graph[i][j] << ' ';
            }
        }
        cout << '\n';
    }
    cout << endl;
}

int main()
{
    // - граф -
    // пусть рассто€ние из вершины в саму себ€ = 0
    // если вершины не соединены рассто€ние = -1

    // - init -
    int n = 0; // - кол-во вершин (5 <= n <= 100)
    vector<vector<int>> graph = read_graph(n, "graph_file.txt");
    print_graph(graph);

    // - prim -
    vector<vector<int>> ostov = create_ostov(graph);
    print_graph(ostov);

    // - depth-first search -
    analyse_graph(graph);
    analyse_graph(ostov);
}