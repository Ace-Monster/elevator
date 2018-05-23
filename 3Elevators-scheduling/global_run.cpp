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
namespace global{
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
        if (elevator->floor() == elevator->target_floor())
            elevator->stop();
    }

    void stop_message(Elevator *elevator)
    {
        ofstream Films;
        Films.open(elevator->OutMessage(), ios::app);
        Films << elevator->system_time() << ' ' << elevator->floor() << endl;
        Films.close();
        return;
    }

    void Stoped(Elevator *elevator, vector<Ask>* people, const int floor_num)
    {
        bool flag = elevator->floor_arrive();
        for (auto at = people->begin(); at != people->end(); at++)
        {
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
            stop_message(elevator);
        elevator_update(elevator, *people, floor_num);
    }

    void Runing(Elevator *elevator, int run_time, int system_time,vector<Ask>* people, const int floor_num)
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
            Stoped(elevator, people, floor_num);
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
                    stop_message(elevator);
                }
                elevator_update(elevator, *people, floor_num);
            }
        }
    }

    int ask_wait_size(vector<Ask> ask) {
        int wait_peo_sum = 0;
        for (auto at = ask.begin(); at != ask.end(); at++)
            if (at->r_status() == Ask::WAIT)
                wait_peo_sum++;
        return wait_peo_sum;
    }
}

using namespace global;
int Run::global_run_main(string input, string output1, string output2, string output3)
{
	cout << "程序正在运行\n本程序采用dfs算法，效率不高，请耐心等待\n枚举大法好，暴力出奇迹\n" << endl;
	ifstream Input;
	ofstream Clean;

	Input.open("input.txt");
	int ask_people_sum, floor = 10;//请求人数,楼层数量
	Input >> ask_people_sum;

	Elevator elevator(floor);
	elevator.outmessage_set(output2);
	Odd_Elevator odd_elevator(floor);
	odd_elevator.outmessage_set(output1);
	Even_Elevator even_elevator(floor);
	even_elevator.outmessage_set(output3);
	Clean.open(elevator.OutMessage());Clean.close();
	Clean.open(odd_elevator.OutMessage());Clean.close();
	Clean.open(even_elevator.OutMessage());Clean.close();

	vector<Ask> people_ask;
	for (int i = 0; i < ask_people_sum; i++)
	{
		int request_time, wait_floor, target_floor;
		Input >> request_time >> wait_floor >> target_floor;
		Ask temp;
		temp.setting(request_time, wait_floor, target_floor);
		people_ask.push_back(temp);
	}
	//sort(q, q + 5);//按时间排序

	int system_time = people_ask[0].r_request_time();//设置系统时间
	for (int i = 0;i < ask_people_sum;i++)//未收全请求前，没收到一次请求就判断一次
	{
		int min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
		while (system_time + min_time < people_ask[i].r_request_time())
		{
			system_time += min_time;
			Runing(&elevator, min_time, system_time, &people_ask, floor);
			Runing(&odd_elevator, min_time, system_time, &people_ask, floor);
			Runing(&even_elevator, min_time, system_time, &people_ask, floor);
		}
		min_time = people_ask[i].r_request_time() - system_time;
		system_time = people_ask[i].r_request_time();
		int ask_time = people_ask[i].r_request_time();
		while (people_ask[i].r_request_time() == ask_time)
		{
			people_ask[i++].w_status(Ask::WAIT);
			if (i >= ask_people_sum)
				break;
		}
		i--;
		Runing(&elevator, min_time, system_time, &people_ask, floor);
		Runing(&odd_elevator, min_time, system_time, &people_ask, floor);
		Runing(&even_elevator, min_time, system_time, &people_ask, floor);
	}
	while (elevator.size() + odd_elevator.size() + even_elevator.size() + ask_wait_size(people_ask) != 0)//直到所有人全部完成请求
	{
		int min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
		Runing(&elevator, min_time, system_time, &people_ask, floor);
		Runing(&odd_elevator, min_time, system_time, &people_ask, floor);
		Runing(&even_elevator, min_time, system_time, &people_ask, floor);
	}
	cout << "程序运行结束" << endl;
	return 0;
}