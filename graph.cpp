/*
graph.cpp

Programmers: Nam Ngo, Rosalie Lee
Date: 11 November 2022
*/
#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<string>
#include <fstream>
#include <sstream>

using namespace std;

template <class Data, class Key>
struct Vertex
{
    Key key;
    Data data;  //satellite data
    Vertex<Data, Key>* p;   //parent
    int distance; //keep track of distance for bfs use

    int d;  //time discover 
    int f;  //time finish

    bool color;  // 0 for white, 1 for black     
    vector<Vertex<Data,Key>*> adj; 

    Vertex();
    Vertex(Data data, Key key);
};

/*
Default constructor if no key or data passed
*/
template <class Data, class Key>
Vertex<Data, Key>::Vertex()
{
    this->key = Key();
    this->data = Data();
}

/*
Constructor when both key and data are passed
*/
template <class Data, class Key>
Vertex<Data, Key>::Vertex(Data data, Key key)
{
    this->key = key;
    this->data = data;
}

template <class Data, class Key>
class Graph
{
    private:
    vector<Vertex<Data,Key>*> vertexList; 
    void dfs();
    void dfs_visit(Vertex<Data,Key>*, int&);

    public:
    Graph(vector<Key> keys, vector<Data> data, vector<vector<Key> > edges);

    Vertex<Data,Key>* get(Key k);
    bool reachable(Key u,Key v);
    void bfs(Key s);
    void print_path(Key u, Key v);
    string edge_class(Key u, Key v);
    void bfs_tree(Key s);

};

/*
Constructor for graph
*/
template <class Data, class Key>
Graph<Data, Key>::Graph(vector<Key> keys, vector<Data> data, vector<vector<Key> > edges)
{
    vertexList.reserve(keys.size());    //reserve memory allocated for the list
    for (int i = 0; i < keys.size(); i++)
    {
        Vertex<Data,Key>* newVertex = new Vertex<Data,Key> (data[i], keys[i]);  //create new vertex with keys and data
        vertexList.push_back(newVertex);    
    }

    for (int j=0; j < edges.size(); j++)    //add edges to each vertex in the list
    {
        (vertexList[j]->adj).reserve(edges[j].size());
        for (int k=0; k < edges[j].size(); k++)
        {
            for (Vertex<Data,Key>* newVertex : vertexList)
            {
                if (edges[j][k] == newVertex->key)
                {
                    (vertexList[j]->adj).push_back(newVertex);
                }
                else
                    continue;
            }
        }
    }
}

/*
get function.

The function returns a pointer to the vertex corresponding to the key k in the graph.

Parameters:
- k: a Key.

Preconditions:
A Graph with n vertices.

Postconditions:
A Graph with n vertices.
*/
template <class Data, class Key>
Vertex<Data,Key>* Graph<Data, Key>::get(Key k)
{
    for(Vertex<Data,Key>* temp:vertexList) //iterate through the list of vertex pointer in the graph
    {
        if (temp -> key == k)
        {
            return temp;
        }
    }
    return nullptr;

}

/*
reachable function.

The function indicate if the vertex corresponding to the key v is reachable from the vertex corresponding to the key u in the graph 

Parameters:
- u: a Key.
- v: a Key.

Preconditions:
A Graph with n vertices.

Postconditions:
A Graph with n vertices with distance and parent rooted at the key u.
*/
template <class Data, class Key>
bool Graph<Data,Key>::reachable(Key u,Key v)
{
    if (this->get(u) == nullptr || this->get(v) == nullptr) //check if there exists vertex with key u or v
    {
        return false;
    }

    bfs(u);

    if (this->get(v)->color == true)
        return true;
    return false;
}

/*
bfs function.

The function execute the breadth-first search algorithm for the graph G from the source vertex corresponding to the key value s.

Parameters:
- s: a Key.

Preconditions:
A Graph with n vertices.

Postconditions:
A Graph with n vertices with distance and parent rooted at the key s.
*/
template <class Data, class Key>
void Graph<Data,Key>::bfs(Key s)
{
    if (this->get(s) == nullptr)        // There must exist a vertex of the key s
    {
        return;
    }

    queue<Vertex<Data,Key>*> q;
    for (int i=0; i < vertexList.size(); i++)   // Initialize every vertices in the list.
    {
        vertexList[i]->color = false;
        vertexList[i]->distance = 0;
        vertexList[i]->p = nullptr;
    }

    Vertex<Data,Key>* u = this->get(s);
    q.push(u);
    u->color = true;                            // The root becomes true after pushing into the queue.
    u->distance = 0;
    u->p = nullptr;

    while (!q.empty())                          // The queue will be empty after visiting all the vertices.
    {
        u = q.front();
        q.pop();
        for (Vertex<Data,Key>* v : u->adj)
        {
            if (v->color == false)              // Setting the distance and parent if the vertex hasn't been visited.
            {
                v->color = true;
                v->distance = u->distance + 1;
                v->p = u;
                q.push(v);
            }
        }
        u->color = true;                        // Color becomes black again after visiting all the adjacents.
    }
}

/*
print_path function.

The function print a shortest path from the vertex in G corresponding to the key u to the vertex in G corresponding to the key v.

Parameters:
- u: a Key.
- v: a Key.

Preconditions:
A Graph with n vertices.

Postconditions:
A Graph with n vertices with distance and parent rooted at the key u.
*/
template <class Data, class Key>
void Graph<Data,Key>::print_path(Key u, Key v)
{
    if (this->get(u) == nullptr || this->get(v) == nullptr) // There must exist the vertices of key u and v.
    {
        return;
    }

    bfs(u);
    Vertex<Data,Key>* vertex_u = this->get(u);
    Vertex<Data,Key>* vertex_v = this->get(v);
    stringstream ss;

    string ans = "";
    while(v != u)
    {
        if (vertex_v->p == nullptr)    // if there is no path from key u to key v
        {
            return;
        }
        ss<<v;
        ans = " -> " + ss.str() + ans;

        v = vertex_v->p->key;  
        vertex_v = vertex_v->p;
        ss.str("");
    }
    ss<<u;
    ans = ss.str() + ans ;
    cout << ans;
   
}

/*
dfs function.

The function execute the depth-first search algorithm for the graph G

Preconditions:


Postconditions:

*/
template <class Data, class Key>
void Graph<Data,Key>::dfs()
{
    for (Vertex<Data,Key>*u : vertexList)
    {
        u->color = false;
        u->p = nullptr;
    }
    int time = 0;
    for (Vertex<Data,Key>*u : vertexList)
    {
        if (u->color == false)
        {
            dfs_visit(u,time);
        }
    }
}

/*
dfs_visit function.

the function keep track of d and f when visiting each new vertex

Parameters:
- u: a Vertex.
- time: an integer.

Preconditions:


Postconditions:

*/
template <class Data, class Key>
void Graph<Data,Key>::dfs_visit(Vertex<Data,Key>*u, int& time)
{
    time = time + 1;    //vertex u has just been discovered
    u->d = time;
    u->color = true;
    for (Vertex<Data,Key>*v : u->adj)   // explore each edge (u,v)
    {
        if (v->color == false)
        {
            v->p = u;
            dfs_visit(v,time);
        }
    }
    time = time + 1;
    u->f = time;
}

/*
edge_class function.

The function return the string representation of the edge classification(tree edge, back edge, forward edge, cross edge, or no edge) of the edge from vertex u to v.

Parameters:
- u: a Key.
- v: a Key.

Preconditions:


Postconditions:

*/
template <class Data, class Key>
string Graph<Data,Key>::edge_class(Key u, Key v)
{
    if (this->get(u) == nullptr || this->get(v) == nullptr) // There must exist the vertices of key u and v.
    {
        return "no edge";
    }

    dfs();
    if (get(v)->p == get(u))
    {
        return "tree edge";
    }
    else if (get(v)->d < get(u)->d && get(u)->f < get(v)->f)
    {
        return "back edge";
    }
    else if (get(u)->d < get(v)->d && get(v)->f < get(u)->f)
    {
        return "forward edge";
    }
    else if (get(v)->f < get(u)->d)
    {
        return "cross edge";
    }
    else{
        return "no edge";
    }
}

/*
bfs_tree function.

The function print a shortest path from the vertex in G corresponding to the key u to the vertex in G corresponding to the key v.

Parameters:
- u: a Key.
- v: a Key.

Preconditions:
A Graph with n vertices.

Postconditions:
A Graph with n vertices with distance and parent rooted at the key s.
*/
template <class Data, class Key>
void Graph<Data,Key>::bfs_tree(Key s)
{
    if (this->get(s) == nullptr)        // There must exist a vertex of the key s.
    {
        return;
    }

    bfs(s);                             // Setting the root from the key s.

    Vertex<Data,Key>* u = this->get(s);
    queue< Vertex<Data, Key>* > q;
    vector< Vertex<Data, Key>* > orderedList;       // The list will be stored with vertices in the distance increasing order.
    int max_distance = 0;

    for (int i = 0; i < vertexList.size(); i++)     // Changing every vertices color to false and checking the maximum distance.
    {
        vertexList[i]->color = false;

        if (max_distance < vertexList[i]->distance)
            max_distance = vertexList[i]->distance;
    }

    cout << s;                                      // Print out the root first.

    q.push(u);                                      

    while (!q.empty())                              // The queue will be empty after visiting all the vertices.
    {
        u = q.front();

        for (int j = 0; j < u->adj.size(); j++)
        {
            if (u->adj[j]->color == false)          // Storing all the adjacents that hasn't been visited in the queue and vector.
            {
                q.push(u->adj[j]);
                orderedList.push_back(u->adj[j]);
                u->adj[j]->color = true;
            }
        }

        q.pop();
    }

    int j = 0;

    for (int i = 1; i <= max_distance; i++)
    {
        cout << endl;                               // Avoiding having unnecessary endline at the end.

        if (orderedList[j]->distance == i){         // Avoiding having unnecessary space at the end of each line.
            cout << orderedList[j]->key;
            j++;
        }

        while (j < orderedList.size())
        {
            if (orderedList[j]->distance == i){     // The vertices are sorted in distance increasing order in the list.
                cout << " " << orderedList[j]->key;
                j++;
            }
            else{
                break;
            }
        }
    }


}

// DESTRUCTOR FOR GRAPH
//test case
//print path
//bfs tree
