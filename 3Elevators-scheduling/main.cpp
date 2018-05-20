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
	cout << "������������\n���������dfs�㷨��Ч�ʲ��ߣ������ĵȴ�\nö�ٴ󷨺ã��������漣\n" << endl;
	ifstream Input;
	ofstream Clean;
	
	Input.open("input.txt");
	int ask_people_sum, floor = 10;//��������,¥������
	Input >> ask_people_sum;

	Elevator elevator(floor);
	elevator.outmessage_set("output2.txt");
	Odd_Elevator odd_elevator(floor);
	odd_elevator.outmessage_set("output1.txt");
	Even_Elevator even_elevator(floor);
	even_elevator.outmessage_set("output3.txt");
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
	//sort(q, q + 5);//��ʱ������

	int system_time = people_ask[0].r_request_time();//����ϵͳʱ��
	for (int i = 0;i < ask_people_sum;i++)//δ��ȫ����ǰ��û�յ�һ��������ж�һ��
	{
		int min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
		while (system_time + min_time < people_ask[i].r_request_time())
		{
			system_time += min_time;
			Run::Runing(&elevator, floor, min_time, system_time);
			Run::Runing(&odd_elevator, floor, min_time, system_time);
			Run::Runing(&even_elevator, floor, min_time, system_time);
			min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
			if (min_time = inf)
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
		Run::Distrbute(&elevator, &odd_elevator, &even_elevator, &people_ask, floor, min_time, system_time);
	}
	while (elevator.size() + odd_elevator.size() + even_elevator.size() + Run::ask_wait_size(people_ask) != 0)//ֱ��������ȫ���������
	{
		int min_time = min(elevator.target_time(), min(odd_elevator.target_time(), even_elevator.target_time()));
		Run::Runing(&elevator, floor, min_time, system_time);
		Run::Runing(&odd_elevator, floor, min_time, system_time);
		Run::Runing(&even_elevator, floor, min_time, system_time);
	}
	//cout << elevator.system_time() << endl;
	cout << "�������н���" << endl;
	system("pause");
	return 0;
}