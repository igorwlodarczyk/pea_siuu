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
    vector<vector<int> > adjacencyMatrix;
};

void display_permutation(vector<int> permutation){
    for(int i = 0; i < permutation.size(); i++){
        cout << permutation[i] << "";
    }
    cout << endl;
}
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

vector<int> two_opt_swap(vector<int> temp_permutation){
    
    int first_city = -1;
    int second_city = -1;
    while(first_city == second_city){
        first_city = rand() % temp_permutation.size();
	    second_city = rand() % temp_permutation.size();
    }
    swap(temp_permutation[first_city], temp_permutation[second_city]);
    return temp_permutation;
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
                temp_permutation = two_opt_swap(temp_permutation);
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
    

vector<pair<string, int> > get_tested_graphs(string filename){
    vector<pair<string, int> > tested_graphs;
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

// Base parameters
int cooling_type;
double factor;
double step;
int max_epoch;
double starting_temperature;
void baseline(){
    cooling_type = 1;
    factor = 0.97;
    step = 0.01;
    max_epoch = 150;
}

unsigned long long silnia(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * silnia(n - 1);
    }
}

string generate_file_name(string parameter, double value){
    ostringstream double_stream;
    double_stream << value;
    string double_string = double_stream.str();
    for(char &c : double_string){
        if(c == '.'){
            c = '_';
        }
    }
    return parameter += double_string + ".txt";
}

void tests(){
    struct timeval start, stop;

    vector<pair<string, int> > result = get_tested_graphs("config.ini");
    int number_of_tests = 5;
    
    // Factor

    cout << "Testowanie wplywu factor na wyniki" << endl;
    baseline();
    for(double factor = 0.92; factor < 1; factor += 0.01){
        cout << "Wartosc factor " << factor << endl;
        string filename = generate_file_name("factor", factor);
        ofstream file("results/factor/" + filename);
        cout << filename << endl;
        for (const auto &pair : result) {
        cout << "Filename: " << pair.first << ", Number: " << pair.second<< endl;
        double avg = 0;
        double avg_time = 0;
        Graph graph = create_graph_from_file("graphs/" + pair.first);
        for(int i = 0; i < number_of_tests; i ++){
            starting_temperature = 30 * graph.size;
            gettimeofday(&start, NULL);
            int solution = simulated_annealing(graph, cooling_type, 1, factor, step, max_epoch, starting_temperature);
            gettimeofday(&stop, NULL);
            double time = (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec) / 1e6;
            avg_time += time;
            avg += solution;
            }
        avg = avg / number_of_tests;
        avg_time = avg_time / number_of_tests;
        cout << avg << endl;
        cout << (avg / pair.second - 1) * 100 << endl;
        cout << avg_time << " s" << endl;
        file << graph.size << ";" << avg_time << ";" << (avg / pair.second - 1) * 100 << endl;
        }
        file.close();
    }
    


    
    // Max epoch
    cout << "Testowanie wplywu max_epoch na wyniki" << endl;
    baseline();
    for(int max_epoch = 50; max_epoch < 160; max_epoch += 20){
        cout << "Wartosc max_epoch " << max_epoch << endl;
        string filename = generate_file_name("max_epoch", max_epoch);
        ofstream file("results/max_epoch/" + filename);
        cout << filename << endl;
        for (const auto &pair : result) {
        cout << "Filename: " << pair.first << ", Number: " << pair.second<< endl;
        double avg = 0;
        double avg_time = 0;
        Graph graph = create_graph_from_file("graphs/" + pair.first);
        for(int i = 0; i < number_of_tests; i ++){
            starting_temperature = 30 * graph.size;
            gettimeofday(&start, NULL);
            int solution = simulated_annealing(graph, cooling_type, 1, factor, step, max_epoch, starting_temperature);
            gettimeofday(&stop, NULL);
            double time = (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec) / 1e6;
            avg_time += time;
            avg += solution;
            }
        avg = avg / number_of_tests;
        avg_time = avg_time / number_of_tests;
        cout << avg << endl;
        cout << (avg / pair.second - 1) * 100 << endl;
        cout << avg_time << " s" << endl;
        file << graph.size << ";" << avg_time << ";" << (avg / pair.second - 1) * 100 << endl;
        }
        file.close();
    }
    
    
    // Testowanie wplywu step na wyniki
    cout << "Testowanie wplywu step na wyniki" << endl;
    baseline();
    cooling_type = 2;
    for(double step = 0.9; step > 0.3; step -= 0.1){
        cout << "Wartosc step " << step << endl;
        string filename = generate_file_name("step", step);
        ofstream file("results/step/" + filename);
        cout << filename << endl;
        for (const auto &pair : result) {
        
        cout << "Filename: " << pair.first << ", Number: " << pair.second<< endl;
        double avg = 0;
        double avg_time = 0;
        Graph graph = create_graph_from_file("graphs/" + pair.first);
        if(graph.size < 150){
        for(int i = 0; i < number_of_tests; i ++){
            starting_temperature = 30 * graph.size;
            gettimeofday(&start, NULL);
            int solution = simulated_annealing(graph, cooling_type, 1, factor, step, max_epoch, starting_temperature);
            gettimeofday(&stop, NULL);
            double time = (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec) / 1e6;
            avg_time += time;
            avg += solution;
            }
        avg = avg / number_of_tests;
        avg_time = avg_time / number_of_tests;
        cout << avg << endl;
        cout << (avg / pair.second - 1) * 100 << endl;
        cout << avg_time << " s" << endl;
        file << graph.size << ";" << avg_time << ";" << (avg / pair.second - 1) * 100 << endl;
        }else cout << "Pass" << endl;
        }
        file.close();
    }
    

    baseline();
    string filename = generate_file_name("starting_temperature", 30);
    ofstream file("results/starting_temperature/" + filename);
    cout << filename << endl;
    for (const auto &pair : result) {
    cout << "Filename: " << pair.first << ", Number: " << pair.second<< endl;
    double avg = 0;
    double avg_time = 0;
    Graph graph = create_graph_from_file("graphs/" + pair.first);
    starting_temperature = 30 * graph.size;
    cout << "Temperatura startowa " << starting_temperature << endl;
    for(int i = 0; i < number_of_tests; i ++){
        gettimeofday(&start, NULL);
        int solution = simulated_annealing(graph, cooling_type, 1, factor, step, max_epoch, starting_temperature);
        gettimeofday(&stop, NULL);
        double time = (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec) / 1e6;
        avg_time += time;
        avg += solution;
        }
    avg = avg / number_of_tests;
    avg_time = avg_time / number_of_tests;
    cout << avg << endl;
    cout << (avg / pair.second - 1) * 100 << endl;
    cout << avg_time << " s" << endl;
    file << graph.size << ";" << avg_time << ";" << (avg / pair.second - 1) * 100 << endl;
    }
    file.close();


    // Testowanie wplywu temperatury startowej na wyniki
    if(true){
    cout << "Testowanie wplywu temperatury startowej na wyniki" << endl;
    baseline();
    string filename = generate_file_name("starting_temperature", 900);
    ofstream file("results/starting_temperature/" + filename);
    cout << filename << endl;
    for (const auto &pair : result) {
    cout << "Filename: " << pair.first << ", Number: " << pair.second<< endl;
    double avg = 0;
    double avg_time = 0;
    Graph graph = create_graph_from_file("graphs/" + pair.first);
    starting_temperature = 30 * graph.size * graph.size;
    cout << "Temperatura startowa " << starting_temperature << endl;
    for(int i = 0; i < number_of_tests; i ++){
        gettimeofday(&start, NULL);
        int solution = simulated_annealing(graph, cooling_type, 1, factor, step, max_epoch, starting_temperature);
        gettimeofday(&stop, NULL);
        double time = (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec) / 1e6;
        avg_time += time;
        avg += solution;
        }
    avg = avg / number_of_tests;
    avg_time = avg_time / number_of_tests;
    cout << avg << endl;
    cout << (avg / pair.second - 1) * 100 << endl;
    cout << avg_time << " s" << endl;
    file << graph.size << ";" << avg_time << ";" << (avg / pair.second - 1) * 100 << endl;
    }
    file.close();
    }
    

    

}

int main(){
    srand((unsigned)time(NULL));
    tests();
    return 0;

}