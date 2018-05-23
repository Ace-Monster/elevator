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
namespace distribution{
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

    void Runing(Elevator *elevator, const int floor_num, int run_time, int system_time, bool update_flag = true)
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

    void Distrbute(Elevator* elevator, Elevator* odd_elevator, Elevator* even_elevator, std::vector<Ask>* people, const int floor_num, int run_time, int system_time)
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
        }//���ȷ���
    }

    int ask_wait_size(vector<Ask> ask) {
        int wait_peo_sum = 0;
        for (auto at = ask.begin(); at != ask.end(); at++)
            if (at->r_status() == Ask::WAIT)
                wait_peo_sum++;
        return wait_peo_sum;
    }
}

using namespace distribution;
int Run::distribution_run_main(string input, string output1, string output2, string output3)
{
	cout << "程序正在运行\n本程序采用dfs算法，效率不高，请耐心等待\n枚举大法好，暴力出奇迹\n" << endl;
	ifstream Input;
	ofstream Clean;
	
	Input.open("input.txt");
	int ask_people_sum, floor = 10;//请求人数,楼层数量
	Input >> ask_people_sum;

	Elevator elevator(floor);
	elevator.outmessage_set(output1);
	Odd_Elevator odd_elevator(floor);
	odd_elevator.outmessage_set(output2);
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
			Runing(&elevator, floor, min_time, system_time);
			Runing(&odd_elevator, floor, min_time, system_time);
			Runing(&even_elevator, floor, min_time, system_time);
			min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
			if (min_time == inf)
				system_time = people_ask[i].r_request_time();
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
		Distrbute(&elevator, &odd_elevator, &even_elevator, &people_ask, floor, min_time, system_time);
	}
	while (elevator.size() + odd_elevator.size() + even_elevator.size() + ask_wait_size(people_ask) != 0)//直到所有人全部完成请求
	{
		int min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
		Runing(&elevator, floor, min_time, system_time);
		Runing(&odd_elevator, floor, min_time, system_time);
		Runing(&even_elevator, floor, min_time, system_time);
	}
	cout << "程序运行结束" << endl;
    return 0;
}