#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../includes/data_structures/queue.h"
#include "../includes/utils/algorithms_utils.h"

// ------------------ Round Robin  Algorithm
void Round_Robin(Queue* queue,int processes_number,int quantum){
    int quantum_value = quantum;
    int t=0;
    int rotation_time=0;
    int waiting_time=0;
    // The time when the process in idle state ( we will need it to calculate the waiting time)
    int wasted_time = 0;
    // Ready queue is a queue of the waiting processes a the current time
    Queue* ready_queue = create_queue();
    if(is_empty(queue)){
        exit(EXIT_FAILURE);
    }
    // Get the first process and enqueue it in the ready queue
    enqueue(ready_queue,dequeue(queue));
    // We stop when there is no new process ( queue is empty) and no waiting process ( ready queue is empty)
    while(!is_empty(queue) || ! is_empty(ready_queue)){

        Process current_process = dequeue(ready_queue);
        //  In case where there is no waiting process at that current time (t) ( process in idle state) : we increment t
        if(t<current_process.arrivalTime){
            print_seperator(t,processes_number);
            idle_state(processes_number);
            while(t<current_process.arrivalTime){
                t++;
                wasted_time++;
            }
        }
        print_seperator(t,processes_number);
        // Run the current process
        while(quantum>0 && current_process.runTime>0){
            quantum--;
            current_process.runTime--;
            t++;
        }
        print_process(current_process);
        
        // If a new process come (he is alrady in the queue), we will enqueue it to the ready queue
        while(!is_empty(queue)){
            if(queue->front->data.arrivalTime <=t){
                enqueue(ready_queue,dequeue(queue));
            }
            else{
                break;
            }
        }
        // We enqueue the current_process to the ready_queue in case it is not terminated. else we calculate the rotation and waiting time
        if(current_process.runTime>0){
            enqueue(ready_queue , current_process);     
        }    
        else{
            // Calculate the rotation and waiting time
            int current_rotation_time= t - current_process.arrivalTime;
            rotation_time+= current_rotation_time;
            waiting_time+=current_rotation_time ;
        }
        // Initialize the quantum for next process
        quantum = quantum_value ;
    }

        waiting_time += (wasted_time -t) ;
        print_seperator(t,processes_number);
        printf("Average rotation time : %.2fs\n" , (float)rotation_time/processes_number);
        printf("Average waiting time : %.2fs\n" , (float)waiting_time/processes_number);

    }

int main(void){

    Process processes[] = {
        {"Process1", 0, 5, 3},
        {"Process2", 1, 3, 2},
        {"Process3", 2, 1, 4},
        {"Process4", 4, 2, 1},

    };
    int processes_number = 4;
    Queue* q = create_queue_from_array(processes,processes_number);
    int quantum = 2;
    Round_Robin(q,processes_number,quantum);
    return 0;
}

