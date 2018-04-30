#include <iostream>
#include "elevator.h"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
#include <string>
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
	{
		int request_time, wait_floor, target_floor;
		cin >> request_time >> wait_floor >> target_floor;
		people_ask[i].setting(request_time, wait_floor, target_floor);
	}
	//sort(q, q + 5);//��ʱ������
	int *wait_peo = new int[floor];
	bool *wait_floor = new bool[floor];
	memset(wait_peo, 0, sizeof(wait_peo));
	memset(wait_floor, 0, sizeof(wait_floor));
	for (int i = 0;i < ask_people_sum;i++)//δ��ȫ����ǰ��û�յ�һ��������ж�һ��
	{
		if (elevator.size() == 0)//����ϵͳʱ��
			elevator.system_time_update(people_ask[i].r_request_time());
		while ((elevator.system_time() + elevator.target_time()) < people_ask[i].r_request_time())//�ó˿�����δ����ʱ����
		{
			if (elevator.size() == 0)
				elevator.system_time_update(people_ask[i].r_request_time());
			else
				Run::stop_massage(elevator, people_ask, wait_peo, wait_floor, ask_people_sum);
		}
		elevator.ask_update(people_ask[i].r_request_time());
		wait_peo[people_ask[i].r_wait_floor()]++;
		wait_floor[people_ask[i].r_wait_floor()] = 1;
		people_ask[i].w_status(Ask::WAIT);
		elevator = Run::elevator_update(elevator, people_ask, wait_peo, wait_floor, ask_people_sum);
	}
	while (elevator.size() != 0)//ֱ��������ȫ���������
		Run::stop_massage(elevator, people_ask, wait_peo, wait_floor, ask_people_sum);
	//cout << elevator.num_time;
	//system("pause");
	return 0;
}