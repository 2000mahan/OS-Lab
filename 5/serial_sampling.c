#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(){

	int iteration = 0;
	printf("Enter iteration: ");
	scanf("%d", &iteration);

	srand(time(0));
	clock_t start = clock();

	int hist[25] = {0}; //0 to 11 : -12 to -1 , 12 to 24 : 0 to 12 => index + 12

	for (int i = 0; i < iteration; ++i)	{

		int counter = 0;
		for (int j = 0; j < 12; ++j){
			
			int random = rand()%(100 - 0 + 1);
			if (random >= 49)
				counter++;
			else
				counter--;
		}
		hist[counter + 12] += 1;
	}

	// time
	clock_t end = clock();
	double time_spent = (double)(end - start)/CLOCKS_PER_SEC;
	printf("\nTime Spent %lf\n", time_spent);

	int sum = 0;
	// print hist
	for (int i = 0; i < 25; ++i){
		sum += hist[i];
		printf(" %d : %d |", i - 12, hist[i]);
	}
	// must be equal to iteration
	printf("\n sum of the cells is : %d \n", sum);

	// print Histogram
	for (int i = 0; i < 25; ++i){

		printf("hist[%d] : ", i - 12);

		for (int j = 0; j < (int)(hist[i]/(iteration/100)); ++j)
			printf("*");
		printf("\n");
	}

	return 0;
}