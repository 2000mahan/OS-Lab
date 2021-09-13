#include<stdio.h>
#include<stdlib.h>

struct Process{

  int pid;
  int bt;
  int wt;
  int tt;
  int priority;
};

struct Process * p;

int turnaroundTime(int i);
int waitingTime(int i);
void swap(int i , int j);
 
int main() {

  int totalWT = 0;
  int totalTT = 0;
  int count = 0;
 
  printf("Enter Total number of Processes:");
  scanf("%d",&count);
  p = (struct Process*) malloc(count*sizeof(struct Process));


  for(int i  = 1; i <= count; i++){
  	p[i-1].pid = i;
    printf("Enter Burst Time and Priority for Process %d: ", i);
    scanf("%d",&p[i-1].bt);
    scanf("%d",&p[i-1].priority);
    p[i-1].wt = 0;
    p[i-1].tt = 0;
  }

  for (int i = 0; i < count; ++i)
    for (int j = i; j < count; ++j){
      
      if (p[i].priority < p[j].priority)
          swap(i,j);
      else if (p[i].priority == p[j].priority && p[i].bt < p[j].bt)
          swap(i,j);
    }

  p[0].wt = 0;
  p[0].tt = p[0].bt;
  for (int i = 1; i < count; ++i){

     totalWT += waitingTime(i);
     totalTT += turnaroundTime(i);
  }  

  // Result
  printf("\n Process\t Priority\t BT\t TT\t WT\n\n");
  for (int i = 0; i < count; ++i){
    printf(" P%d\t\t %d\t\t %d\t %d\t %d\n", p[i].pid, p[i].priority, p[i].bt, p[i].tt, p[i].wt);
  }
  // printing Average Waiting Time
  printf("\nAverage Waiting Time = %f\n",totalWT*1.0/count);
  // printing Average TurnaroundTime
  printf("Average Turnaround Time = %f\n",totalTT*1.0/count);
  
  return 0;
}

void swap(int i, int j){
   
    int temp_id = p[i].pid;
    int temp_bt = p[i].bt;
    int temp_priority = p[i].priority;
    p[i].pid = p[j].pid;
    p[i].bt = p[j].bt;
    p[i].priority = p[j].priority;
    p[j].pid = temp_id;
    p[j].bt = temp_bt;
    p[j].priority = temp_priority;
}

int waitingTime(int i){
    p[i].wt = p[i-1].tt;
    return p[i].wt;
}

int  turnaroundTime(int i){
    p[i].tt = p[i].bt + p[i].wt;
    return p[i].tt;
}
