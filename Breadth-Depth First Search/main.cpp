#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>


using namespace std;

int NODE_INDEX = 0;

class Node{
        public:
        int index;
        int x,y,p;

        Node(int x, int y, int p){
            this->x=x;
            this->y=y;
            this->p=p;
            index = NODE_INDEX;
            NODE_INDEX++;
        }
};

class Graph{
    
    public:

    vector<vector<Node>> adjacencyL;
    int source_node;
    int destination_node;
    int node_count;
    map<int,int> source_node_neighbors;
    map<int, bool> is_visited;
    bool path_is_found = false;
    
    void fill_graph(string file_name);
    void calculate_neighbors();
    
    void create_bfs_file();
    bool BFS(int array1[], int array2[]);

    void create_dfs_file();
    vector<int> DFS(int current_node, vector<int> path, vector<int> &final_path);

    void create_graph_file();
};



void Graph::fill_graph(string file_name){

    ifstream f(file_name);
    string line;

    string firstline;

    getline(f, firstline);
    stringstream ss(firstline);
    ss >> node_count >> source_node >> destination_node;
    
    int size;
    while(getline(f,line)){
        stringstream ss(line);
        int x, y, p;
        ss >> x >> y >> p;
        
        Node newNode(x,y,p);
  
        float array2ance;
        adjacencyL.push_back(vector<Node>{newNode});
        
        size = adjacencyL.size();
        for(int j = 0; j < size - 1; j++){

            Node tempnode = adjacencyL[j][0];
            array2ance=(newNode.x-tempnode.x)*(newNode.x-tempnode.x)+(newNode.y-tempnode.y)*(newNode.y-tempnode.y);
            
            if(array2ance <= newNode.p && array2ance <= tempnode.p){
                adjacencyL[j].push_back(newNode);
                adjacencyL[adjacencyL.size() - 1].push_back(tempnode);
            }
        }
    }
    size = adjacencyL.size();
    for(int i = 0; i < size; i++){
        is_visited[i] = false;
        }
}

bool Graph::BFS(int array1[], int array2[])
{
    list<int> queue;
    bool visited[node_count];
    int i = 0;
    while(i < node_count){
        visited[i] = false;
        array2[i] = node_count;
        array1[i] = -1;
        i++;
    }
    visited[source_node] = true;
    array2[source_node] = 0;
    queue.push_back(source_node);
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop_front();
        int size_u = adjacencyL[u].size();
        for (int i = 0; i < size_u; i++) {
            if (visited[adjacencyL[u][i].index] == false) {
                visited[adjacencyL[u][i].index] = true;
                array2[adjacencyL[u][i].index] = array2[u] + 1;
                array1[adjacencyL[u][i].index] = u;
                queue.push_back(adjacencyL[u][i].index);
                
                if (adjacencyL[u][i].index == destination_node)
                    return true;
            }
        }
    }
 
    return false;
}
 
 
void Graph::create_bfs_file(){
    
    ofstream file("bfs.txt");
    file << "BFS:" << endl;

    int array1[node_count], array2[node_count];
    if (BFS(array1, array2) == false) {
        file << "-1";
        return;
    }
 
 
    vector<int> path;
    int node = destination_node;
    path.push_back(node);
    while (array1[node] != -1) {
        path.push_back(array1[node]);
        node = array1[node];
    }
 
 
    file << array2[destination_node] << " ";
    file << path[path.size() - 1];
    for (int i = path.size() - 2; i >= 0; i--)
        file << "->" << path[i];
    
    file.close();
}

vector<int> Graph::DFS(int current_node, vector<int> path, vector<int> &final_path)
{

    if(path_is_found){
        return final_path;
    }
    int next_node_num;
    is_visited[current_node] = true;
    int current_size = adjacencyL[current_node].size();
    for(int i = 1; i < current_size; i++){
        next_node_num = adjacencyL[current_node][i].index;
        int is_neighbor = source_node_neighbors.count(current_node);
        if(is_neighbor == 1 && !path_is_found && path.size() > 1){
            path_is_found = true;
            final_path = path;
        }
        if(!is_visited[next_node_num]){
            path.push_back(next_node_num);
            DFS(next_node_num, path, final_path);
            path.pop_back();
        }
    }
    return final_path;
}

void Graph::calculate_neighbors(){
    map<int,int> neighbors;
    int source_size = adjacencyL[source_node].size();
    for(int i = 0; i < source_size; i++){
        neighbors[adjacencyL[source_node][i].index] = 0;
    }
    source_node_neighbors = neighbors;
}

void Graph::create_graph_file(){

    ofstream file("graph.txt");

    file << "Graph:" << endl;
    int size = adjacencyL.size();
    int matrix[size][size];

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            matrix[i][j] = 0;
        }
    }

    for(int i = 0; i < node_count; i++){
        int size_i = adjacencyL[i].size();
        for(int j = 0; j < size_i; j++){
            matrix[i][adjacencyL[i][j].index] = 1;
            
        }
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i == j){
                matrix[i][j] = 0;
            }
            file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}

void Graph::create_dfs_file(){

    vector<int> path;
    path = DFS(source_node, path, path);

    

    ofstream file("dfs.txt");

    file << "DFS:" << endl;
    if(path.size() == 0){
        file << "-1";
    }else{
        path.insert(path.begin(), source_node);
        path.push_back(source_node);
        int path_size = path.size();

        file << path_size - 1 << " ";
        file << path[0];
        
        for(int i = 1; i < path_size; i++){
            file << "->" << path[i];
        }
    }
    file.close();
}   

int main(int argc, char const *argv[]){

    Graph graph;
    string file_path;
    
    file_path = argv[1];
    
    graph.fill_graph(file_path);
    
    graph.calculate_neighbors();

    graph.create_dfs_file();

    graph.create_bfs_file();

    graph.create_graph_file();

    exit(0);
}