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

struct UpSet
{
	int sum = inf;
	int floor;
};

int dfs(Elevator *elevator, vector<Ask> message, bool wait_floor[], const int now_floor, const int floor_num)
{
	int sum = inf;
	bool *t_wait_floor = new bool[floor_num];
	memcpy(t_wait_floor, wait_floor, sizeof(bool)*floor_num);
	t_wait_floor[now_floor] = false;
	for (auto at = message.begin(); at != message.end();at++)
	{
		if (at->r_status() == Ask::RUNING)
			at->w_status(Ask::COMPLETED);
		if (at->r_status() == Ask::DISTRIBUTED)
		{
			at->w_status(Ask::RUNING);
			t_wait_floor[at->r_target_floor() - 1] = true;
		}
	}

	for (int i = 0;i < floor_num;i++)
	{
		if (!t_wait_floor[i])
			continue;
		int temp = abs(now_floor - i) + 1;
		temp += dfs(elevator, message, t_wait_floor, i, floor_num);
		if (sum > temp)
			sum = temp;
	}
	if (sum == inf)
		sum = 0;
	delete[] t_wait_floor;
	return sum;
}

UpSet dfs_one(Elevator *elevator, Ask* people, vector<Ask> message, const int floor_num)
{ 
	UpSet up_message;
	if (elevator->run(people->r_wait_floor(), people->r_target_floor()) == false)
		return up_message;
	int sum = inf;
	bool *wait_floor = new bool[floor_num];

	for (int i = 0;i < floor_num;i++)
		wait_floor[i] = 0;
	
	Ask t_people = *people;
	t_people.w_status(Ask::DISTRIBUTED);
	message.push_back(t_people);
	for (auto at = message.begin(); at != message.end(); at++)
	{
		if(at->r_status() == Ask::RUNING)
			wait_floor[at->r_target_floor() - 1] = true;
		if (at->r_status() == Ask::DISTRIBUTED)
			wait_floor[at->r_wait_floor() - 1] = true;
	}
	
	for (int i = 0;i < floor_num;i++)
	{
		if (!wait_floor[i])
			continue;

		int temp = abs(elevator->floor() - i) + 1;
		temp += dfs(elevator, message, wait_floor, i, floor_num);
		
		if (sum > temp)
		{
			sum = temp;
			
			up_message.sum = sum;
			up_message.floor = i + 1;
		}
	}

	delete[] wait_floor;
	return up_message;
}

void elevator_update(Elevator *elevator, const int floor_num)
{
	int sum = inf;
	bool *wait_floor = new bool[floor_num];

	for (int i = 0;i < floor_num;i++)
		wait_floor[i] = 0;

	vector<Ask> message = elevator->message_get();
	for (auto at = message.begin(); at != message.end(); at++)
	{
		if (at->r_status() == Ask::RUNING)
			wait_floor[at->r_target_floor() - 1] = true;
		if (at->r_status() == Ask::DISTRIBUTED)
			wait_floor[at->r_wait_floor() - 1] = true;
	}

	for (int i = 0;i < floor_num;i++)
	{
		if (!wait_floor[i])
			continue;

		int temp = abs(elevator->floor() - i) + 1;
		temp += dfs(elevator, message, wait_floor, i, floor_num);

		if (sum > temp)
		{
			sum = temp;
			elevator->target_update(i + 1);
		}
	}
	if (sum == inf)
		elevator->stop();
	delete[] wait_floor;
	return;
}

void stop_message(Elevator *elevator)
{
	ofstream Films;
	Films.open(elevator->OutMessage(), ios::app);
	if (elevator->OutPutJudge() && elevator->floor() == 1)
	{
		Films.close();
		return;
	}
	Films << elevator->system_time() << ' ' << elevator->floor() << endl;
	//cout << elevator->system_time() << ' ' << elevator->floor() << endl;
	Films.close();
	return;
}

void Stoped(Elevator *elevator, const int floor_num)
{
	bool flag = elevator->floor_arrive();
	if(flag)
		stop_message(elevator);
	elevator_update(elevator, floor_num);
}

void Run::Runing(Elevator *elevator, const int floor_num, int run_time, int system_time, bool update_flag)
{
	if (elevator->target_time() == run_time)
		Stoped(elevator, floor_num);
	else
	{
		int t_floor = 0;
		bool flag = (elevator->floor_difference() == (elevator->target_time() - 1));
		if (flag)
			t_floor += (elevator->direction() ? (-1) : (1));
		t_floor += (elevator->direction() ? (elevator->floor() + run_time) : (elevator->target_floor() + run_time));
		if (t_floor <= 0 || t_floor > 10)
			return;
		if (elevator->target_time() != inf)
		{
			elevator->floor_arrive(t_floor, run_time);
			elevator->OutPutJudge();
		}
		if(update_flag)
			elevator_update(elevator, floor_num);
	}
}

void Run::Distrbute(Elevator* elevator, Elevator* odd_elevator, Elevator* even_elevator, std::vector<Ask>* people, const int floor_num, int run_time, int system_time)
{
	elevator->system_time_update(system_time);
	odd_elevator->system_time_update(system_time);
	even_elevator->system_time_update(system_time);
	for (auto at = people->begin();at != people->end();at++)
	{
		if (at->r_status() == Ask::WAIT)
		{
			UpSet nomal, odd, even;
			if (elevator->run(at->r_wait_floor(), at->r_target_floor()))
				nomal = dfs_one(elevator, &*at, elevator->message_get(), floor_num);
			if (odd_elevator->run(at->r_wait_floor(), at->r_target_floor()))
				odd = dfs_one(odd_elevator, &*at, elevator->message_get(), floor_num);
			if (even_elevator->run(at->r_wait_floor(), at->r_target_floor()))
				even = dfs_one(even_elevator, &*at, elevator->message_get(), floor_num);
			if (nomal.sum <= min(odd.sum, even.sum))
			{
				elevator->people_distribute(&*at);
				Runing(elevator, floor_num, run_time, system_time);
			}
			else if (odd.sum < even.sum)
			{
				odd_elevator->people_distribute(&*at);
				Runing(odd_elevator, floor_num, run_time, system_time);
			}
			else
			{
				even_elevator->people_distribute(&*at);
				Runing(even_elevator, floor_num, run_time, system_time);
			}
		}
	}//µ÷¶È·ÖÅä
}

int Run::ask_wait_size(vector<Ask> ask) {
	int wait_peo_sum = 0;
	for (auto at = ask.begin(); at != ask.end(); at++)
		if (at->r_status() == Ask::WAIT)
			wait_peo_sum++;
	return wait_peo_sum;
}