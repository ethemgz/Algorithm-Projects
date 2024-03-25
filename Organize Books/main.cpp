#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

string FILE_NAME = "books.csv";
int NUMBER_OF_PARTITION_CALLS = 0;
int NUMBER_OF_SWAPS = 0;

vector<pair<int, float>> read_from_csv(){
    
    vector<pair<int, float>> data;
    ifstream csv_file(FILE_NAME);
    
    string line;
    

    getline(csv_file, line);
    
    while(getline(csv_file, line)){

        int bookID_index, point_start_index, point_end_index = 0;
        int ID;
        float point;

        bookID_index = line.find(",");
        ID = stoi(line.substr(0,bookID_index));
        
        for(int i = 0; i < 4; i++){
            point_start_index = point_end_index + 1;
            point_end_index = line.find(",", point_end_index + 1);
        }
        int point_length = point_end_index - point_start_index;
        point = stof(line.substr(point_start_index, point_length));
        
        data.push_back(make_pair(ID, point));
    }

    csv_file.close();
    return data;
}

void write_to_csv(vector<pair<int, float>> books){
    
    ofstream output_csv(FILE_NAME);

    int i;
    for(i = 0; i < books.size(); i++){
        output_csv << books[i].first << "," << books[i].second << endl;
    }
    output_csv.close();
}

int partition(vector<pair<int, float>> &books, int lowest, int highest)
{
    NUMBER_OF_PARTITION_CALLS++;
    int i = lowest, j = lowest;
    
    float pivot = books[highest].second;
    
    while(j != highest){
        if(books[j].second < pivot){
            NUMBER_OF_SWAPS++;
            int temp_ID = books[j].first;
            float temp_point = books[j].second;

            books[j].first = books[i].first;
            books[j].second = books[i].second;

            books[i].first = temp_ID;
            books[i].second = temp_point;

            i = i + 1;
        }
        j = j + 1;
    }

    if(books[i].second < pivot){
        i = i + 1;
    }

    NUMBER_OF_SWAPS++;
    int temp_ID = books[j].first;
    float temp_point = books[j].second;

    books[j].first = books[i].first;
    books[j].second = books[i].second;

    books[i].first = temp_ID;
    books[i].second = temp_point;

    return i;
}
 
void quick_sort(vector<pair<int, float>> &books, int lowest, int highest)
{
    if (lowest >= highest){
        return;
    }
    
    int pivot_index = partition(books, lowest, highest);

    quick_sort(books, lowest, pivot_index - 1);
    quick_sort(books, pivot_index + 1, highest);
}

int main() {

    vector<pair<int, float>> books = read_from_csv();
    
    int books_size = books.size() - 1;

    for(int i = 0; i < books_size; i++){
        cout << books[i].first << " " << books[i].second << endl;
    }

    auto start_time = chrono::high_resolution_clock::now();
    quick_sort(books, 0, books_size);
    auto finish_time = chrono::high_resolution_clock::now();
    auto time_in_nanoseconds = chrono::duration_cast<chrono::nanoseconds>(finish_time-start_time).count();
    float miliseconds = time_in_nanoseconds / 1000000.0;

    for(int i = 0; i < books_size; i++){
        cout << books[i].first << " " << books[i].second << endl;
    }

    cout << "FULL" << endl;
    cout << "Algorithm Working Time In Miliseconds: " << miliseconds << endl;
    cout << "Swap Count: " << NUMBER_OF_SWAPS << endl;
    cout << "Swap Partition Calls: " << NUMBER_OF_PARTITION_CALLS << endl;
    
    write_to_csv(books);

    return 0;
}