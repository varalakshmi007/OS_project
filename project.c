#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define seats 3

sem_t sem_S,sem_T;
pthread_mutex_t thread;

int r_sleep = 5;
int chair[3];
int waiting=0;
int nextturn=0; 
int nextteacher=0;

int random()
{
	int time;
	time = random() % r_sleep + 1;
	sleep(time);
}

void *s_working(void *id);
void *t_working();

int main()
{
	int Num_of_students,*id,i;
	pthread_t *students,teacher;
	
	printf("How many students are there?\n");
	scanf("%d",&Num_of_students);
	
	students = (pthread_t*)malloc(sizeof(pthread_t)* Num_of_students);
	id = (int*)malloc(sizeof(int)*Num_of_students);
	
	pthread_mutex_init(&thread,NULL);
	sem_init(&sem_S,0,0);
	sem_init(&sem_T,0,1);
	
	pthread_create(&teacher,NULL,t_working,NULL);
	
	for(i=0;i<Num_of_students;i++)
	{
		id[i]=i+1;
		pthread_create(&students[i],NULL,s_working,(void*)&id[i]);
	}
	
	pthread_join(teacher,NULL);
	
	for(i=0;i<Num_of_students;i++)
	{
		pthread_join(students[i],NULL);
	}
	return 0;
}


void* s_working(void* id)
{
	int sid= *(int*)id;
	printf("CURRENT: student %d is learning from teacher\n",sid);
	while(1)
	{
		pthread_mutex_lock(&thread);
		
		if(waiting < seats)	
		{
			chair[nextturn] = sid;
			waiting++;
			
			printf("student %d is waiting on the chair outside.\n",sid);
			printf("waiting students : 1. %d\n 2. %d\n 3. %d\n",chair[0],chair[1],chair[2]);
			
			nextturn = (nextturn+1) % seats;
			
			pthread_mutex_unlock(&thread);
	
			sem_post(&sem_S);
			sem_wait(&sem_T);
		}
		else 
		{
			pthread_mutex_unlock(&thread);
			
			printf("No chairs,student %d is programming\n",sid);		
		}
	}
}
void *t_working()
{
	while(1)
	{
		sem_wait(&sem_S);	
		
		pthread_mutex_lock(&thread);
		
		printf("TA is teaching student %d\n",chair[nextteacher]);	
		chair[nextteacher]=0;
		waiting--;
		
		printf("waiting students : 1 - %d/n 2 - %d/n 3 - %d\n",chair[0],chair[1],chair[2]);
		nextteacher = (nextteacher + 1) % seats;
		
		sleep(1);

		printf("Teacher work is done\n");	

		pthread_mutex_unlock(&thread);

		sem_post(&sem_T);
	}	
}







