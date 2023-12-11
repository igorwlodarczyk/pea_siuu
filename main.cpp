#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <climits>
#include <sys/time.h>
#include <math.h>


using namespace std;

struct Graph {
    int number;
    int size;
    vector<vector<int>> adjacencyMatrix;
};

Graph create_graph_from_file(string filename){
    ifstream input_file(filename);
    Graph graph;
    input_file >> graph.size;
    
    graph.adjacencyMatrix.resize(graph.size, vector<int>(graph.size));
    for (int i = 0; i < graph.size; i++) {
        for (int j = 0; j < graph.size; j++) {
            input_file >> graph.adjacencyMatrix[i][j];
        }
    }
    
    input_file.close();
    return graph;

}

void display_graph(Graph graph){
    cout << "Adjacency matrix" << std::endl;
    for (int i = 0; i < graph.size; i++) {
        for (int j = 0; j < graph.size; j++) {
            cout << graph.adjacencyMatrix[i][j] << " ";
        }
        cout << std::endl;
    }
}

int main(){
    
    return 0;
}