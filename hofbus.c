struct station {
 	//write your code here
 	int available_seats; // Number of available seats
	int waiting_students; // Number of students waiting at the station
	int next_ticket;
	int next_student;
	pthread_mutex_t lock;
	pthread_cond_t bus_arrive_cond; //Condition variiable to wait for the bus to arrive
	pthread_cond_t bus_loaded_cond;

};

void
station_init(struct station *station)
{
	//write your code here
	pthread_mutex_init(&station->lock,NULL);
	pthread_cond_init(&station->bus_arrive_cond,NULL);
	pthread_cond_init(&station->bus_loaded_cond,NULL);
	station->waiting_students = 0;
        station->available_seats = 0;
        station->next_ticket = 1;
	station->next_student = 1;


}

void
station_load_bus(struct station *station, int count)
{
 	 //write your code here
	pthread_mutex_lock(&station->lock);

	station->available_seats = count;

	while(station->waiting_students>0 && station->available_seats > 0){
	pthread_cond_signal(&station->bus_arrive_cond);
	pthread_cond_wait(&station->bus_loaded_cond, &station->lock);
	}
	station->available_seats=0;
	pthread_mutex_unlock(&station->lock);
}


int
station_wait_for_bus(struct station *station, int myticket, int myid)
{

	pthread_mutex_lock(&station->lock);
	
	station->waiting_students++;

	while(station->next_ticket != myticket || station->available_seats==0)
	{
	pthread_cond_wait(&station->bus_arrive_cond, &station->lock);
	}

	station->waiting_students--;
	station->available_seats--;

	int my_board_order = station->next_student;
	
	station->next_student++;
	station->next_ticket++;
	
	pthread_cond_signal(&station->bus_loaded_cond);

	pthread_mutex_unlock(&station->lock);

	return my_board_order;

}
