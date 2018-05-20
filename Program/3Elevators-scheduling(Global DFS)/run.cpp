#include "elevator.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>


using namespace std;
using namespace myelevator;

int dfs(Elevator *elevator, vector<Ask> people, vector<Ask> message, bool wait_floor[], const int now_floor, const int floor_num, const int system_time)
{
	int sum_time = inf, basic_time = 0;
	bool *t_wait_floor = new bool[floor_num];
	memcpy(t_wait_floor, wait_floor, sizeof(bool)*floor_num);
	t_wait_floor[now_floor] = false;
	for (auto at = people.begin(); at != people.end(); at++)
	{
		if (at->r_wait_floor() == now_floor + 1 && at->r_status() == Ask::WAIT && elevator->run(now_floor+1, at->r_target_floor()))
		{
			at->w_status(Ask::RUNING);
			t_wait_floor[at->r_target_floor() - 1] = true;
			message.push_back(*at);
		}
	}
	for (auto at = message.begin(); at != message.end(); at++)
	{
		if (at->r_target_floor() == now_floor + 1 && at->r_status() == Ask::RUNING)
		{
			at->w_status(Ask::COMPLETED);
			basic_time += system_time - at->r_request_time();
		}
	}
	for (int i = 0;i < floor_num;i++)
	{
		if (!t_wait_floor[i])
			continue;
		int t_sum_time = dfs(elevator, people, message, t_wait_floor, i, floor_num, system_time + abs(now_floor - (i + 1)));//+1
		if (sum_time > t_sum_time)
			sum_time = t_sum_time;
	}
	if (sum_time == inf) sum_time = 0;
	delete[] t_wait_floor;
	return sum_time + basic_time;
}

void elevator_update(Elevator *elevator, vector<Ask> people, const int floor_num)
{
	//Elevator *t_elevator = new Elevator(*elevator);
	int sum_time = inf, now_floor = elevator->floor();
	bool *wait_floor = new bool[floor_num];
	memset(wait_floor, 0, sizeof(bool)*floor_num);

	vector<Ask> message = elevator->message_get();
	for (auto at = message.begin(); at != message.end(); at++)
		wait_floor[at->r_target_floor() - 1] = true;
	
	for (auto at = people.begin(); at != people.end(); at++)
	{
		if (elevator->run(at->r_wait_floor(), at->r_target_floor()) && at->r_status() == Ask::WAIT)
		{
			wait_floor[at->r_wait_floor() - 1] = true;
		}
	}
	
	for (int i = 0;i < floor_num;i++)
	{
		if (!wait_floor[i])
			continue;
		int t_sum_time = dfs(elevator, people, message, wait_floor, i, floor_num, elevator->system_time() + abs(now_floor - (i + 1)));//+1
		if (t_sum_time == 0)
			continue;
		if (sum_time > t_sum_time)
		{
			sum_time = t_sum_time;
			elevator->target_update(i + 1);
		}
	}
	delete[] wait_floor;
	//delete t_elevator;
	if (elevator->floor() == elevator->target_floor())
		elevator->stop();
}

void stop_message(Elevator *elevator, const string Out_Message)
{
	ofstream Films;
	Films.open(Out_Message, ios::app);
	Films << elevator->system_time() << ' ' << elevator->floor() << endl;
	cout << elevator->system_time() << ' ' << elevator->floor() << endl;
	//system("pause");
	Films.close();
	return;
}

void Stoped(Elevator *elevator, const string Out_Message, vector<Ask>* people, const int floor_num)
{
	bool flag = elevator->floor_arrive();
	for (auto at = people->begin(); at != people->end(); at++)
	{
		//if (at->r_status() == Ask::COMPLETED)
		//	at = people->erase(at);
		if (at == people->end())
			break;
		if (at->r_wait_floor() == elevator->floor() && at->r_status() == Ask::WAIT && elevator->run(elevator->floor(), at->r_target_floor()))
		{
			at->w_status(Ask::RUNING);
			elevator->people_up(&*at);
			flag = true;
		}
	}
	if(flag)
		stop_message(elevator, Out_Message);
	elevator_update(elevator, *people, floor_num);
}

void Run::Runing(Elevator *elevator, int run_time, int system_time,vector<Ask>* people, const string Out_Message, const int floor_num)
{
	if (elevator->target_time() == inf)
	{
		elevator->system_time_update(system_time);
		for (auto at = people->begin(); at != people->end(); at++)
		{
			if (at->r_wait_floor() == elevator->floor() && at->r_status() == Ask::WAIT && elevator->run(elevator->floor(), at->r_target_floor()))
			{
				at->w_status(Ask::RUNING);
				elevator->people_up(&*at);
			}
		}
		elevator_update(elevator, *people, floor_num);
		return;
	}
	if (elevator->target_time() == run_time)
		Stoped(elevator, Out_Message, people, floor_num);
	else
	{
		int t_floor = 0;
		bool flag = (elevator->floor_difference() == (elevator->target_time() - 1));
		if (flag)
		{
			t_floor += (elevator->direction() ? (-1) : (1));
			//elevator->times_update(1);
		}
		t_floor += (elevator->direction() ? (elevator->floor() + run_time) : (elevator->target_floor() + run_time));
		elevator->floor_arrive(t_floor, run_time);
		if (flag && run_time == 1)
		{
			flag = false;
			for (auto at = people->begin(); at != people->end(); at++)
			{
				if (at->r_wait_floor() == elevator->floor() && at->r_status() == Ask::WAIT && elevator->run(elevator->floor(), at->r_target_floor()))
				{
					flag = true;
					at->w_status(Ask::RUNING);
					elevator->people_up(&*at);
				}
			}
			if (flag)
			{
				elevator->target_floor_update(elevator->floor());
				stop_message(elevator, Out_Message);
			}
			elevator_update(elevator, *people, floor_num);
		}
	}
}

int Run::ask_wait_size(vector<Ask> ask) {
	int wait_peo_sum = 0;
	for (auto at = ask.begin(); at != ask.end(); at++)
		if (at->r_status() == Ask::WAIT)
			wait_peo_sum++;
	return wait_peo_sum;
}