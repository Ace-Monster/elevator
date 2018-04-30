#include "elevator.h"

using namespace myelevator;

void Ask::setting(int w_request_time, int w_wait_floor, int w_target_floor){
		request_time = w_request_time;
		wait_floor = w_wait_floor;
		target_floor = w_target_floor;
	}

int Ask::r_request_time(){
	return request_time;
}

int Ask::r_target_floor(){
	return target_floor;
}

int Ask::r_wait_floor(){
	return wait_floor;
}

Ask::Status Ask::r_status(){
	return status;
}

void Ask::w_status(Status t_status){
	status = t_status;
}