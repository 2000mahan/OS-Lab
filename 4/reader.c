#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>



int main()
{
    int segment_id;
    key_t key;
    char * shared_memory;
    int shared_segment_size = 100;

    key = 3232;

    segment_id = shmget(key, shared_segment_size,IPC_CREAT |0666);

    shared_memory = shmat(segment_id, NULL, 0);

    printf("%s\n", shared_memory);

    *shared_memory = '*';

    exit(0);
}