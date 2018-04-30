#include "elevator.h"
#include <algorithm>

using namespace myelevator;

int Elevator::run_times(){
    return times.run_time;
}

int Elevator::num_times(){
    return times.num_time;
}

int Elevator::system_time(){
    return times.system_time;
}

int Elevator::target_time(){
    return floor_difference(floors.target_floor);
}

void Elevator::num_time_update(int uptime){
    times.num_time += size() * uptime;
}

void Elevator::system_time_update(int uptime){
    times.system_time = uptime;
}

int Elevator::times_update(int uptime){
    times.system_time += uptime;
    times.run_time += uptime;
    return uptime;
}

int Elevator::floor(){
    return floors.floor;
}

int Elevator::target_floor(){
    return floors.target_floor;
}

void Elevator::w_target_floor(int _target_floor){
    floors.target_floor = _target_floor;
}

int Elevator::floor_difference(int _floor){
    return abs(floors.floor - _floor);
}

int Elevator::floor_arrive(int _floor, int uptime){
    floors.floor = _floor;
    return times_update(uptime);
}

int Elevator::people_num(People::Status status){
    return people.people[status];
}

int Elevator::size(){
	return people.people[0] + people.people[1];
}

void Elevator::people_up(int people_sum){
    people.people[0] += people_sum;
	people.people[1] -= people_sum;
}

void Elevator::people_down(int people_num){
    people.people[People::INSIDE] -= people_num;
}

void Elevator::ask_update(int request_time){
    int time_difference = request_time - times.system_time;
    num_time_update(time_difference);
    times_update(time_difference);
    people.people[People::WAIT]++;
    floors.floor += (floors.floor > floors.target_floor) ? -time_difference : time_difference;
}