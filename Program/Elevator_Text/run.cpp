#include "elevator.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

using namespace myelevator;

int Run::time_compute(Elevator *t_elevator, Ask t_people_ask[], int wait_people[], bool wait_floor[], int pre_floor, int ask_people_sum)//下一层判断前临时改变电梯参数
{
	int t = t_elevator->floor_arrive(pre_floor, t_elevator->floor_difference(pre_floor)) * t_elevator->size();
	wait_floor[t_elevator->floor()] = 0;
	for (int k = 0;k < ask_people_sum;k++)
	{
		if (t_people_ask[k].r_wait_floor() == pre_floor && t_people_ask[k].r_status() == Ask::WAIT)
			t_people_ask[k].w_status(Ask::RUNING), t_elevator->people_up(), wait_floor[t_people_ask[k].r_target_floor()] = true;
		else if (t_people_ask[k].r_target_floor() == pre_floor && t_people_ask[k].r_status() == Ask::RUNING)
			t_people_ask[k].w_status(Ask::COMPLETED), t_elevator->people_down();
	}
	wait_people[pre_floor] = 0;
	t_elevator->times_update();
	t += t_elevator->size();
	return t;
}

int Run::dfs(Elevator t_elevator, Ask tAsk[], int wait_people_i[], bool wait_floor_i[], int j, int ask_people_sum)//ycz调用的，第二及之后的判断，返回电梯运行时间
{
	bool wait_floor[11];
	memcpy(wait_floor, wait_floor_i, sizeof(wait_floor));
	Ask *t_people_ask = new Ask[ask_people_sum];int wait_people[11];
	memcpy(t_people_ask, tAsk, sizeof(t_people_ask));
	memcpy(wait_people, wait_people_i, sizeof(wait_people));
	int t_numi = time_compute(&t_elevator, t_people_ask, wait_people, wait_floor, j, ask_people_sum);
	int tt_num = 1000000;
	for (int i = 1;i <= 3;i++)
	{
		if (!wait_floor[i])
			continue;
		int t_num = t_numi;
		t_num += dfs(t_elevator, t_people_ask, wait_people, wait_floor, i, ask_people_sum);//递归
		if (tt_num > t_num)
			tt_num = t_num;
	}
	if (tt_num == 1000000)
		return t_numi;
	delete[] t_people_ask;
	return tt_num;//返回估计时间
}

Elevator Run::elevator_update(Elevator t_elevator, Ask t_people_ask[], int wait_people[], bool wait_floor[], int ask_people_sum)//第一层判断，直接决定电梯的下一步运行目标
{
	int tt_num = 1000;
	int tempflt = 0;
	for (int i = 1;i <= 3;i++)
	{
		if (!wait_floor[i])
			continue;
		int t_num = dfs(t_elevator, t_people_ask, wait_people, wait_floor, i, ask_people_sum);
		if (tt_num > t_num)
		{
			tempflt = i;
			tt_num = t_num;
			t_elevator.w_target_floor(i);
		}
	}
	return t_elevator;//返回电梯参数
}

void Run::stop_massage(Elevator &elevator, Ask people_ask[], int wait_people[], bool wait_floor[], int ask_people_sum)//停下后的状态调整
{

	elevator.num_time_update(elevator.floor_arrive(elevator.target_floor(), elevator.floor_difference(elevator.target_floor())));
	wait_floor[elevator.floor()] = 0;
	int temp_peo = elevator.size(), temp_peo2 = elevator.people_num(People::WAIT);
	elevator.people_up(wait_people[elevator.floor()]);
	wait_people[elevator.floor()] = 0;
	for (int k = 0;k < ask_people_sum;k++)
	{
		if (people_ask[k].r_wait_floor() == elevator.floor() && people_ask[k].r_status() == Ask::WAIT)
			people_ask[k].w_status(Ask::RUNING), wait_floor[people_ask[k].r_target_floor()] = true;
		else if (people_ask[k].r_target_floor() == elevator.floor() && people_ask[k].r_status() == Ask::RUNING)
			people_ask[k].w_status(Ask::COMPLETED), elevator.people_down();
	}
	/*
	printf("第%d秒，电梯在%d停靠，%d人下电梯，%d人上电梯，电梯内%d人\ask_people_sum",
	elevator.system_time, elevator.floor, temp_peo - elevator.size(), temp_peo2 - elevator.people[2], elevator.size() - elevator.people[2]);
	*///注释部分用于调试，输出每次停下时的人员变化
	elevator.times_update();
	elevator.num_time_update();
	std::cout << elevator.system_time() << ' ' << elevator.floor() << std::endl;
	elevator = elevator_update(elevator, people_ask, wait_people, wait_floor, ask_people_sum);
}