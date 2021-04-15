#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "lcgrand.h"

#define IDLE 0
#define BUSY 1
#define Q_limit 100000

void arrive(int* server_status, int* num_in_queue, int* num_custs_delayed,
	 double* sim_time,  double* area_under_Q, float* last_event_time, float* time_since_last_event,
	 double* time_arrival, float* mean_interarrival_time, float* mean_service_time,
	float* next_arr_time, float *next_dept_time);

void depart(int* server_status, int* num_in_queue, int* num_custs_delayed,  double* sim_time,  double* area_under_Q,
	float* last_event_time, float* time_since_last_event, double* time_arrival,
	float* mean_service_time, float* next_dept_time,  double* q_delay,  double* total_q_delay);

float expon(float mean);


int main()
{
        // initialize
    int server_status = IDLE;
    int num_in_queue = 0;       //number of people currently in the queue
    int num_custs_delayed = 0;  // number of customers that has been served
    int num_delays_required = 0; // Maximum number of customers that is going to be served

     double sim_time = 0.0;       // current simulation time
     double total_q_delay = 0.0;
     double q_delay = 0.0;        // the time difference between a customer's arrival time and his depature time
    float avg_delay = 0.0;
    float avg_num_in_queue = 0.0;
     double area_under_Q = 0.0;
    float last_event_time = 0.0;
    float time_since_last_event = 0.0;
     double time_arrival[Q_limit]={0};   // array that stores each customer's arrival time
    float next_dept_time = pow(10,30);
    float next_arr_time = 0.0;

	float mean_interarrival_time = 0.0; //initialize inputs
    float mean_service_time = 0.0;      // end of initialization



	printf("Enter mean_interarrival_time\n");
	scanf("%f", &mean_interarrival_time);
    next_arr_time=expon(mean_interarrival_time);


	printf("Enter mean_service_time\n");
	scanf("%f", &mean_service_time);

	printf("Enter num_delays_required\n");
	scanf ("%d", &num_delays_required);



    while(num_custs_delayed<num_delays_required){

        if(next_arr_time<next_dept_time){

            arrive(&server_status,&num_in_queue,&num_custs_delayed,&sim_time,&area_under_Q,&last_event_time,&time_since_last_event,
                    time_arrival,&mean_interarrival_time,&mean_service_time,&next_arr_time,&next_dept_time);
        }
        else{
			depart(&server_status, &num_in_queue, &num_custs_delayed, &sim_time, &area_under_Q,
				&last_event_time, &time_since_last_event, time_arrival, &mean_service_time, &next_dept_time, &q_delay, &total_q_delay);
        }

    }


    printf("\n");
    printf("sim_time= %f \n",sim_time);        // report
//    printf("server_status= %d \n",server_status);
    printf("num_in_queue= %d \n",num_in_queue);
    printf("num_custs_delayed= %d \n",num_custs_delayed);
    printf("num_delays_required= %d \n",num_delays_required);
    printf("total_delay= %.4f \n",total_q_delay);

    avg_delay = total_q_delay/num_custs_delayed;
    printf("avg_delay= %.5f \n",avg_delay);

    avg_num_in_queue = area_under_Q/sim_time;
    printf("avg_num_in_queue= %.5f \n",avg_num_in_queue);

//printf("area_under_Q= %.4f \n",area_under_Q);
//printf("last_event_time= %.4f \n",last_event_time);
// printf("time_since_last_event= %.4f \n",time_since_last_event);

    return 0;

}

void arrive(int* server_status,int* num_in_queue,int* num_custs_delayed,    // Arrival event
 double* sim_time,  double* area_under_Q,float* last_event_time,float* time_since_last_event,
 double* time_arrival,float* mean_interarrival_time,float* mean_service_time,
float* next_arr_time,float *next_dept_time) {


	*sim_time = *next_arr_time;
	*next_arr_time = *sim_time + expon(*mean_interarrival_time);

	*time_since_last_event = *sim_time - *last_event_time;
	*last_event_time = *sim_time;
	*area_under_Q += (*num_in_queue) * (*time_since_last_event);


	if(*server_status == BUSY)
	{
        (*num_in_queue)++;

		if(*num_in_queue > Q_limit)
		{
			printf("\nOverflow of the array time_arrival at");
			printf(" time %f",*sim_time);
			exit(2); // double check
		}

		time_arrival[*num_in_queue] = *sim_time;  // store the new customer's arrival time
	}

	else
	{

		(*num_custs_delayed)++;
		*server_status = BUSY;
		*next_dept_time = *sim_time + expon(*mean_service_time);
	}
}

void depart(int* server_status, int* num_in_queue, int* num_custs_delayed,  double* sim_time,  double* area_under_Q,
	float* last_event_time, float* time_since_last_event,  double* time_arrival,
	float* mean_service_time, float* next_dept_time,  double* q_delay,  double* total_q_delay) {


	*sim_time = *next_dept_time;

	*time_since_last_event = *sim_time - *last_event_time;
	*last_event_time = *sim_time;
	*area_under_Q += (*num_in_queue) * (*time_since_last_event);


	//Check to whether the queue is empty

	if (*num_in_queue == 0)
	{
		*server_status = IDLE;
		*next_dept_time = pow(10,30);
	}

	else
	{
		(*num_in_queue)-- ;
		*q_delay = *sim_time - time_arrival[1];

		*total_q_delay += *q_delay;

		(*num_custs_delayed)++;

		*next_dept_time = *sim_time + expon(*mean_service_time);

		//Move each customer in queue (if any) up one place
		for (int i = 1; i <= *num_in_queue; ++i)

			time_arrival[i] = time_arrival[i + 1];
	}
}

float expon(float mean){
    return -mean*log(lcgrand(1));

}
