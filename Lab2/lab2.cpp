/*======================================================================
| Lab2
|
| Name: Lab2.cpp
|
| Written by: Chigozie Muonagolu - Febuary 2023
|
| Purpose: To simulate Scheduling Algorithms
|
| usage: ./lab2
|
| Description of properties:
|
|------------------------------------------------------------------
*/

#include <iostream>
#include <time.h>
#include <random>
#include <algorithm>
#include <limits.h>
#include  <list>
using namespace std;

//Class to represent a process object
class Process{
public:
    int ProcessId = -1;  //For comparing 
    int ArrivalTime;
    int RunTime;
    string status;
    int TimeLeft;
    bool ResponseCalculated = false;
    //Overload for == operator 
    bool operator==( Process& other) {
        return (this->ProcessId == other.ProcessId);
    }
};

const int LENGTH = 5000;    //Size of process array
Process processArray [LENGTH] = {}; //Array of processes

//Function declaration for Job 1
void Job1();
//Function declaration for Job 2
void Job2();
//Function declaration for Job 3
void Job3();

//Function declaration for FCFS Algorithm
void FCFS(Process array[], int Length);

//Function declaration for Shortest Job First Algorithm
void SJF(Process array[], int Length);

//Function declaration for Shortest Job First with pre-emption every 30 units
void STCF(Process array[], int Length);

//Function declaration for RoundRobin (Quantum Size or CPU time slice = 30, 75).
void RoundRobin(Process array[], int Length, int quantumSize);

//Helper function for sorting, according to job run time - Subroutine for SJF
bool CompareRunTime(Process a, Process b){
    return (a.RunTime < b.RunTime && (b.RunTime!= -1) && (b.ProcessId!=-1));
}

//Helper function for retunring the next process ready to run - Subroutine for SJF
Process GetNextProcess(int currentTime,list<Process> Pending){
    list<Process>::iterator it;
    Process toReturn;
    toReturn.ProcessId = -1;
    for (it = Pending.begin(); it != Pending.end(); it++) {
        //If the current process is arriving on time
        if(it->ArrivalTime <= currentTime){
            toReturn = *it;
            it = Pending.erase(it);  //remove it 
            return toReturn;
        }
    }
    return toReturn;
}

//Helper function to find the shortest process from RunningProcesses list - Subroutine for STCF
list<Process>::iterator GetShortestProcess(list<Process>& RunningProcesses){
    list<Process>::iterator shortest = RunningProcesses.begin();
    int min = shortest->TimeLeft;
    //Get the process with the shortest time remaining
    for (list<Process>::iterator it = RunningProcesses.begin(); it != RunningProcesses.end(); ++it) {
        if (it->TimeLeft > 0 && it->TimeLeft <= min) {
            shortest = it;
            min = shortest->TimeLeft;
        }
    }
    return shortest;
}

//Helper function to remove from RunningProcesses list - Subroutine for STCF
void RemoveProcess(list<Process>& RunningProcesses, Process* toRemove) {
    for (auto it = RunningProcesses.begin(); it != RunningProcesses.end(); ++it) {
        if (it->ProcessId == toRemove->ProcessId) {
            RunningProcesses.erase(it);
            break;
        }
    }
}

//Helper function to copy array content - Called in main
void ArrayCopy(Process oldArray [], Process newArray[]){
    for(int i =0; i< LENGTH; i++){
        newArray[i] = oldArray[i];
    }
}

//Decalrations for functions to run the jobs
void RunJob1();
void RunJob2();
void RunJob3();


int main(int argc, const char * argv[]) {

    int menu = -1;
    do{
        cout<<"From 1-3 enter a job number to select the job you wish to run: ";
        cin>>menu;
        switch (menu)
        {
        case 0:
            printf("Terminating program. \n");
            break;
        case 1:
            printf("Results for Job 1: \n");
            RunJob1();
            break;
        case 2:
            printf("Results for Job 2: \n");
            RunJob2();
            break;
        case 3:
            printf("Results for Job 3: \n");
            RunJob3();
            break;
        default:
            printf("Invlaid input!\n");
            break;
        }
    }while(menu!=0);
   
    return 0;
}

//Function definition for job 1
void Job1(){
    srand((unsigned)time(NULL));
    //Using c++ normal distribution subroutine
    normal_distribution<double> RunTime(150, 20);  //Generates the run time
    default_random_engine generator(rand() / RAND_MAX);
    normal_distribution<double> ArivalTime(75.0, 20.0); //Generates the arival time

    //Create the first job
    Process temp;
    temp.ProcessId = 0;
    temp.RunTime = RunTime(generator); 
    temp.ArrivalTime = ArivalTime(generator);
    temp.status = "not arrived";  
    temp.TimeLeft = temp.RunTime; 
    processArray[0] = temp;

    //For the length of process array
    for(int i =1; i< LENGTH -1; i++){
        Process temp;
        temp.ProcessId = i;
        //Generate the run time
        temp.RunTime = RunTime(generator);    
        //Generate the arrival time + the previous arrival time
        temp.ArrivalTime = ArivalTime(generator) + processArray[i-1].ArrivalTime;
        temp.status = "not arrived";  
        temp.TimeLeft = temp.RunTime; 
        processArray[i] = temp;
    }
}

//Function definition for job 2
void Job2(){
    srand((unsigned)time(NULL));
     //Using c++ normal distribution subroutine
    default_random_engine generator(rand() / RAND_MAX);
    normal_distribution<double> ArivalTime(75.0, 20.0); //The arrival time
    //Create the first job
    double size = ((double) rand() / (RAND_MAX));
    Process temp;
    temp.ProcessId = 0;
    //Large job 
    if(size > 0.8){     //Large job
        //Generate the run time
        normal_distribution<double> RunTime(250.0, 15.0); 
        temp.RunTime = RunTime(generator);    
    }
    else{   //Small job
        //Generate the run time
        normal_distribution<double> RunTime(50.0, 10.0); 
        temp.RunTime = RunTime(generator);    
    }
    temp.ArrivalTime = ArivalTime(generator);
    temp.status = "not arrived";  
    temp.TimeLeft = temp.RunTime; 
    processArray[0] = temp;

    //Create the rest of the jobs
    for(int i =1; i< LENGTH-1; i++){
        double size = ((double) rand() / (RAND_MAX));
        Process temp;
        temp.ProcessId = i;
        //Large job 
        if(size > 0.8){     //Large job
            //Generate the run time
            normal_distribution<double> RunTime(250.0, 15.0); 
            temp.RunTime = RunTime(generator);  
        }
        else{   //Small job
            //Generate the run time
            normal_distribution<double> RunTime(50.0, 10.0); 
            temp.RunTime = RunTime(generator);   
        }
        //Generate the arrival time + the previous arrival time
        temp.ArrivalTime = ArivalTime(generator) + processArray[i-1].ArrivalTime;   
        temp.status = "not arrived";  
        temp.TimeLeft = temp.RunTime; 
        processArray[i] = temp;
    }
}

//Function definition for job 3
void Job3(){
    srand((unsigned)time(NULL));
     //Using c++ normal distribution subroutine
    default_random_engine generator(rand() / RAND_MAX);
    normal_distribution<double> ArivalTime(75.0, 20.0); //The arrival time
    //Create the first job
    double size = ((double) rand() / (RAND_MAX));
    Process temp;
    temp.ProcessId = 0;
    //Large job 
    if(size > 0.8){   //Small job
            //Generate the run time
        normal_distribution<double> RunTime(50.0, 10.0); 
        temp.RunTime = RunTime(generator);     
    }
    else{    //Large job 
        //Generate the run time
        normal_distribution<double> RunTime(250.0, 15.0); 
        temp.RunTime = RunTime(generator);   
    }
    temp.ArrivalTime = ArivalTime(generator);
    temp.status = "not arrived"; 
    temp.TimeLeft = temp.RunTime;  
    processArray[0] = temp;

    //Create the rest of the jobs
    for(int i =1; i< LENGTH-1; i++){
        double size = ((double) rand() / (RAND_MAX));
        Process temp;
        temp.ProcessId = i;
        if(size > 0.8){    //Small job
            //Generate the run time
            normal_distribution<double> RunTime(50.0, 10.0); 
            temp.RunTime = RunTime(generator);    
        }
        else{    //Large job
            //Generate the run time
            normal_distribution<double> RunTime(250.0, 15.0); 
            temp.RunTime = RunTime(generator); 
        }
        //Generate the arrival time + the previous arrival time
        temp.ArrivalTime = ArivalTime(generator) + processArray[i-1].ArrivalTime;  
        temp.status = "not arrived"; 
        temp.TimeLeft = temp.RunTime;   
        processArray[i] = temp;
    }
}

//FCFS definition
void FCFS(Process array[], int Length){
    int turnAroundTime = 0;
    int sumOfTurnAroundTime = 0;
    int averageTurnAroundTime = 0;

    int responseTime = 0;
    int sumOfResponseTime = 0;
    int averageResponseAroundTime = 0;

    int currentTime = array[0].ArrivalTime;     //initial clocktime
    int startTime = currentTime;
    for(int i =0; i<Length; i++){
        array[i].status = "in progress";
         //Find the total response time
        if(i == 0){  //first job

            continue;
        }
        else{
            responseTime = abs(currentTime - array[i].ArrivalTime);
            sumOfResponseTime+=responseTime;
        }

        array[i].status = "completed";
        currentTime+=array[i].RunTime;  //Update the current time
        
        //Find the total turn around time
        turnAroundTime = (currentTime) - array[i].ArrivalTime;
        sumOfTurnAroundTime+=turnAroundTime;
    }
    averageTurnAroundTime = (sumOfTurnAroundTime/Length);
    averageResponseAroundTime = (sumOfResponseTime/Length);

    printf("FCFS: Average Response Time: %ld --- Average Turn around Time: %ld ---- Start Time: %d  ---- Finish Time: %d\n", averageResponseAroundTime, averageTurnAroundTime, startTime, currentTime);

}

//SJF definition
void SJF(Process array[] , int Length){
    //Set the current time to, the arrival time of the very first job
    int turnAroundTime = 0;
    int sumOfTurnAroundTime = 0;
    long averageTurnAroundTime = 0;

    int responseTime = 0;
    int sumOfResponseTime = 0;
    long averageResponseAroundTime = 0;

    int currentTime = array[0].ArrivalTime;
    int startTime = currentTime;
    int completeJobs = 0;
    int i = 0;
    list<Process> PendingProcesses;     //List of jobs that are yet to arrive
    Process current;
    sort(array, array+Length, CompareRunTime);   //Sort the processes accorinding to the shortest jobs first


    while(completeJobs < Length){
        //if the current job has arrived
        if(i < Length && array[i].ArrivalTime <= currentTime ){
            if(array[i].ProcessId == -1){
                i+=1;
                continue;
            }
            array[i].status = "arrived";
            PendingProcesses.push_back(array[i]);   //Add it and set its status to arrived
            i+=1;
        }
        else{   //Else no current process currently arriving on time
            if(i < Length){
                PendingProcesses.push_back(array[i]);    //Add it to the PendingProcesses list
            }
        }
        current = GetNextProcess(currentTime, PendingProcesses);    //Check for the quickest process arriving on time
        if(current.ProcessId == -1) //No process was found 
        {
            i+=1;
            continue;
        }
        //Run the current job
        if(current.ProcessId > -1){
            current.status = "in progress";
            //Find the total response time
            responseTime = abs(currentTime - current.ArrivalTime);
            sumOfResponseTime+=responseTime;

            current.status = "completed";
            currentTime+=current.RunTime;  //Update the current time

            //Find the total turn around time
            turnAroundTime = currentTime - current.ArrivalTime;
            sumOfTurnAroundTime+=turnAroundTime;
            i+=1;
            completeJobs+=1;
        }
    }
    averageTurnAroundTime = (sumOfTurnAroundTime/Length);
    averageResponseAroundTime = (sumOfResponseTime/Length);

    printf("SJF: Average Response Time: %ld --- Average Turn around Time: %ld  ---- Start Time: %d  ---- Finish Time: %d\n", averageResponseAroundTime, averageTurnAroundTime, startTime, currentTime);
}

//STCF definition
void STCF(Process array[], int Length){
    list<Process> RunningProcesses;
    list<Process> InterruptedProcesses;

    int turnAroundTime = 0;
    long sumOfTurnAroundTime = 0;
    long averageTurnAroundTime = 0;

    int responseTime = 0;
    long sumOfResponseTime = 0;
    long averageResponseAroundTime = 0;

    int clockSize = 30;   //Unit to interrupt a process 
    int currentTime = array[0].ArrivalTime;
    int startTime = currentTime;

    Process* current = new Process;
    int i = 0;
    int completedJobs = 0;
    int contextSwtch = 0;
    while(completedJobs < (Length-1)){        
        //Add any process that has arrived to the Running process list 
        while(i < Length && array[i].ArrivalTime <= currentTime){
            array[i].status =  "in progress";
            RunningProcesses.push_back(array[i]);
            i+=1;
        }
        
        //Preempt step
        //Check if any shorter job exists in the running process list than the current one
        if(!RunningProcesses.empty()){
            list<Process>::iterator shortest = GetShortestProcess(RunningProcesses); // refrence to the process with a shortest time
            if(current->ProcessId == -1){    //if first run
                current = &(*shortest);
            }
            else if (shortest->ProcessId != current->ProcessId) {
                current = &(*shortest);
                //Update context swtch
                contextSwtch +=1;
            }
            if(current->ResponseCalculated!=true){
                //Calculate response time
                responseTime = currentTime - current->ArrivalTime;
                sumOfResponseTime += responseTime;
                current->ResponseCalculated = true;
            }
        }
        //Update the current time and the current job's remaining time
        current->TimeLeft -= clockSize;
        currentTime += clockSize;

        if(current->ProcessId >= 0){
            //If the process just completed
        if(current->TimeLeft <= 0){
            
            current->status = "completed";
            //Calculate turn around time
            turnAroundTime = currentTime - current->ArrivalTime;
            sumOfTurnAroundTime += turnAroundTime;

            completedJobs += 1;
            RemoveProcess(RunningProcesses, current);
        }
        }
    }
    averageTurnAroundTime = (sumOfTurnAroundTime/Length);
    averageResponseAroundTime = (sumOfResponseTime/Length);

    printf("STCF: Average Response Time: %ld --- Average Turn around Time: %ld ---- Context Switches: %d  ---- Start Time: %d  ---- Finish Time: %d\n", averageResponseAroundTime, averageTurnAroundTime, contextSwtch, startTime, currentTime);
}

//Round Robin definition
void RoundRobin(Process array[], int Length, int quantumSize){
    list<Process> StartedProcesses;

    int turnAroundTime = 0;
    long sumOfTurnAroundTime = 0;
    long averageTurnAroundTime = 0;

    int responseTime = 0;
    long sumOfResponseTime = 0;
    long averageResponseAroundTime = 0; 

    int currentTime = array[0].ArrivalTime;     //Set the current time
    int startTime = currentTime;
    int completedJobs =0;
    int contextSwtch = 0;
    int i =0;
    Process current;
    while(completedJobs < Length){
        //Add processes that have arrived to the started list
        while(i < Length && array[i].ArrivalTime <= currentTime){
            Process processToAdd = array[i];
            processToAdd.status = "in progress";
            StartedProcesses.push_back(array[i]);
            i+=1;
        }
        
        if(!StartedProcesses.empty()){
            current = StartedProcesses.front();     //Pop the most recent process
            StartedProcesses.pop_front();   //Remove it from the started process list
            contextSwtch+=1;    //Update the context switch 
        }

        //If an active process is running 
       if(current.ProcessId >= 0){
         //Update the response time if the current job had just started 
        if (current.TimeLeft == current.RunTime) {
            responseTime = currentTime - current.ArrivalTime;
            sumOfResponseTime += responseTime;
        }

        current.TimeLeft-=quantumSize;  //Run for the CPU slice time duration
        currentTime+=quantumSize;    //Update current time
        current.status = "interrupted"; 
        StartedProcesses.push_back(current);    //add it back to the back of the list

        if(current.TimeLeft < quantumSize){
            completedJobs+=1;
            current.status = "completed";
            turnAroundTime = currentTime - current.ArrivalTime;
            sumOfTurnAroundTime += turnAroundTime;
        }
       }
       else{
            currentTime+=quantumSize;    //Update current time
       }
    }
    averageTurnAroundTime = (sumOfTurnAroundTime/Length);
    averageResponseAroundTime = (sumOfResponseTime/Length);

    printf("Round Robin with CPU time slice of %d: Average Response Time: %ld --- Average Turn around Time: %ld ---- Context Switches: %d  ---- Start Time: %d  ---- Finish Time: %d\n", quantumSize, averageResponseAroundTime, averageTurnAroundTime, contextSwtch, startTime, currentTime);
}


//Function to Run job 1
void RunJob1(){
    Job1();
    //Create a copy of the array
    Process processArray_copy [LENGTH];
    ArrayCopy(processArray, processArray_copy);
    //Run the algorithms
    FCFS(processArray, LENGTH);
    SJF(processArray_copy, LENGTH);
    STCF(processArray, LENGTH);
    RoundRobin(processArray, LENGTH, 30);
    RoundRobin(processArray, LENGTH, 75);    
}

//Function to Run job 2
void RunJob2(){
    Job2();
    //Create a copy of the array
    Process processArray_copy [LENGTH];
    ArrayCopy(processArray, processArray_copy);
    //Run the algorithms
    FCFS(processArray, LENGTH);
    SJF(processArray_copy, LENGTH);
    STCF(processArray, LENGTH);
    RoundRobin(processArray, LENGTH, 30);
    RoundRobin(processArray, LENGTH, 75);
}

//Function to Run job 3
void RunJob3(){
    Job3();
    //Create a copy of the array
    Process processArray_copy [LENGTH];
    ArrayCopy(processArray, processArray_copy);
    //Run the algorithms
    FCFS(processArray, LENGTH);
    SJF(processArray_copy, LENGTH);
    STCF(processArray, LENGTH);
    RoundRobin(processArray, LENGTH, 30);
    RoundRobin(processArray, LENGTH, 75);
}
