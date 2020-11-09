#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
/*include ip_checksum.h*/

int main(int argc, char *argv[])
{
	int status;

	pthread_t prod;
	pthread_t con;

	pthread_attr_t attr;

	if (argc != 2)
	{
		printf("Please enter two commands EX ( ./prodcon <nitems> \n)");
		return -1;
	}

	semid = sem_open("pSem", O_CREAT | O_EXCL, 0644, 0);
	if(semid == NULL)
	{
		perror("Semaphore init failed \n");
		return -1;
	}

	semid_full = sem_open("pSemfull", O_CREAT | O_EXCL, 0644, 0);
	if(semid_full == NULL)
	{
		perror("semaphore init failed \n");
		return -1 ;
	}

	pthread_attr_init(&attr);

	pthread_mutex_init(&mutex, 0);

	memsize = atoi(argv[1]);
	printf(memsize);

	pthread_create(&prod, &attr, producer, NULL);
	pthread_create(&con, &attr, consumer, NULL);

	printf("Thread Created \n");

	pthread_join(prod, NULL);
	pthread_join(con, NULL);

	printf("Threads created \n");

	pthread_join(prod,NULL);
	pthread_join(con, NULL);

	printf("Thread finished\n");
	sem_unlink("psem");
	sem_unlink("pSemFull");
	pthread_mutex_destroy(&mutex);

	return 0;
}
void *producer(void *args)
{
	printf("Producer \n");
	item next_produced;

	next_produced.item_no = 0;

	while(1)
	{
		next_produced.item_no += 1;

		for(int 1 = 0; 1 < memsize; i ++)
		{
			next_produced.payload[i] = (unsigned char)rand() % 256;
		}
		next_produced.cksum = (unsigned short)ip_checksum(&next_produced.payload[0], PAYLOAD_SIZE);

		sem_wait(semid);
		pthread_mutex_lock(mutex);

		memcpy((void *)&buffer[header->in], &next_produced, sizeof(item));

		header->in = (header->in + 1) % BUFFER_SIZE;

		sem_post(semid_full);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}
void *consumer(void *args)
{
	item next_consumed;
	print("CONSUMER \n");

	while(1)
	{
		printf("Consumer check \n");
		sem_wait(semid_full);
		printf("after sem_wait \n");
		pthread_mutex_lock(&mutex);

		print("after pthread mutex lock \n");

		memcpy((void *)&next_consumed, (void *)&buffer[header->out], sizeof(item));

		printf("consumer check 123 \n");


		header->out = (header-><out + 1)% memsize;
	}
}
