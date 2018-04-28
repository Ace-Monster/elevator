#include <iostream>
#include "elevactor.h"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;
using namespace myelevator;
int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int ask_people_sum;//��������
	int floor = 3;//¥������
	cin >> ask_people_sum;
	Ask *people_ask = new Ask[ask_people_sum];
	Elevator elevator(floor);
	for (int i = 0; i < ask_people_sum; i++)
	cin >> people_ask[i].request_time >> people_ask[i].wait_floor >> people_ask[i].target_floor;
	//sort(q, q + 5);//��ʱ������
	int *wait_peo = new int[floor];
	bool *wait_floor = new bool[floor];
	memset(wait_peo, 0, sizeof(wait_peo));
	memset(wait_floor, 0, sizeof(wait_floor));
	for (int i = 0;i < ask_people_sum;i++)//δ��ȫ����ǰ��û�յ�һ��������ж�һ��
	{
		if (elevator.people.peo_size() == 0)//����ϵͳʱ��
			elevator.times.system_time = people_ask[i].request_time;
		while ((elevator.times.system_time + elevator.times.target_time) < people_ask[i].request_time)//�ó˿�����δ����ʱ����
		{
			if (elevator.people.peo_size() == 0)
				elevator.times.system_time = people_ask[i].request_time;
			else
				stopmassage(elevator, people_ask, wait_peo, wait_floor, ask_people_sum);
		}
		elevator.times.num_time += (people_ask[i].request_time - elevator.times.system_time) * elevator.people.peo_size();
		elevator.times.run_time += people_ask[i].request_time - elevator.times.system_time;
		if (elevator.floor.floor > elevator.floor.target_floor)
			elevator.floor.floor -= people_ask[i].request_time - elevator.times.system_time;
		else
			elevator.floor.floor += people_ask[i].request_time - elevator.times.system_time;
		elevator.times.system_time = people_ask[i].request_time;
		wait_peo[people_ask[i].wait_floor]++;
		wait_floor[people_ask[i].wait_floor] = 1;
		elevator.people.people[1]++;
		people_ask[i].status = 1;
		elevator = ycz(elevator, people_ask, wait_peo, wait_floor, ask_people_sum);
	}
	while (elevator.people.peo_size() != 0)//ֱ��������ȫ���������
		stopmassage(elevator, people_ask, wait_peo, wait_floor, ask_people_sum);
	//cout << elevator.num_time;
	//system("pause");
	return 0;
}