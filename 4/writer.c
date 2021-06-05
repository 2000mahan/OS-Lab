#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main (int argc, char *argv[])
{
    int segment_id;
	key_t key;
	key = 3232;
    char* shared_memory;
    int shared_segment_size = 100;

    struct shmid_ds shmbuffer;
    int segment_size;


    segment_id = shmget(key, shared_segment_size, IPC_CREAT | 0666);

    shared_memory = shmat (segment_id, NULL, 0);
    printf("shared memory attached at address %p\n", shared_memory);

    shmctl(segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    printf("segment size: %d\n", segment_size);

    sprintf(shared_memory, "%s", argv[1]);
    printf("This string inserted in shared memory: %s\n\n", argv[1]);
	
    printf("Waiting for reader...\n");
    while (*shared_memory != '*')
        sleep(1);

    shmdt(shared_memory);

    return 0;
}
