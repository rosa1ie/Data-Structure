/*
test_graph.cpp

Programmers: Nam Ngo, Rosalie Lee
Date: 11 November 2022
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include "graph.cpp"

using namespace std;

Graph<string, string>* generate_graph(string fname){
    string line;
    ifstream infile(fname);
    vector<string> keys;
    vector<string> data;
    vector<vector<string> > adjs;
    if(infile.is_open()){
        while(getline(infile, line)){
            unsigned long delim = line.find(":");
            string key = line.substr(0, delim);
            string adj = line.substr(delim+1);
            
            keys.push_back(key);
            data.push_back(key + " data");
            delim = adj.find(",");
            vector<string> adj_lst;
            while(delim < adj.length()){
                adj_lst.push_back(adj.substr(0, delim));
                adj = adj.substr(delim+1);
                delim = adj.find(",");
            }
            adj_lst.push_back(adj);
            adjs.push_back(adj_lst);
        }
    }
    Graph<string,string>* G = new Graph<string, string>(keys, data, adjs);
    return G;
}

void test_get(Graph<string,string>* G) {
    try {
        if(G->get("S")==nullptr || G->get("S")->data != "S data") {
            cout << "Incorrect result getting vertex \"s\"" << endl; 
        }
        if(G->get("a") != nullptr) {
            cout << "Incorrect result getting non-existant vertex \"a\"" << endl;
        }
    } catch(exception& e) {
        cerr << "Error getting vertex from graph : " << e.what() << endl;
    }
}

void test_reachable(Graph<string,string>* G) {
    try {
        if(!G->reachable("R", "V")) {
            cout << "Incorrectly identified adjacent vertex \"V\" as unreachable from \"R\"" << endl;
        }
        if(!G->reachable("X", "W")) {
            cout << "Incorrectly identified \"W\" as unreachable from \"X\"" << endl;
        }
        if(G->reachable("S", "A")) {
            cout << "Incorrectly identified non-existant vetex \"A\" as reachable from \"S\"" << endl;
        }
    } catch(exception& e) {
        cerr << "Error testing reachable : " << e.what() << endl;
    }
}

void test_bfs(Graph<string,string>* G) {
    try {
        G->bfs("U");
        string vertices[8] = {"V", "R", "S", "W", "T", "X", "U", "Y"};
        int distances[8] = {0,0,0,2,0,3,0,1};
        for(int i = 0; i < 8; i++){
            if(G->get(vertices[i])==nullptr || G->get(vertices[i])->distance!=distances[i]) {
                cout << "Incorrect bfs result. Vertex " << vertices[i] << " should have distance " << distances[i] << " from source vertex \"t\"" << endl;
            }
        }
        G->bfs("Y");
        string vertices1[8] = {"V", "R", "S", "W", "T", "X", "U", "Y"};
        int distances1[8] = {0,0,0,1,0,2,3,0};
        for(int i = 0; i < 8; i++){
            if(G->get(vertices1[i])==nullptr || G->get(vertices1[i])->distance!=distances1[i]) {
                cout << "Incorrect bfs result. Vertex " << vertices[i] << " should have distance " << distances[i] << " from source vertex \"t\"" << endl;
            }
        }
        G->bfs("S");
        string vertices2[8] = {"V", "R", "S", "W", "T", "X", "U", "Y"};
        int distances2[8] = {2,1,0,0,0,0,0,0};
        for(int i = 0; i < 8; i++){
            if(G->get(vertices2[i])==nullptr || G->get(vertices2[i])->distance!=distances2[i]) {
                cout << "Incorrect bfs result. Vertex " << vertices[i] << " should have distance " << distances[i] << " from source vertex \"t\"" << endl;
            }
        }
    } catch(exception& e) {
        cerr << "Error testing bfs : " << e.what() << endl;
    } 
}

void test_print_path(Graph<string,string>* G) {
    try {
        stringstream buffer;
        streambuf* prevbuf = cout.rdbuf(buffer.rdbuf());
        G->print_path("T", "V");

        cout.rdbuf(prevbuf);
        if(buffer.str()!="T -> S -> R -> V") {
            cout << "Incorrect path from vertex \"T\" to vertex \"V\". Expected: T -> S -> R -> V but got : " << buffer.str() << endl;
        }
    } catch(exception& e) {
        cerr << "Error testing print path : " << e.what() << endl;
    }
}

void test_edge_class(Graph<string,string>* G) {
    try {
        string e_class =  G->edge_class("R", "V"); // tree edge
        if(e_class != "tree edge") {
            cout << "Misidentified tree edge (\"R\", \"V\") as : " << e_class << endl;
        }
        e_class = G->edge_class("X", "U"); // back edge
        if(e_class != "back edge") {
            cout << "Misidentified back edge (\"X\", \"U\") as : " << e_class << endl;
        }
        e_class =  G->edge_class("R", "U"); // no edge
        if(e_class != "no edge") {
            cout << "Misidentified non-existant edge (\"R\", \"U\") as : " << e_class << endl;
        }
        e_class = G->edge_class("T", "W"); // forward edge
        if(e_class != "forward edge") {
            cout << "Misidentified forward edge (\"T\", \"W\") as : " << e_class << endl;
        }
        e_class = G->edge_class("T", "S"); // cross edge
        if(e_class != "cross edge") {
            cout << "Misidentified forward edge (\"T\", \"S\") as : " << e_class << endl;
        }
    } catch(exception& e) {
        cerr << "Error testing edge class : " << e.what() << endl;
    }
    
}

void test_bfs_tree(Graph<string,string>* G) {
    try {
        stringstream buffer1;
        streambuf* prevbuf1 = cout.rdbuf(buffer1.rdbuf());
        G->bfs_tree("U");
        cout.rdbuf(prevbuf1);
        if(buffer1.str() != "U\nY\nW\nX") {
            cout << "Incorrect bfs tree. Expected : U\nY\nW\nX \nbut got :\n" << buffer1.str() << endl;
        }
        stringstream buffer2;
        streambuf* prevbuf2 = cout.rdbuf(buffer2.rdbuf());
        G->bfs_tree("Y");
        cout.rdbuf(prevbuf2);
        if(buffer2.str() != "Y\nW\nX\nU") {
            cout << "Incorrect bfs tree. Expected : Y\nW\nX\nU \nbut got :\n" << buffer2.str() << endl;
        }
        stringstream buffer3;
        streambuf* prevbuf3 = cout.rdbuf(buffer3.rdbuf());
        G->bfs_tree("T");
        cout.rdbuf(prevbuf3);
        if(buffer3.str() != "T\nS U W\nR Y X\nV") {
            cout << "Incorrect bfs tree. Expected : Y\nW\nX\nU \nbut got :\n" << buffer3.str() << endl;
        }
    } catch(exception& e) {
        cerr << "Error testing bfs tree : " << e.what() << endl;
    }
    
}

int main() {

    Graph<string,string>* G = generate_graph("graph_description.txt");
    // test_get(G);
    // test_reachable(G);
    test_bfs(G);
    // test_print_path(G);
    // test_edge_class(G);
    test_bfs_tree(G);

    cout << "Testing completed" << endl;

    delete G;
    
    return 0;
}