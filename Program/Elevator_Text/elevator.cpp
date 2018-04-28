#include "elevator.h"
#include <cmath>
#include <cstring>
#include <iostream>

using namespace myelevator;

int Run::plan(Elevator *t_elevator, Ask t_people_ask[], int wait_people[], bool wait_floor[], int i, int ask_people_sum)//��һ���ж�ǰ��ʱ�ı���ݲ���
{
	t_elevator->times.once_time = abs(t_elevator->floor.floor - i);
	t_elevator->times.system_time += t_elevator->times.once_time;
	t_elevator->times.run_time += t_elevator->times.once_time;
	t_elevator->floor.floor = i;
	wait_floor[t_elevator->floor.floor] = 0;
	int t = t_elevator->once_time_num();
	for (int k = 0;k < ask_people_sum;k++)
	{
		if (t_people_ask[k].wait_floor == i && t_people_ask[k].status == 1)
			t_people_ask[k].status = -1, t_elevator->people.people[1]--, t_elevator->people.people[0]++, wait_floor[t_people_ask[k].target_floor] = 1;
		else if (t_people_ask[k].target_floor == i && t_people_ask[k].status == -1)
			t_people_ask[k].status = -2, t_elevator->people.people[0]--;
	}
	wait_people[i] = 0;
	t_elevator->times.system_time++, t_elevator->times.run_time++, t += t_elevator->people.size();
	return t;
}

int Run::yc(Elevator t_elevator, Ask tAsk[], int wait_people_i[], bool wait_floor_i[], int j, int ask_people_sum)//ycz���õģ��ڶ���֮����жϣ����ص�������ʱ��
{
	bool wait_floor[11];
	memcpy(wait_floor, wait_floor_i, sizeof(wait_floor));
	Ask *t_people_ask = new Ask[ask_people_sum];int wait_people[11];
	memcpy(t_people_ask, tAsk, sizeof(t_people_ask));
	memcpy(wait_people, wait_people_i, sizeof(wait_people));
	int t_numi = plan(&t_elevator, t_people_ask, wait_people, wait_floor, j, ask_people_sum);
	int tt_num = 1000000;
	for (int i = 1;i <= 3;i++)
	{
		if (!wait_floor[i])
			continue;
		int t_num = t_numi;
		t_num += yc(t_elevator, t_people_ask, wait_people, wait_floor, i, ask_people_sum);//�ݹ�
		if (tt_num > t_num)
			tt_num = t_num;
	}
	if (tt_num == 1000000)
		return t_numi;
	delete t_people_ask;
	return tt_num;//���ع���ʱ��
}

Elevator Run::ycz(Elevator t_elevator, Ask t_people_ask[], int wait_people[], bool wait_floor[], int ask_people_sum)//��һ���жϣ�ֱ�Ӿ������ݵ���һ������Ŀ��
{
	int tt_num = 1000;
	int tempflt = 0;
	for (int i = 1;i <= 3;i++)
	{
		if (!wait_floor[i])
			continue;
		int t_num = yc(t_elevator, t_people_ask, wait_people, wait_floor, i, ask_people_sum);
		if (tt_num > t_num)
		{
			tempflt = i;
			tt_num = t_num;
			t_elevator.floor.target_floor = i;
			t_elevator.times.target_time = abs(t_elevator.floor.floor - t_elevator.floor.target_floor);
		}
	}
	return t_elevator;//���ص��ݲ���
}

void Run::stopmassage(Elevator &elevator, Ask people_ask[], int wait_people[], bool wait_floor[], int ask_people_sum)//ͣ�º��״̬����
{

	elevator.times.num_time += elevator.people.size() * elevator.times.target_time;
	elevator.times.system_time += elevator.times.target_time;
	elevator.times.run_time += elevator.times.target_time;
	elevator.floor.floor = elevator.floor.target_floor;
	wait_floor[elevator.floor.floor] = 0;
	int temp_peo = elevator.people.size(), temp_peo2 = elevator.people.people[1];
	elevator.people.people[0] += wait_people[elevator.floor.floor], elevator.people.people[1] -= wait_people[elevator.floor.floor], wait_people[elevator.floor.floor] = 0;
	for (int k = 0;k < ask_people_sum;k++)
	{
		if (people_ask[k].wait_floor == elevator.floor.floor && people_ask[k].status == 1)
			people_ask[k].status = -1, wait_floor[people_ask[k].target_floor] = 1;
		else if (people_ask[k].target_floor == elevator.floor.floor && people_ask[k].status == -1)
			people_ask[k].status = -2, elevator.people.people[0]--;
	}
	/*
	printf("��%d�룬������%dͣ����%d���µ��ݣ�%d���ϵ��ݣ�������%d��\ask_people_sum",
	elevator.system_time, elevator.floor, temp_peo - elevator.size(), temp_peo2 - elevator.people[2], elevator.size() - elevator.people[2]);
	*///ע�Ͳ������ڵ��ԣ����ÿ��ͣ��ʱ����Ա�仯
	elevator.times.system_time++, elevator.times.run_time++;
	elevator.times.num_time += elevator.people.size();
	std::cout << elevator.times.system_time << ' ' << elevator.floor.floor << std::endl;
	elevator = ycz(elevator, people_ask, wait_people, wait_floor, ask_people_sum);
}