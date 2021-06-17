#include<stdio.h>
 
int main()
{
 
  int count = 0;
  int numberOfProcesses = 0; 
  int time = 0;
  int remainingProcesses = 0;
  int flag = 0;
  int timeQuantum = 0;
  int waitingTime = 0;
  int turnaroundTime = 0;
  int arrivalTime[100];
  int burstTime[100];
  int needingTime[100];

  printf("Enter Total number of Processes:");
  scanf("%d",&numberOfProcesses);

  remainingProcesses = numberOfProcesses;

  for(count = 0;count < numberOfProcesses;count++)
  {
  	int processesNumber = count + 1;
    printf("Enter Arrival Time and Burst Time for Process Number %d :",processesNumber);
    scanf("%d",&arrivalTime[count]);
    scanf("%d",&burstTime[count]);
    needingTime[count]=burstTime[count];
  }

  printf("Enter the Time Quantum(q):");
  scanf("%d",&timeQuantum);
  printf("\nProcess\t TurnaroundTime WaitingTime\n\n");

  time = 0;
  count = 0;
  while(remainingProcesses > 0)
  {
    if(needingTime[count] <= timeQuantum && needingTime[count] > 0)
    {
      time+=needingTime[count];
      needingTime[count] = 0;
      flag = 1;
    }
    else if(needingTime[count] > 0)
    {
      needingTime[count] -= timeQuantum;
      time+=timeQuantum;
    }
    if(needingTime[count] == 0 && flag == 1)
    {
      remainingProcesses--;
      int processNumber = count + 1;
      int processTurnaroundTime = time - arrivalTime[count];
      int processWaitingTime = time - arrivalTime[count] - burstTime[count];
      printf("P%d\t \t%d\t \t%d\n", processNumber, processTurnaroundTime,processWaitingTime);

      waitingTime += time-arrivalTime[count]-burstTime[count]; // Turnaround Time - burst Time
      turnaroundTime += time-arrivalTime[count]; // departure Time(time variable in here) - arrival Time
      
      flag=0;
    }
    if(count == numberOfProcesses - 1)
      count=0;
    else if(arrivalTime[count + 1] <= time)
      count++;
    else
      count=0;
  }

  // printing Average Waiting Time
  printf("\nAverage Waiting Time = %f\n",waitingTime*1.0/numberOfProcesses);

  // printing Average TurnaroundTime
  printf("Average Turnaround Time = %f\n",turnaroundTime*1.0/numberOfProcesses);
  
  return 0;
}