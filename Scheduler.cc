
#include <stdio.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <stdbool.h>
#include <fstream>
#include <iomanip>

using namespace std;


class Process {
    public:
    int PID = INT_MAX;
    int arrival = INT_MAX;
    int burst = INT_MAX;
    int priority = INT_MAX;
    int remaining_burst = INT_MAX;

    void setPID(int process_ID){
        PID = process_ID;
    }
    void setArrival(int arrival_time){
        arrival = arrival_time;
    }
    void setBurst(int burst_time){
        burst = burst_time;
    }
    void setPriority(int priority_level){
        priority = priority_level;
    }
    void setRemainingBurst(int burst_remainder){
        remaining_burst = burst_remainder;
    }

};

void datareader(vector<Process> &read_data, const string &file_name){

   ifstream myFile(file_name);
   string line;
   vector<vector<string> > big_man;

   while (getline(myFile, line)){

       stringstream current_string(line);
       string final;
       vector<string> little_man;

       while (getline(current_string, final, ',')){

           little_man.push_back(final);

       }

       big_man.push_back(little_man);

   }
   for (long unsigned int i = 0; i < big_man.size(); i++){
          
        Process big_p;
        big_p.setPID(stoi(big_man[i][0]));
        big_p.setArrival(stoi(big_man[i][1]));
        big_p.setBurst(stoi(big_man[i][2]));
        big_p.setPriority(stoi(big_man[i][3]));
        big_p.setRemainingBurst(stoi(big_man[i][2]));

        read_data.push_back(big_p);
   }
}

void FCFS(vector<Process> &data){

    struct SortFCFS
    {
        bool operator()(const Process &p1, const Process &p2){

            if (p1.arrival < p2.arrival){
                return true;
            }
            else if (p1.arrival == p2.arrival){
                if (p1.PID < p2.PID)
                {
                    return true;
                }
                
            }
            return false;
        }
    };

    sort(data.begin(), data.end(), SortFCFS());

    int time = 0;
    int turnaround_time=0;
    int waiting_time=0;
    int counter = 0;
    vector<Process> Completed;

    while(Completed.size() != data.size()){

        if (data[counter].arrival <= time){

            data[counter].remaining_burst--;
        }
        time++;
        if (data[counter].remaining_burst == 0)
        {
            Completed.push_back(data[counter]);
            turnaround_time += time - data[counter].arrival;
            waiting_time += (time - data[counter].arrival) - data[counter].burst; 
            counter++;
        }
    }





//gap proccess finishes before next one arrives 

    double average_turnaround_time = static_cast<double>(turnaround_time)/data.size();
    double average_waiting_time = static_cast<double>(waiting_time)/data.size();
    double throughput = static_cast<double>(data.size())/time;

    cout<<fixed<<setprecision(3)<<"FCFS: Average_turnaround_time: " << average_turnaround_time<<'\n';
    cout<<"FCFS: Avareage_wait_time: " << average_waiting_time<<'\n';
    cout<<"FCFS: Throughput: " << throughput << '\n';

}

void SJF (vector<Process> &data){
    
    struct SortSJF
    {
        bool operator()(const Process &p1, const Process &p2){

            if (p1.arrival == p2.arrival){
                if (p1.burst == p2.burst){
                    return p1.PID < p2.PID;
                }
                return p1.burst < p2.burst;
            }
            return p1.arrival <p2.arrival;
        }
    };
        struct SortWaitingSJF
    {
        bool operator()(const Process &p1, const Process &p2){

            if (p1.remaining_burst == p2.remaining_burst){
                if (p1.arrival == p2.arrival){
                    return p1.PID < p2.PID;
                }
                return p1.arrival < p2.arrival;
            }
            return p1.remaining_burst < p2.remaining_burst;
        }
    };

    sort(data.begin(), data.end(), SortSJF());

    int current_p = 0;
    int current_time = 0;
    long unsigned int complete_proccesses = 0;
    int completion_time = 0;
    int tournaround_time = 0;
    int waiting_time = 0;
    vector<Process> waiting_p;
    Process* current_proccess = &data[0];

    while (complete_proccesses != data.size()){

        if (data[current_p].arrival == current_time){
            while (data[current_p].arrival == current_time){
                waiting_p.push_back(data[current_p]);
                current_p++;
            }
        }
            sort(waiting_p.begin(), waiting_p.end(), SortWaitingSJF());

            if(waiting_p.size() != 0){
                current_proccess = &waiting_p[0];
            }

            if(waiting_p[0].remaining_burst < current_proccess->remaining_burst){
                current_proccess = &waiting_p[0];
            }
            current_proccess->remaining_burst --;
            current_time ++;
            if(current_proccess->remaining_burst == 0){
                completion_time = current_time;
                tournaround_time += completion_time - current_proccess->arrival;
                waiting_time += completion_time - current_proccess->arrival - current_proccess->burst;
                complete_proccesses ++;
                waiting_p.erase(waiting_p.begin());
            }

    }

    double average_turnaround_time = static_cast<double>(tournaround_time)/data.size();
    double average_waiting_time = static_cast<double>(waiting_time)/data.size();
    double throughput = static_cast<double>(data.size())/current_time;

    cout<<"SJF: Average_turnaround_time: " << average_turnaround_time<<'\n';
    cout<<"SJF: Avareage_wait_time: " << average_waiting_time<<'\n';
    cout<<"SJF: Throughput: " << throughput << '\n';
}

void priority(vector<Process> &data){

    struct SortPriority
    {
        bool operator()(const Process &p1, const Process &p2){

            if (p1.arrival == p2.arrival){
                if (p1.priority == p2.priority){
                    return p1.PID < p2.PID;
                }
                return p1.priority < p2.priority;
            }
            return p1.arrival <p2.arrival;
        }
    };
        struct SortWaitingPriority
    {
        bool operator()(const Process &p1, const Process &p2){

            if (p1.priority == p2.priority){
                if (p1.arrival == p2.arrival){
                    return p1.PID < p2.PID;
                }
                return p1.arrival < p2.arrival;
            }
            return p1.priority < p2.priority;
        }
    };

    sort(data.begin(), data.end(), SortPriority());

    int current_p = 0;
    int current_time = 0;
    long unsigned int complete_proccesses = 0;
    int completion_time =0;
    int tournaround_time =0;
    int waiting_time = 0;
    vector<Process> waiting_p;
    Process* current_proccess = &data[0];

    while (complete_proccesses != data.size()){

        if (data[current_p].arrival == current_time){
            while (data[current_p].arrival == current_time){
                waiting_p.push_back(data[current_p]);
                current_p++;
            }
        }
            sort(waiting_p.begin(), waiting_p.end(), SortWaitingPriority());
            if(waiting_p.size() != 0){
                current_proccess = &waiting_p[0];
            }
            if(waiting_p[0].priority < current_proccess->priority){
                current_proccess = &waiting_p[0];
            }
            current_proccess->remaining_burst --;
            current_time ++;
            if(current_proccess->remaining_burst == 0){
                completion_time = current_time;
                tournaround_time += completion_time - current_proccess->arrival;
                waiting_time += completion_time - current_proccess->arrival - current_proccess->burst;
                complete_proccesses ++;
                waiting_p.erase(waiting_p.begin());
            }

    }

    double average_turnaround_time = static_cast<double>(tournaround_time)/data.size();
    double average_waiting_time = static_cast<double>(waiting_time)/data.size();
    double throughput = static_cast<double>(data.size())/current_time;

    cout<<"Proirity: Average_turnaround_time: " << average_turnaround_time<<'\n';
    cout<<"Proirity: Avareage_wait_time: " << average_waiting_time<<'\n';
    cout<<"Proirity: Throughput: " << throughput << '\n';
}


int main(int argc, char** argv){

    if (argc !=2){
        cout<< "Incorrect number of arguments" << std::endl;
        return -1;
    }

    cout<< argc <<'\n';

    vector<Process> test_FCFS;
    vector<Process> test_SJF;
    vector<Process> test_Priority;
    string file = argv[1];

    datareader(test_FCFS, file);
    datareader(test_SJF, file);
    datareader(test_Priority, file);

    cout<< "------FCFS-------\n";
    FCFS(test_FCFS);
    cout<< "------SJF------- \n";
    SJF(test_SJF);
    cout<< "------Priority-------\n";
    priority(test_Priority);

    return 0;
}