#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <climits>
#include <sys/time.h>
#include <math.h>
#include <random>
#include <cmath>




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

void two_opt_swap(vector<int>& temp_permutation){
    srand((unsigned)time(NULL));
    int first_city = -1;
    int second_city = -1;
    while(first_city != second_city){
        first_city = rand() % temp_permutation.size();
	    second_city = rand() % temp_permutation.size();
    }
    swap(temp_permutation[first_city], temp_permutation[second_city]);

}

int calculate_path(vector<int> permutation, Graph graph){
    int shortest_path_length = 0;
    for(int i = 0; i < graph.size; i++){
        if(i ==  graph.size - 1){
            shortest_path_length += graph.adjacencyMatrix[permutation[i]][permutation[0]];
        }else {
            shortest_path_length += graph.adjacencyMatrix[permutation[i]][permutation[i + 1]];
        }
    }
    return shortest_path_length;
}

double geometrical_cooling(double temperature, double factor){
    return temperature * factor;
}

double linear_cooling(double temperature, double step){
    return temperature - step;
}

int simulated_annealing(Graph graph, int cooling_type, int type, double factor, double step, int max_epoch, double starting_temperature){
    int size = graph.size;
    vector<int> permutation;
    vector<int> temp_permutation;
    for (int i = 0; i < graph.size; i++) {
        permutation.push_back(i);
    }

    random_device rd;
    mt19937 g(rd());
    std::shuffle(permutation.begin(), permutation.end(), g);
    
    int shortest_path_length = calculate_path(permutation, graph);
    // double temperature = 300 * size;
    double temperature = starting_temperature;
    double absolute_temperature = 0.01;
    int temp_path_length;
    int delta;
    uniform_real_distribution<double> dis(0.0, 1.0);
    while(temperature > absolute_temperature){
        int epoch = 0;
        while(epoch < max_epoch){
            if(type == 1){
                temp_permutation = permutation;
                two_opt_swap(temp_permutation);
            }

            temp_path_length = calculate_path(temp_permutation, graph);
            delta = temp_path_length - shortest_path_length;
            if(delta < 0){
                shortest_path_length = temp_path_length;
                permutation = temp_permutation;
            }else{
                double p = exp(- (delta / temperature));
                double r =dis(g);
                if(r < p){
                    shortest_path_length = temp_path_length;
                    permutation = temp_permutation;
                }
            }
            epoch += 1;
        }
        if(cooling_type == 1){
            temperature = geometrical_cooling(temperature, factor);
        }else if(cooling_type == 2){
            temperature = linear_cooling(temperature, step);
        }

    }
    return shortest_path_length;
}

vector<pair<string, int>> get_tested_graphs(string filename){
    vector<pair<string, int>> tested_graphs;
    ifstream file(filename);
    string line;
    bool inside_file_content = false;
     while (getline(file, line)){
        if (line == "[tested_graphs]") {
                inside_file_content = true;
                continue;
            } else if (line == "[end]") {
                inside_file_content = false;
                break;
            }

            if (inside_file_content) {
            stringstream ss(line);
            string filename_part, number_part;
            getline(ss, filename_part, ',');
            getline(ss, number_part);

            int number = stoi(number_part);

            tested_graphs.push_back(make_pair(filename_part, number));
        }
     }

     file.close();
     return tested_graphs;
}

void tests(){
    vector<pair<string, int>> result = get_tested_graphs("config.ini");

    
    for (const auto &pair : result) {
        cout << "Filename: " << pair.first << ", Number: " << pair.second<< endl;
    }


}

int main(){
    Graph graph = create_graph_from_file("tsp.txt");

    // int solution = simulated_annealing(graph, 2, 1, 1, 1, 1);
    tests();
    return 0;


}