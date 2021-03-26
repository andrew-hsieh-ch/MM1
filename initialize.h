#include <stdio.h>
#include <math.h>


#define IDLE 0
#define BUSY 1
#define Q_limit 10000

int i;
int server_status=IDLE;
int num_in_queue=0;
int num_custs_delayed=0;
int num_delays_required=10000;
float sim_time=0.0;
float total_delay=0.0;
float avg_delay=0.0;
float avg_num_in_queue=0.0;
float area_under_Q=0.0;
float last_event_time=0.0;
float time_since_last_event=0.0;
float time_arrival[Q_limit];
float mean_interarrival_time = 0;
float next_arr_time = 0;
float next_dept_time = 10^30;
float mean_service_time = 0;
float Q_delay = 0;
float total_q_delay = 0;
float q_delay = 0;


// whats up
