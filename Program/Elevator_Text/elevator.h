#pragma once
namespace myelevator
{
class Ask {
public:
	enum Status {NONE, WAIT, RUNING, COMPLETED};
	//-1�������ϵ��ݣ�0����δ��������1����δ�ϵ���
private:
	int request_time;//����ʱ��
	int wait_floor;//��ʼ¥��
	int target_floor;//Ŀ��¥��
	Status status;//��ǰ�ȴ�˳��
public:
	void setting(int, int, int);
	int r_request_time();
	int r_target_floor();
	int r_wait_floor();
	Status r_status();
	void w_status(Status);
/*private:
	bool operator <  (const Ask &p) const
	{
		return this->request_time < p.request_time;
	}*/
};//����

class Time {
public:
	int num_time = 0;//�ȴ���ʱ��
	int system_time = 0;//ϵͳʱ��
	int run_time = 0;//��������ʱ��
	int target_time = 1;//�����ڵ�ǰĿ��¥��ͣ����Ҫ���ѵ�ʱ��
	Time(int floor){ target_time = floor; }
	~Time() { }
};//ʱ��ģ��

class People {
public:
	enum Status{INSIDE = 0, WAIT = 1};
	int people[2] = { 0 };//��������, 0->������, 1->�ȴ�
};//���ݳ˿�ģ��

class Floor {
public:
	int floor = 1;//���ݵ�ǰ¥��
	int target_floor = 3;//���ݵ�ǰĿ��¥��
};//����¥��ģ��

class Elevator {
private:
	Time times;
	People people;
	Floor floors;
private:
	int run_times();
	int num_times();
public:
	int system_time();
	int target_time();
	void num_time_update(int uptime = 1);
	void system_time_update(int);
	int times_update(int uptime = 1);
	
	int floor();
	int target_floor();
	void w_target_floor(int);
	int floor_difference(int);
	int floor_arrive(int, int);

	int people_num(People::Status);
	int size();//�������������
	void people_up(int people_sum = 1);
	void people_down(int people_num = 1);

	void ask_update(int);
	
	Elevator(int floor) : times(floor) { }
	~Elevator() { }
	
};//���ݶ���

class Run {
private:
	static int time_compute(Elevator*, Ask[], int[], bool[], int, int);//��һ���ж�ǰ��ʱ�ı���ݲ���
	static int dfs(Elevator, Ask[], int[], bool[], int, int);//ycz���õģ��ڶ���֮����жϣ����ص�������ʱ��
public:
	static Elevator elevator_update(Elevator, Ask[], int[], bool[], int);//��һ���жϣ�ֱ�Ӿ������ݵ���һ������Ŀ��
	static void stop_massage(Elevator&, Ask[], int[], bool[], int);//ͣ�º��״̬����
};//������
}
