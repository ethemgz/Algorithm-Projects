#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "math.h"
#include <chrono>

using namespace std;

string OUTPUT_FILE_NAME = "output.txt";

double MaxValue = -1;
double MinValue = -1;
string FirstDate;
string LastDate;
int SORTED_UNTIL = 1;

struct Value{
    vector<double> values;
};

double calculate_mean(Value &sample){

    int size = sample.values.size();
    double sum = 0;
    double mean;

    for(int i = 0; i < size; i++){
        sum = sum + sample.values[i];
    }
    mean = sum / size;
    return mean;
}

double calculate_std(Value &sample){

    double mean = calculate_mean(sample);
    int size = sample.values.size();
    double squares_sum = 0;

    for(int i = 0; i < size; i++){
        squares_sum = squares_sum + (sample.values[i] - mean) * (sample.values[i] - mean) / (size - 1);
    }
    return sqrt(squares_sum);
}


void insertionSort(Value &sample)
{
    int size = sample.values.size();
    int i, j;
    double key;

    bool a_bool = false;

    //we can start the sorting index from a point the last time we sorted the array
    //in other words array is sorted up until a point so we will pass thoso parts
    for (i = SORTED_UNTIL; i < size; i++)
    {
        key = sample.values[i];
        j = i - 1;
        
        while (j >= 0 && sample.values[j] > key)
        {
            sample.values[j + 1] = sample.values[j];
            j = j - 1;
        }
        sample.values[j + 1] = key;
    }
    SORTED_UNTIL = size - 1;
}

double calculate_median(Value &sample){

    int size = sample.values.size();
    double median;

    insertionSort(sample);

    if(size % 2 == 1){
        median = sample.values[size / 2];
    }else{
        median = (sample.values[size / 2] + sample.values[size / 2 - 1]) / 2;
    }
    return median;
}

string remove_trailing_zeros(string str_in){
    int dot_index = str_in.find(".");
    if(dot_index != -1){
        int size = str_in.size();
        int i = size - 1;

        while(i > dot_index + 1 & str_in[i] == '0'){
            str_in.erase(i);
            i--;
        }
    }
    return str_in;
}

double calculate_thirdq(Value &sample){

    int size = sample.values.size();
    double thirdq;

    insertionSort(sample);

    if(size % 4 == 0){
        thirdq = sample.values[3 * (size / 4) - 1] * 0.75 + sample.values[3 * (size / 4)] * 0.25;
    }else if(size % 4 == 1){
        thirdq = sample.values[(3 * size + 1) / 4 - 1];
    }else if(size % 4 == 2){
        thirdq = sample.values[(3 * size + 2) / 4 - 1] * 0.75 + sample.values[(3 * size + 2) / 4 - 2] * 0.25;
    }else if(size % 4 == 3){
        thirdq = sample.values[3 * (size / 4) + 1] * 0.50 + sample.values[3 * (size / 4) + 2] * 0.50;
    }

    return thirdq;
}

double calculate_firstq(Value &sample){

    int size = sample.values.size();
    double firstq;

    insertionSort(sample);

    if(size % 4 == 0){
        firstq = sample.values[size / 4 - 1] * 0.25 + sample.values[size / 4] * 0.75;
    }else if(size % 4 == 1){
        firstq = sample.values[(size + 3) / 4 - 1];
    }else if(size % 4 == 2){
        firstq = sample.values[size / 4 + 1] * 0.25 + sample.values[size / 4] * 0.75;
    }else if(size % 4 == 3){
        firstq = sample.values[size / 4] * 0.50 + sample.values[size / 4 + 1] * 0.50;
    }

    return firstq;
}

void write_file(map<string, int> estimation_map, Value &sample){
    
    string output_string;
    output_string = FirstDate + "," + LastDate;

    if(estimation_map["mean"] == 1){
        double mean = calculate_mean(sample);
        output_string = output_string + "," + remove_trailing_zeros(to_string(mean));
    }
    if(estimation_map["std"] == 1){
        double std = calculate_std(sample);
        output_string = output_string + "," + remove_trailing_zeros(to_string(std));
    }
    if(estimation_map["min"] == 1){
        output_string = output_string + "," + remove_trailing_zeros(to_string(MinValue));
    }
    if(estimation_map["firstq"] == 1){
        double firstq = calculate_firstq(sample);
        output_string = output_string + "," + remove_trailing_zeros(to_string(firstq));
    }
    if(estimation_map["median"] == 1){
        double median = calculate_median(sample);
        output_string = output_string + "," + remove_trailing_zeros(to_string(median));
    }
    if(estimation_map["thirdq"] == 1){
        double thirdq = calculate_thirdq(sample);
        output_string = output_string + "," + remove_trailing_zeros(to_string(thirdq));
    }
    if(estimation_map["max"] == 1){
        output_string = output_string + "," + remove_trailing_zeros(to_string(MaxValue));
    }
    output_string = output_string + "\n";
    ofstream output_file;
    output_file.open(OUTPUT_FILE_NAME, ios::app);

    output_file << output_string;

    output_file.close();
}

void add_data(string data, map<string, int> estimation_map, string important_value, Value &sample){
    
    
    string date, time;
    double gap, grp, v, gi;
    int comma_index;
    double important_data;

    //parse the comma seperated data, update and erase the "data" string after finding and processing each data 
    comma_index = data.find(",");
    date = data.substr(0,comma_index);
    
    data.erase(0, comma_index+1);
    comma_index = data.find(",");
    time = data.substr(0,comma_index);
    
    data.erase(0, comma_index+1);
    comma_index = data.find(",");
    gap = stod(data.substr(0,comma_index));

    data.erase(0, comma_index+1);
    comma_index = data.find(",");
    grp = stod(data.substr(0,comma_index));

    data.erase(0, comma_index+1);
    comma_index = data.find(",");
    v = stod(data.substr(0,comma_index));

    data.erase(0, comma_index+1);
    comma_index = data.find(",");
    gi = stod(data.substr(0,comma_index));

    if(important_value == "gap"){important_data = gap;}
    if(important_value == "grp"){important_data = grp;}
    if(important_value == "v"){important_data = v;}
    if(important_value == "gi"){important_data = gi;}

    //if this is the first time program enters the function initilize some variables
    if(MaxValue == -1){
        MaxValue = important_data;
        MinValue = important_data;
        FirstDate = date + "," + time;
    }

    //update max and min value
    if(important_data > MaxValue){MaxValue = important_data;}
    if(important_data < MinValue){MinValue = important_data;}
    
    //if any of  mean, median, firstq, median, thirdq is asked we will use a vector otherwise not
    if(estimation_map["firstq"] == 1 || estimation_map["median"] == 1 || estimation_map["thirdq"] == 1 || estimation_map["mean"] == 1 || estimation_map["std"] == 1){
        sample.values.push_back(important_data);
    }

    LastDate = date + "," + time;
}


void read_file(string file_path, Value &sample){

    map<string, int> estimation_map;
    estimation_map.insert(pair<string, int>("mean", 0));
    estimation_map.insert(pair<string, int>("std", 0));
    estimation_map.insert(pair<string, int>("min", 0));
    estimation_map.insert(pair<string, int>("firstq", 0));
    estimation_map.insert(pair<string, int>("median", 0));
    estimation_map.insert(pair<string, int>("thirdq", 0));
    estimation_map.insert(pair<string, int>("max", 0));

    ofstream output_file;
    output_file.open(OUTPUT_FILE_NAME);
    output_file.close();

    ifstream input_file;
    input_file.open(file_path);

    string input_line;
    getline(input_file, input_line);
    int estimation_num = stoi(input_line);

    for(int i = 0; i < estimation_num; i++){
        getline(input_file, input_line);
        estimation_map[input_line] = 1;
    }

    getline(input_file, input_line);// an additional getline for the empty line
    getline(input_file, input_line);

    int line_count;
    string important_value;

    int comma_index = input_line.find(",");
    line_count = stoi(input_line.substr(0,comma_index));

    input_line.erase(0, comma_index + 1);
    important_value = input_line;

    int lines_read = 0;
    while(lines_read < line_count){
        getline(input_file, input_line);
        lines_read++;

        if(input_line == "add"){
            getline(input_file, input_line);
            lines_read++;
            add_data(input_line, estimation_map, important_value, sample);

        }else if(input_line == "print"){
            write_file(estimation_map, sample);
        }
    }
    input_file.close();
}

int main(int argc, char **argv){

    auto begin = std::chrono::high_resolution_clock::now();

    Value sample;

    read_file(argv[1], sample);

    auto end = std::chrono::high_resolution_clock::now();
    auto nanosecond =  std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    // cout << "time(ns): " << nanosecond << endl;
    // cout << "time(μs): " << nanosecond / 1000 << endl;
    // cout << "time(ms): " << nanosecond / 1000000 << endl;
    // cout << "time(s): " << nanosecond / 1000000000 << endl;
}