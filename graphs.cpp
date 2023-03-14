
// classes: Graph, meshGraph
// weight generate (norm. dist.)
// use typedefs: Edge, Edgelist etc.

// TODO mean, stddev -> float 
// TODO meshGraph -> GridGraph etc.
// TODO use addEdgeRandom() instd. of addEdgeRandom() +
// TODO move coordsInit to constructor?
// TODO randomNumGen -> uniNumGen

#include <iostream>
#include <vector>
#include <random>

using namespace std;

typedef float point2D[2] ;

float manh_dist (point2D& p1, point2D& p2) {
    return (abs(p1[0]-p2[0])+abs(p1[1]-p2[1]));
}

int randNumGen(int _n) { // используется для номеров вершин
    int n;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0,_n);
    n = distrib(gen);
    return n;
}

int normNumGen(int m, int d) { // используется для генерации координат
    int n;
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> distrib(m,d);

    n = distrib(gen);
    return n;
}



class Graph { // на вход получаем M, N и E

    public:
        int V;
        int E;
        int mean;
        int stddev;
        point2D *coordsArr;

        vector<pair<int, int> > *adj;

        Graph() { // элементарный граф из одной вершины
            V = 1;
            E = 0;
            mean = 5;
            stddev = 2;
            coordsArr = new point2D[V];
            adj = new vector<pair<int, int> >[V];
        }

        Graph(int _V, int _E, int _mean, int _stddev) {
            V = _V;
            E = _E;
            mean = _mean;
            stddev = _stddev;
            coordsArr = new point2D[V];
            adj = new vector<pair<int, int> >[V];
        }
        
        void coordsInit() {
            for (int i = 0; i < V; i++) {
                coordsArr[i][0] = normNumGen(mean,stddev);
                coordsArr[i][1] = normNumGen(mean,stddev);
            }
        }

        void addEdgeWeight(int u, int v, int weight) {
            adj[u].push_back(make_pair(v, weight));
            adj[v].push_back(make_pair(u, weight));
        }

        void addEdgeRandom(int u, int v) {
            float weightMD = manh_dist(coordsArr[u],coordsArr[v]);
            adj[u].push_back(make_pair(v, weightMD));
            adj[v].push_back(make_pair(u, weightMD));
        }

    virtual void spanningTree() = 0; // виртуальная функция

    // Вывести список смежности
        void printGraph(int V)
        {
            int v, w;
            for (int u = 0; u < V; u++)
            {
                cout << "Node " << u << " makes an edge with \n";
                for (auto it = adj[u].begin(); it!=adj[u].end(); it++)
                {
                    v = it->first;
                    w = it->second;
                    cout << "\tNode " << v << " with edge weight ="
                        << w << "\n";
                }
                cout << "\n";
            }
        }

};

class meshGraph : public Graph {
    public:
        int M;
        int N;

    meshGraph() {
        N = 3;
        M = 3;
        V = N*M;
        E = 10;
        mean = 5;
        stddev = 2;
        adj = new vector<pair<int, int> >[V];
        coordsArr = new point2D[V];
    }

    meshGraph(int _N, int _M, int _E, int _mean, int _stddev) { // : Graph(_E) - мб так проще?
        N = _N;
        M = _M;
        V = N*M;
        E = _E;
        mean = _mean;
        stddev = _stddev;
        adj = new vector<pair<int, int> >[V];
        coordsArr = new point2D[V];
    }

    bool hasEdge(int _v1, int _v2) {
        int i = 0;
        int len = adj[_v1].size();
        //cout << len;
        while (i < len ) {
            if (adj[_v1][i].first == _v2)
                return true;
            i++;
        }
        return false;
    }

    void spanningTree () {
        int  i = 0;
        while (i<N*M-1) {
            if (((i+1)%(2*M)) != 0) { // если не точка перехода типа 9, 19, 29 при M=5 (смежности 2)
                if ((i / M) % 2 == 0) { // не "грабли" (ЗЕЛЕНЫЕ)
                    if ((i+1)%M != 0) { // не крайние ЗЕЛЕНЫЕ по линии возрастания
                        addEdgeRandom(i, i+1);
                    }
                    else {
                    }
                    if (i + M < N*M) { // вообще говоря, это не должно работать для крайних, там уже есть ребро...
                        addEdgeRandom(i, i+ M);
                    }

                }
                else { // грабли
                }
            }
            else {
                addEdgeRandom(i, i+1);
            }
            i = i + 1;
        }
    }

    void meshFull() {
        int i = 0;
        while (i<N*M-1) {
            if (((i+1)%(2*M)) != 0) { // если не точка перехода типа 9, 19, 29 при M=5 (смежности 2)
                if ((i / M) % 2 == 0) { // не "грабли" (ЗЕЛЕНЫЕ)
                    if ((i+1)%M != 0) { // не крайние ЗЕЛЕНЫЕ по линии возрастания
                        addEdgeRandom(i, i+1);
                    }
                    else {
                    }
                    if (i + M < N*M) { // вообще говоря, это не должно работать для крайних, там уже есть ребро...
                        addEdgeRandom(i, i+ M);
                    }

                }
                else { // грабли
                    if (i + M < N*M) { // не последний ряд из желтых
                     addEdgeRandom(i, i + 2*(M - i%M) - 1);
                    }
                    addEdgeRandom(i, i+1);
                }
            }
            else {
                addEdgeRandom(i, i+1);
            }
            i = i + 1;
        }

    }

    void addGridEdges(int n) {

        //... 25.09
        // в зависимости от цветогого класса, у нас есть разные варианты (рандомайзеров)
        int i;
        int i2;
        int _choice;
        bool stop_fl;

        cout << "---------------------------------" << endl;
        cout << "ADDING " << n << " GRID EDGES... " << "\n" << endl;

        for (int j = 0; j < n; j++) {
            do {
                stop_fl = false;
                i = randNumGen(V-1);

                // рандомный v1, далее в завис, от ситуации различные варианты v2;
                if (((i+1)%(2*M)) != 0) { // если не точка перехода типа 9, 19, 29 при M=5 (смежности 2) ()
                    if ((i / M) % 2 == 0) { // не "грабли" (ЗЕЛЕНЫЕ/КРАСНЫЕ)
                        if ((i+1)%M != 0) { // не крайние ЗЕЛЕНЫЕ по линии возрастания
                            if (i % M == 0) { // красные => нет вариантов выбора
                                cout << "RED: " << i << endl;
                                stop_fl = true; // нет вариантов ребра

                            }
                            else {
                                cout << "GREEN: " << i << endl;
                                // если не верхний ряд, то вариант один:
                                if (i/M > 1) {
                                    i2 = i - 2*(i%M) - 1;
                                }
                                else { // верхний ряд
                                    stop_fl = true;
                                }

                            }
                        }
                        else { // MAX ЗЕЛЕНЫЕ

                            cout << "MAX GREEN: " << i << endl;
                            // если не верхний ряд, то вариант один:
                                if (i/M > 1) {
                                    i2 = i - 2*(i%M) - 1;
                                }
                                else { // верхний ряд
                                    stop_fl = true;
                                }

                        };
                        if (i + M < N*M) {

                        }
                        else { // последний ряд из зеленых...
                        }

                    }
                    else { // грабли (ЖЕЛТЫЕ/СИНИЕ)
                        if (i%M == 0) { // углы граблей (СИНИЕ)
                            cout << "BLUE: " << i << endl;
                            if (i + M < N*M) { // не посл. ряд из голубых => 2 варианта
                                _choice = randNumGen(1);
                                if (_choice == 0) {
                                    cout << "- CHOICE: 0" << endl;
                                    i2 = i + 1;
                                }
                                else {
                                    i2 = i + 2*(M - i%M) - 1;
                                }
                            }
                            else { // последний ряд из голубых => 1 вариант
                                i2 = i + 1;
                            }

                        }
                        else { // ЖЕЛТЫЕ
                            cout << "YELLOW: " << i << endl;
                            if (i + M < N*M) { // не последний ряд из желтых => 3 варианта
                                _choice = randNumGen(2);
                                if (_choice == 0) {
                                    cout << "- CHOICE: 0" << endl;
                                    i2 = i - 1;
                                }
                                else if (_choice == 1){
                                    cout << "- CHOICE: 1" << endl;
                                    i2 = i + 1;
                                }
                                else {
                                    cout << "- CHOICE: 2" << endl;
                                    i2 = i + 2*(M - i%M) - 1;
                                }

                            }
                            else { // последний ряд из желтых => 2 варианта

                                _choice = randNumGen(1);
                                if (_choice == 0) {
                                    cout << "- CHOICE: 0" << endl;
                                    i2 = i - 1;
                                }
                                else {
                                    i2 = i + 1;
                                }
                            }
                        }
                    }
                }
                else { // точка перехода (ФИОЛЕТОВЫЕ) => всегда 1 вариант
                    cout << "PURPLE: " << i << endl;
                    i2 = i - 1;
                }
            } while (hasEdge(i, i2) || (i == i2) || (stop_fl));
            cout << "DONE! " << i << " " << i2 << endl;
            addEdgeRandom(i, i2);
        }
    }

    void addRandomEdges(int n) {
        int v1;
        int v2;
        for (int i = 0; i < n; i++) {
            do {
                //random v1, v2 of [0, V]
                cout << "choosing random two vertices: " << endl;
                v1 = randNumGen(V-1);
                v2 = randNumGen(V-1);
                cout << v1 << " " << v2 << endl;

            } while (hasEdge(v1, v2) || (v1 == v2)); // если ребро уже есть или вершины совпали, делаем еще раз
            cout << "DONE! " << v1 << " " << v2 << endl;
            addEdgeRandom(v1, v2);
        }
    }

};

class meshDiagGraph : public meshGraph {

    /*наследуется:
    spanningTree()
    meshFull()
    addGridEdge()
    addRandomEdge() - не используем!
    */

public:
    meshDiagGraph() {
        N = 3;
        M = 3;
        V = N*M;
        E = 10;
        mean = 5;
        stddev = 2;
        adj = new vector<pair<int, int> >[V];
        coordsArr = new point2D[V];

    }

    meshDiagGraph(int _N, int _M, int _E, int _mean, int _stddev) { // : Graph(_E) - мб так проще?
        N = _N;
        M = _M;
        V = N*M;
        E = _E;
        mean = _mean;
        stddev = _stddev;
        adj = new vector<pair<int, int> >[V];
    }

    void addGridDiagEdges(int n) {
        cout << "ADDING: " << n << " DIAG/GRID EDGES... " << "\n" << endl;

        int _choice;
         for (int j = 0; j < n; j++) {
             _choice = randNumGen(1);
             if (_choice == 0) {
                 cout << "GRID CHOSEN: " << endl;
                 addGridEdges(1);
             }
             else {
                 cout << "DIAG CHOSEN: " << endl;
                 addDiagEdges(1);
             }

         }
    }

    void addDiagEdges(int n) {
        int i;
        int i2;
        int _choice;
        bool stop_fl;
        cout << "ADDING " << n << " DIAG EDGES... " << "\n" << endl;

         for (int j = 0; j < n; j++) {

            do {
                stop_fl = false;
                i = randNumGen(V-1);

                // рандомный v1, далее в завис, от ситуации различные варианты v2;
                if (((i+1)%(2*M)) != 0) { // если не точка перехода типа 9, 19, 29 при M=5 (смежности 2) ()
                    if ((i / M) % 2 == 0) { // не "грабли" (ЗЕЛЕНЫЕ/КРАСНЫЕ)
                        if ((i+1)%M != 0) { // не крайние ЗЕЛЕНЫЕ по линии возрастания
                            if (i % M == 0) { // красные
                                cout << "RED: " << i << endl;
                                if (i/M < 1) { // верхний ряд => 1 вариант // !ВОЗМОЖНО НЕВЕРНО, ПРОВЕРИТЬ!
                                    i2 = i + M + 1;
                                }
                                else if (i + M + 1 < N * M) {// 2 варианта
                                    _choice = randNumGen(1);
                                    if (_choice == 0) {
                                        i2 = i - 2;
                                    }
                                    else {
                                        i2 = i + M + 1;
                                    }
                                }
                                else {// последний ряд => 1 вариант
                                    i2 = i - 2;
                                }

                            }
                            else {
                                cout << "GREEN: " << i << endl;
                                // если не верхний ряд, то вариант один:
                                if (i/M > 1) {
                                    i2 = i - 2*(i%M) - 1;
                                }
                                else { // верхний ряд
                                    stop_fl = true;
                                }

                            }
                        }
                        else { // MAX ЗЕЛЕНЫЕ

                            cout << "MAX GREEN: " << i << endl;
                            // если не верхний ряд, то вариант один:
                                if (i/M > 1) {
                                    i2 = i - 2*(i%M) - 1;
                                }
                                else { // верхний ряд
                                    stop_fl = true;
                                }

                        };
                        if (i + M < N*M) {

                        }
                        else { // последний ряд из зеленых/красных...
                        }

                    }
                    else { // грабли (ЖЕЛТЫЕ/СИНИЕ)
                        if (i%M == 0) { // углы граблей (СИНИЕ)
                            cout << "BLUE: " << i << endl;
                            if (i + M < N*M) { // не посл. ряд из голубых => 2 варианта

                                _choice = randNumGen(1);
                                if (_choice == 0) {
                                    cout << "- CHOICE: 0" << endl;
                                    i2 = i - M + 1;
                                }
                                else {
                                    i2 = i + 2*(M - i%M) - 2;
                                }
                            }
                            else { // последний ряд из голубых => 1 вариант
                                i2 = i - M + 1;
                            }

                        }
                        else { // ЖЕЛТЫЕ
                            cout << "YELLOW: " << i << endl;
                            if (i + M < N*M) { // 4 варианта
                                _choice = randNumGen(3);
                                switch (_choice) {
                                    case 0:
                                        i2 = i - M - 1;
                                        break;
                                    case 1:
                                        i2 = i - M + 1;
                                        break;
                                    case 2:
                                        i2 = i + 2*(M - i%M) - 2;
                                        break;

                                    default:
                                        i2 = i + 2*(M - i%M);
                                }
                            }
                            else { // последний ряд из желтых => 2 варианта
                                _choice = randNumGen(1);
                                switch (_choice) {
                                    case 0:
                                        i2 = i - M - 1;
                                        break;
                                    case 1:
                                        i2 = i - M + 1;
                                        break;
                                }
                            }
                        }
                    }
                }
                else { // точка перехода (ФИОЛЕТОВЫЕ)
                    cout << "PURPLE: " << i << endl;
                    if (i + M < N * M) { // 2 варианта
                        _choice = randNumGen(1);
                        if (_choice == 0) {
                            i2 = i - M - 1;
                        }
                        else {
                            i2 = i + 2;
                        }
                    }
                    else {// последний ряд => 1 вариант
                        i2 = i - M - 1;
                    }
                }
            } while (hasEdge(i, i2) || (i == i2) || (stop_fl));
            cout << "DONE! " << i << " " << i2 << endl;
            addEdgeWeight(i, i2, normNumGen(mean,stddev));
        }

    }

};

class radialGraph : public Graph {
    public:
        int k;
        int mean;
        int stddev;

        radialGraph() {
            V = 25;
            E = 20;
            k = 3;
            mean = 5;
            stddev = 2;
            coordsArr = new point2D[V];
            adj = new vector<pair<int, int> >[V];
        }

        radialGraph(int v, int e, int _k, int _mean, int _stddev) {
            V = v;
            E = e;
            k = _k;
            mean = _mean;
            stddev = _stddev;
            coordsArr = new point2D[V];
            adj = new vector<pair<int, int> >[V];
            
        }
    
    /*
    void spanningTreeX() {
        int i, n1;
        i = 0;
        n1 = (V-1)/((1+k)*k/2); // кол-во вершин на первом кольце

        while (i < V - 1) {
            if (i % n1 == 1) {
                addEdgeWeight(i, i + ((i/n1)+1)*n1 - 1, normNumGen(mean,stddev));
            }
        }
    }
    */
    
    
    int ringNum(int i, int n1, int k) {
        for (int j = 0; j < k+1; j++) {
            if ((1+j)*j*n1/2 > i) {
                if ((j)*(j-1)*n1/2 < i) {
                    return j;
                }
                else {
                    return j - 1;
                }
            }
        }
        return k;
    }
    
    void spanningTree() {
        

        
        int n1 = (V-1)/((1+k)*k/2); 
        cout << "Overall V: " << V << endl;
        cout << "Num of verts on first ring: " << n1 << endl;
        cout << "Main verts: " << 1 + n1*(k+1)*k/2 << endl;
    
        for (int i = 0; i < V-1; i++) { // i = 0, ..., V-1 because last edge (V-1,V)
            
            // just print every verts with corresponding ringNums
            // cout << i << " " << ringNum(i,n1,k) << endl;
            
            // circling edge if needed
            if ((ringNum(i+1,n1,k) != ringNum(i,n1,k)) && i != 0) { // если это граничная точка (КРОМЕ НУЛЯ)
                int k_pred = ringNum(i,n1,k) - 1;
                
                int i_2 = (k_pred*n1)*(k_pred+1)/2 + 1;
                addEdgeRandom(i,i_2);
                
            }
            
            // regular edge
            addEdgeRandom(i,i+1);
        }
      
        // final circling edge
        
        int i_2 = k*(k-1)*n1/2 + 1;
        addEdgeRandom(V-1,i_2);
    }
    
    void addRingEdges(int n) {

    }


 };

int main() {   
    // GridGraph...
    
    cout << "Hello, World!";
    int N = 10;
    int M = 20;
    int E = 250;
    int V = N*M;
    int mean = 1;
    int stddev = 2;
    meshGraph *g = new meshGraph(N, M, E, mean, stddev);
    int x = 2*N*M-(N+M);
    g->coordsInit();
    if (E >= x) {
        g->meshFull();
        g->addRandomEdges(E-x);
    } else {
        g->spanningTree();
        g->addGridEdges(E - (V-1));
    };
    cout << "N: " << N << " M: " << M << endl;
    g->printGraph(V);

    return 0;
}
