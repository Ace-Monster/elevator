#include "elevator.h"
#include <algorithm>
#include <vector>
#include<iostream>

using namespace myelevator;

void Elevator::outmessage_set(const std::string out_message) {
	Out_Message = out_message;
}

const std::string Elevator::OutMessage() {
	return Out_Message;
}

const bool Elevator::OutPutJudge() {
	if (flag)
	{
		flag = false;
		return true;
	}
	return flag;
}

const int Elevator::run_times(){
    return times.run_time;
}

const int Elevator::system_time(){
    return times.system_time;
}

const int Elevator::target_time(){
	return times.target_time;
}

void Elevator::system_time_update(int uptime){
    times.system_time = uptime;
}

void Elevator::times_update(int uptime){
    times.system_time += uptime;
    times.run_time += uptime;
	times.target_time -= uptime;
}

const int Elevator::floor(){
    return floors.floor;
}

const int Elevator::target_floor(){
    return floors.target_floor;
}

const bool Elevator::direction(){
    if(floors.target_floor - floors.floor > 0)
        return Floor::Direction::UP;
    else
        return Floor::Direction::DOWN;
}

void Elevator::target_floor_update(int _target_floor){
    floors.target_floor = _target_floor;
}

int Elevator::floor_difference(int _floor){
    if(_floor == -1)
        _floor = floors.target_floor;
    return abs(floors.floor - _floor);
}

int Elevator::floor_arrive(int _floor, int uptime){
    if(_floor == -1)
    {
        _floor = floors.target_floor;
        uptime = times.target_time;
    }
    floors.floor = _floor;
	times_update(uptime);
	bool flag1 = people_down(_floor, times.system_time);
	bool flag2 = people_up(_floor);
	bool flag = (flag1 || flag2);
	return flag;//·µ»ØÊÇ·ñÍ£¿¿
}

const int Elevator::size(){
	return people.people.size();
}

const std::vector<Ask> Elevator::message_get() {
	std::vector<Ask> message;
	for (auto at = people.people.begin(); at != people.people.end(); at++)
		message.push_back(**at);
	return message;
}

void Elevator::people_distribute(Ask* t_people){
	t_people->w_status(Ask::DISTRIBUTED);
    people.people.push_back(t_people);
}

bool Elevator::people_up(int floor) {
	int Flag = false;
	for (auto at = people.people.begin();at != people.people.end();at++)
	{
		if ((*at)->r_wait_floor() == floor && (*at)->r_status() == Ask::DISTRIBUTED)
		{
			//std::cout << "UP" << std::endl;
			(*at)->w_status(Ask::RUNING);
			Flag = true;
		}
	}
	return Flag;
}

bool Elevator::people_down(int floor, int system_time) {
	int Flag = false;
    for(auto at = people.people.begin();at != people.people.end();at++)
    {
        bool flag = true;
        while(flag && at != people.people.end())
        {
            flag = false;
            if((*at)->r_target_floor() == floor && (*at)->r_status() == Ask::RUNING)
            {
				//std::cout << "OUT"<< (*at)->r_request_time() << " " <<(*at)->r_target_floor()<< " " << (*at)->r_status() << std::endl;
				(*at)->completed(system_time);
                at = people.people.erase(at);
                flag = true;
				Flag = true;
            }
        }
		if (at == people.people.end())
			break;
    }
	return Flag;
}

void Elevator::target_update(int _floor){
    int floor_difference = abs(_floor - floors.floor);
	if (floors.target_floor == floors.floor && !people.people.empty())
		floor_difference++;
	floors.target_floor = _floor;
	times.target_time = floor_difference;
}

bool Elevator::run(int wait_floor, int to_floor){
    return true;
}

bool Odd_Elevator::run(int wait_floor, int to_floor){
    if(wait_floor % 2 && to_floor % 2)
        return true;
    else
        return false;
}

bool Even_Elevator::run(int wait_floor, int to_floor){
    if((!(wait_floor % 2) || wait_floor == 1) && (!(to_floor % 2) || to_floor == 1))
        return true;
    
    else
        return false;
    
}

void Elevator::stop() {
	times.target_time = 1e9;
}