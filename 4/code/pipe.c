#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  int file_des[2]; // files descriptors of the pipe
  char Str1[100] = "This is First Process";
  int i;
  int counter = 4;

 
  if (pipe(file_des) == -1) { // handles pipe error
    perror("pipe()");
    exit(EXIT_FAILURE);
  }

  switch (fork()) {
    case -1: 
      perror("fork()");
    case 0: // child
      if (close(file_des[0]) == -1) { // close reading side of pipe
        perror("close()");
        exit(EXIT_FAILURE);
      }

      /* child writes to pipe */
      FILE *fw;

      fw = fdopen(file_des[1], "w"); //  convert file descriptor to FILE handler

	  
    	for (i = 0; Str1[i]!='\0'; i++)
    	{
    		if(Str1[i] == ""){
    			counter = counter + 1;
    			printf("hey");
    		}

    		if(Str1[i] >= 'a' && Str1[i] <= 'z')
    		{
    			Str1[i] = Str1[i] - 32;
  		}		
    		else if(Str1[i] >= 'A' && Str1[i] <= 'Z')
    		{
    			Str1[i] = Str1[i] + 32;
  		}
    	}
	 
      fprintf(fw, "%s", Str1);


      break;
    default: // parent

      if (close(file_des[1]) == -1) { // close writing side of pipe
        perror("close()");
        exit(EXIT_FAILURE);
      }

      /* parent reads from pipe */
      FILE *fr;
      char result[100];

      fr = fdopen(file_des[0], "r"); //  convert file descriptor to FILE handler
	  while(counter != 0){
	  fscanf(fr, "%s", &result);
      printf("%s", result);
      printf(" ");
      counter--;
  }
     	printf("\n");

      break;
  }
}
