#include <iostream>
#include "elevator.h"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
#include <string>
#include <fstream>
//#include "run.cpp"
using namespace std;
using namespace myelevator;
int main()
{
	string nomal_out = "output2.txt";string odd_out = "output1.txt";string even_out = "output3.txt";
	ifstream Input;
	ofstream Clean;
	Clean.open(nomal_out);
	Clean.close();
	Clean.open(odd_out);
	Clean.close();
	Clean.open(even_out);
	Clean.close();
	Input.open("input.txt");
	int ask_people_sum, floor = 10;//请求人数,楼层数量
	Input >> ask_people_sum;

	Elevator elevator(floor);
	Odd_Elevator odd_elevator(floor);
	Even_Elevator even_elevator(floor);

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
	/*for (auto at = people_ask.begin();at != people_ask.end();at++)
	{
		if (at->r_wait_floor() == 1 && at->r_request_time() == system_time)
		{
			at->w_status(Ask::RUNING);
			elevator.people_up(&*at);
		}
	}*/
	for (int i = 0;i < ask_people_sum;i++)//未收全请求前，没收到一次请求就判断一次
	{
		int min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
		while (system_time + min_time < people_ask[i].r_request_time())
		{
			system_time += min_time;
			Run::Runing(&elevator, min_time, system_time, &people_ask, nomal_out, floor);
			Run::Runing(&odd_elevator, min_time, system_time, &people_ask, odd_out, floor);
			Run::Runing(&even_elevator, min_time, system_time, &people_ask, even_out, floor);
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
		Run::Runing(&elevator, min_time, system_time, &people_ask, nomal_out, floor);
		Run::Runing(&odd_elevator, min_time, system_time, &people_ask, odd_out, floor);
		Run::Runing(&even_elevator, min_time, system_time, &people_ask, even_out, floor);
	}
	while (elevator.size() + odd_elevator.size() + even_elevator.size() + Run::ask_wait_size(people_ask) != 0)//直到所有人全部完成请求
	{
		int min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
		Run::Runing(&elevator, min_time, system_time, &people_ask, nomal_out, floor);
		Run::Runing(&odd_elevator, min_time, system_time, &people_ask, odd_out, floor);
		Run::Runing(&even_elevator, min_time, system_time, &people_ask, even_out, floor);
	}
	cout << elevator.system_time() << endl;
	system("pause");
	return 0;
}