#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;



class RoomTime{
    public:
    float start;
    float end;
    // int salon_number;
    int capacity;

    string place, salon, start_date, end_date;

    RoomTime(string place, string salon, string start_date, string end_date){
        this -> place = place;
        this -> salon = salon;
        this -> start_date = start_date;
        this -> end_date = end_date;
        
        int index;
        
        string hours, minutes, number;
        index = start_date.find(":");
        hours = start_date.substr(0, index);
        minutes = start_date.substr(index + 1, start_date.size()-1);
        start = stoi(hours) + stoi(minutes)/60.0;

        index = end_date.find(":");
        hours = end_date.substr(0, index);
        minutes = end_date.substr(index + 1, end_date.size()-1);
        end = stoi(hours) + stoi(minutes)/60.0;

        // index = salon.find("_");
        // number = salon.substr(index + 1, salon.size()-1);
        // salon_number = stoi(number);
        // cout << start_date << " " << index << " " << hours << " " << minutes << " " << start << endl;
    }
};

class StageTime{
    public:
    float start;
    float end;
    // int salon_number;
    int capacity;

    string place, start_date, end_date;

    StageTime(string place, string start_date, string end_date, int daily_capacity){
        this -> place = place;
        this -> start_date = start_date;
        this -> end_date = end_date;
        
        int index;
        
        string day, month, number;
        int start_day, start_month, end_day, end_month;

        index = start_date.find(".");
        start_day = stoi(start_date.substr(0, index));
        start_month = stoi(start_date.substr(index + 1, start_date.size()-1));
        start = start_day + start_month * 31;
        
        index = end_date.find(".");
        end_day = stoi(end_date.substr(0, index));
        end_month = stoi(end_date.substr(index + 1, end_date.size()-1));
        end = end_day + end_month * 31;

        int total_days = end - start;
        capacity = daily_capacity * total_days;
    }
};

class Stage{
    public:
        string stage_name;
        vector<RoomTime> Rooms;
        vector<int> chair_counter;
        vector<vector<RoomTime>> path;
        int total_capacity;

        void insertion_sort_rooms();
        void interval_scheduling();

        Stage(string name){
            stage_name = name;
        }
};

class Place{
    public:
        vector<Stage> Stages;

        vector<StageTime> Stage_time;
        vector<int> chair_counter;
        vector<vector<StageTime>> path;
        int total_capacity;
        

        void read_daily_schedule(string);
        void read_capacity(string);
        void sort_rooms();
        void best_for_each_place(string);

        void read_availability(string);
        void insertion_sort_stages();
        void best_tour(string);
        void interval_scheduling_tour();
        
        
};


void Place::insertion_sort_stages(){
    
    int n = Stage_time.size();

    int i, j;
    StageTime key = Stage_time[0];
    for (i = 1; i < n; i++){ 
        key = Stage_time[i]; 
        j = i - 1;

        while (j >= 0 && Stage_time[j].end > key.end){ 
            Stage_time[j + 1] = Stage_time[j]; 
            j = j - 1; 
        } 
        Stage_time[j + 1] = key; 
    } 
}

void Place::read_availability(string case_name){
    string file_path = "inputs/" + case_name + "/availability_intervals.txt";

    ifstream file;
    file.open(file_path);
    if(!file.is_open()){
        cout << file_path << " could not open!";
        exit(1);
    }

    string a_line;
    getline(file, a_line);

    string place = "", start_date, end_date, prev_place;
    int daily_capacity = 0, prev_daily_capacity;

    int size = Stages.size();

    while(getline(file, a_line)){

        prev_place = place;
        prev_daily_capacity = daily_capacity;

        stringstream(ss_line);
        ss_line << a_line;
        ss_line >> place >> start_date >> end_date;
        
        if(prev_place != place){
            for(int i = 0; i < size; i++){
                if(Stages[i].stage_name == place){
                    daily_capacity = Stages[i].total_capacity;
                    break;
                }
            }
        }else{
            daily_capacity = prev_daily_capacity;
        }
        
        StageTime new_stage(place, start_date,end_date, daily_capacity);
        
        Stage_time.push_back(new_stage);
        chair_counter.push_back(0);

        vector<StageTime> t_path;
        path.push_back(t_path);
    }
}

void Place::read_capacity(string case_name){
    string file_path = "inputs/" + case_name + "/capacity.txt";

    ifstream file;
    file.open(file_path);
    if(!file.is_open()){
        cout << file_path << " could not open!";
        exit(1);
    }

    string a_line;
    getline(file, a_line);

    string stage, salon, capacity;

    int size = Stages.size();
    while(getline(file, a_line)){
        stringstream(ss_line);
        ss_line << a_line;
        ss_line >> stage >> salon >> capacity;
        
        for(int i = 0; i < size; i++){
            if(stage == Stages[i].stage_name){
                int stage_size = Stages[i].Rooms.size();
                for(int j = 0; j < stage_size; j++){
                    if(Stages[i].Rooms[j].salon == salon){
                        Stages[i].Rooms[j].capacity = stoi(capacity);
                    } 
                }
                break;
            }
        }
    }
}



void Stage::insertion_sort_rooms(){
    
    int n = Rooms.size();

    int i, j;
    RoomTime key = Rooms[0];
    for (i = 1; i < n; i++){ 
        key = Rooms[i]; 
        j = i - 1;

        while (j >= 0 && Rooms[j].end > key.end){ 
            Rooms[j + 1] = Rooms[j]; 
            j = j - 1; 
        } 
        Rooms[j + 1] = key; 
    } 
}

void Place::sort_rooms(){
    
    int size = Stages.size();

    for(int i = 0; i < size; i++){
        Stages[i].insertion_sort_rooms();
    }
}

void Place::read_daily_schedule(string case_name){
    string file_path = "inputs/" + case_name + "/daily_schedule.txt";

    ifstream file_ds;
    file_ds.open(file_path);
    if(!file_ds.is_open()){
        cout << file_path << " could not open!";
        exit(1);
    }

    string a_line;
    
    // make a read to ignore first line
    getline(file_ds, a_line);

    string start_time, end_time;
    string stage = "", salon, prev_stage;

    
    Stage temp_stage("current_stage");
    

    while(getline(file_ds, a_line)){
        
        prev_stage = stage;
        stringstream(ss_line);
        ss_line << a_line;
        ss_line >> stage >> salon >> start_time >> end_time;
        

        if(prev_stage != stage && prev_stage != ""){
            Stages.push_back(temp_stage);
            Stage new_stage(stage);
            temp_stage = new_stage;
            
        }
        temp_stage.stage_name = stage;
        
        RoomTime new_room(stage, salon, start_time, end_time);
        temp_stage.Rooms.push_back(new_room);
        temp_stage.chair_counter.push_back(0);

        vector<RoomTime> path;
        temp_stage.path.push_back(path);
        // cout << stage << salon << start_time << end_time << endl;
        // cout << a_line << endl;
    }
    Stages.push_back(temp_stage);
}

void Stage::interval_scheduling(){

    int size = Rooms.size();
    int including_chairs;
    
    for(int i = 0; i < size; i++){
        
        including_chairs = Rooms[i].capacity;
        int l = -1;

        for (int j = i - 1; j >= 0; j--) {
            if (Rooms[j].end <= Rooms[i].start){
                l = j;
                break;
            }
        }

        if (l != -1){
            including_chairs += chair_counter[l];
            path[i] = path[l];
        }
        
        if(i == 0 || including_chairs >= chair_counter[i - 1]){
            chair_counter[i] = including_chairs;
            path[i].push_back(Rooms[i]);
        }else{
            chair_counter[i] = chair_counter[i - 1];
            path[i] = path[i - 1];
        }
        
    }
}

void Place::best_for_each_place(string case_name){

    int stage_size = Stages.size();
    
    string a_string;

    string output_path = "outputs/" + case_name.substr(0,4) + case_name.substr(5,case_name.size()-5) + "/best_for_eachplace.txt";
    ofstream output_file(output_path);

    for(int i = 0; i < stage_size; i++){
        Stages[i].interval_scheduling();

        int room_size = Stages[i].Rooms.size();

        a_string = Stages[i].stage_name + " --> " + to_string(Stages[i].chair_counter[room_size - 1]) + "\n";
        Stages[i].total_capacity = Stages[i].chair_counter[room_size - 1];
        
        int path_size = Stages[i].path[room_size-1].size();
        for(int j = 0; j < path_size; j++){
            a_string += Stages[i].path[room_size-1][j].place + "     " + 
                        Stages[i].path[room_size-1][j].salon + "     " + 
                        Stages[i].path[room_size-1][j].start_date + "   " + 
                        Stages[i].path[room_size-1][j].end_date + "\n";
        }
        a_string += "\n";
        output_file << a_string;
    }
}

void Place::interval_scheduling_tour(){

    int size = Stage_time.size();
    int including_chairs;
    
    for(int i = 0; i < size; i++){
        
        including_chairs = Stage_time[i].capacity;
        int l = -1;

        for (int j = i - 1; j >= 0; j--) {
            if (Stage_time[j].end <= Stage_time[i].start){
                l = j;
                break;
            }
        }

        if (l != -1){
            including_chairs += chair_counter[l];
            path[i] = path[l];
        }
        
        if(i == 0 || including_chairs >= chair_counter[i - 1]){
            chair_counter[i] = including_chairs;
            path[i].push_back(Stage_time[i]);
        }else{
            chair_counter[i] = chair_counter[i - 1];
            path[i] = path[i - 1];
        }
        
    }
}

string string_to_date(string date){
    int index;
    string return_string = "", day, month;

    index = date.find(".");
    day = date.substr(0, index);
    month = date.substr(index + 1, date.size()-1);

    if(month == "05"){
        return_string = "May ";
    }else if(month == "06"){
        return_string = "June ";
    }
    
    return_string += day;
    return return_string;
}

void Place::best_tour(string case_name){
    
    
    string a_string;

    string output_path = "outputs/" + case_name.substr(0,4) + case_name.substr(5,case_name.size()-5) + "/best_tour.txt";
    ofstream output_file(output_path);

    interval_scheduling_tour();
    int size = Stage_time.size();

    a_string = "Total Revenue --> " + to_string(chair_counter[size - 1]) + "\n";
    total_capacity = chair_counter[size - 1];

    int path_size = path[size-1].size();
    for(int j = 0; j < path_size; j++){
            a_string += path[size-1][j].place + "     " + 
                        string_to_date(path[size-1][j].start_date) + "   " + 
                        string_to_date(path[size-1][j].end_date) + "\n";
    }
    a_string += "\n";
    output_file << a_string;
}

int main(int argc, char **argv){

    Place PlaceObj;

    string case_name;
    string case_number = argv[1];
    case_name = "case_" + case_number;
    // case_name = "case_2";

    PlaceObj.read_daily_schedule(case_name);
    PlaceObj.sort_rooms();
    PlaceObj.read_capacity(case_name);
    PlaceObj.best_for_each_place(case_name);

    PlaceObj.read_availability(case_name);
    PlaceObj.insertion_sort_stages();
    PlaceObj.best_tour(case_name);
    
    exit(0);
}