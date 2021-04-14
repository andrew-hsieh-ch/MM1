#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "initialize.h"
#include "lcgrand.h"

#define IDLE 0
#define BUSY 1
#define Q_limit 10000

void arrive(int* server_status, int* num_in_queue, int* num_custs_delayed,
	float* sim_time, float* area_under_Q, float* last_event_time, float* time_since_last_event,
	float* time_arrival[Q_limit], float* mean_interarrival_time, float* mean_service_time,
	float* next_arr_time, float *next_dept_time);

void depart(int* server_status, int* num_in_queue, int* num_custs_delayed, float* sim_time, float* area_under_Q,
	float* last_event_time, float* time_since_last_event, float* time_arrival[Q_limit],
	float* mean_service_time, float* next_dept_time, float* q_delay, float* total_q_delay);

float expon(float mean);


int main()
{
    int server_status=IDLE;     // initialize
    int num_in_queue=0;
    int num_custs_delayed=0;
    int num_delays_required=10000;
    float sim_time=0.0;
    float total_q_delay=0.0;
    float q_delay = 0.0;
    float avg_delay=0.0;
    float avg_num_in_queue=0.0;
    float area_under_Q=0.0;
    float last_event_time=0.0;
    float time_since_last_event=0.0;
    float time_arrival[Q_limit];
    float next_dept_time=pow(10,30);

	float mean_interarrival_time = 0.0; //initialize input
    float mean_service_time = 0.0;



	printf("Enter mean_interarrival_time\n");
	scanf("%f", &mean_interarrival_time);



	printf("Enter mean_service_time\n");
	scanf("%f", &mean_service_time);

    next_arr_time=expon(mean_interarrival_time);


    for (int i=0;i<Q_limit;i++){ // initialize time_arrival
        time_arrival[i]=0.0;
    }





    while(num_custs_delayed<num_delays_required){

        if(next_arr_time<next_dept_time){

            arrive(&server_status,&num_in_queue,&num_custs_delayed,&sim_time,&area_under_Q,&last_event_time,&time_since_last_event,
		&time_arrival[Q_limit],&mean_interarrival_time,&mean_service_time,&next_arr_time,&next_dept_time);
        }
        else{

			depart(&server_status, &num_in_queue, &num_custs_delayed, &sim_time, &area_under_Q,
				&last_event_time, &time_since_last_event, &time_arrival[Q_limit],
				&mean_service_time, &next_dept_time, &q_delay, &total_q_delay);
        }

    }
    printf("\n");
    printf("!!!");//report;
    printf("sim_time= %d \n",sim_time);         // testing initialized value
    printf("server_status= %d \n",server_status);
    printf("num_in_queue= %d \n",num_in_queue);
    printf("num_custs_delayed= %d \n",num_custs_delayed);
    printf("num_delays_required= %d \n",num_delays_required);
    printf("total_delay= %.4f \n",total_delay);
    printf("avg_delay= %.4f \n",avg_delay);
    printf("avg_num_in_queue= %.4f \n",avg_num_in_queue);
    printf("area_under_Q= %.4f \n",area_under_Q);
    printf("last_event_time= %.4f \n",last_event_time);
    printf("time_since_last_event= %.4f \n",time_since_last_event);

    return 0;

}

void arrive(int* server_status,int* num_in_queue,int* num_custs_delayed,
float* sim_time,float* area_under_Q,float* last_event_time,float* time_since_last_event,
float* time_arrival[Q_limit],float* mean_interarrival_time,float* mean_service_time,
float* next_arr_time,float *next_dept_time) // change to call by reference

{
	*sim_time = *next_arr_time;
	*next_arr_time = *sim_time + expon(*mean_interarrival_time);


	*time_since_last_event = *sim_time - *last_event_time;
	*last_event_time = *sim_time;
	*area_under_Q += *num_in_queue * *time_since_last_event;


	if(*server_status == BUSY)
	{
		++ *num_in_queue;
		if(*num_in_queue > Q_limit)
		{
			printf("\nOverflow of the array time_arrival at");
			printf(" time %f",*sim_time);
			exit(2); // double check
		}
		*time_arrival[*num_in_queue] = *sim_time;
	}
	else
	{

		++*num_custs_delayed;
		*server_status = BUSY;

		*next_dept_time = *sim_time + expon(*mean_service_time);
	}
}

void depart(int* server_status, int* num_in_queue, int* num_custs_delayed, float* sim_time, float* area_under_Q,
	float* last_event_time, float* time_since_last_event, float* time_arrival[Q_limit],
	float* mean_service_time, float* next_dept_time, float* q_delay, float* total_q_delay)  // change to call by regerence

{
	*sim_time = *next_dept_time;

	*time_since_last_event = *sim_time - *last_event_time;
	*last_event_time = *sim_time;
	*area_under_Q += *num_in_queue * *time_since_last_event;


	//Check to whether the queue is empty

	if (*num_in_queue == 0)
	{
		*server_status = IDLE;
		*next_dept_time = pow(10, 30);
	}

	else
	{
		--num_in_queue;
		*q_delay = *sim_time - *time_arrival[1];

		*total_q_delay += *q_delay;

		++num_custs_delayed;

		*next_dept_time = *sim_time + expon(*mean_service_time);

		//Move each customer in queue (if any) up one place
		for (int i = 1; i <= *num_in_queue; ++i)

			*time_arrival[i] = *time_arrival[i + 1];
	}
}

float expon(float mean){
    return -mean*log(lcgrand(1));

}
