#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
namespace myelevator
{
const int inf = 1e9;
class Ask {
public:
	enum Status {NONE, WAIT, DISTRIBUTED, RUNING, COMPLETED};
	//-1�������ϵ��ݣ�0����δ��������1����δ�ϵ���
private:
	int request_time;//����ʱ��
	int wait_floor;//��ʼ¥��
	int target_floor;//Ŀ��¥��
	int _end_time;
	Status status = NONE;//��ǰ�ȴ�˳��
public:
	void setting(int, int, int);

	int r_request_time();
	int r_target_floor();
	int r_wait_floor();
	Status r_status();
	void w_status(Status);
	const int end_time();
	void completed(int);
/*private:
	bool operator <  (const Ask &p) const
	{
		return this->request_time < p.request_time;
	}*/
};//����

class Time {
public:
	int system_time = 0;//ϵͳʱ��
	int run_time = 0;//��������ʱ��
	int target_time = 0;//�����ڵ�ǰĿ��¥��ͣ����Ҫ���ѵ�ʱ��
	Time() { }
	Time(int floor){ target_time = inf; }
	~Time() { }
};//ʱ��ģ��

class People {
public:
	enum Status{INSIDE = 0, WAIT = 1};
	std::vector<Ask*> people;
	People() { people.clear(); }
};//���ݳ˿�ģ��

class Floor {
public:
	enum Direction {UP = true, DOWN = false};
	int floor = 1;//���ݵ�ǰ¥��
	int target_floor = 1;//���ݵ�ǰĿ��¥��
	Floor() { }
};//����¥��ģ��

class Elevator {
private:
	std::string Out_Message;
	bool flag = true;
	Time times;
	People people;
	Floor floors;
	enum Status {STOP, RUNING};
public:
	void outmessage_set(const std::string);
	const std::string OutMessage();
	const bool OutPutJudge();

	const int run_times();
	const int system_time();
	const int target_time();
	void system_time_update(int);
	void times_update(int uptime = 1);
	
	const int floor();
	const int target_floor();
	const bool direction();
	void target_floor_update(int);
	int floor_difference(int _floor = -1);
	int floor_arrive(int _floor = -1, int uptime = -1);

	const int size();//�������������
	const std::vector<Ask> message_get();
	void people_distribute(Ask*);
	bool people_up(int);
	bool people_down(int, int);

	void target_update(int);
	
	virtual bool run(int, int);
	void stop();
	Elevator() { }
	Elevator(int floor) : times(floor) { }
	~Elevator() { }
	
};//���ݶ���

class Odd_Elevator: public Elevator
{
public:
	virtual bool run(int, int);
	Odd_Elevator(int floor) : Elevator(floor) { }
};

class Even_Elevator: public Elevator
{
public:
	virtual bool run(int, int);
	Even_Elevator(int floor) : Elevator(floor) { }
};

class Run {
public:
	static int ask_wait_size(std::vector<Ask>);
	static void Runing(Elevator*, const int, int, int, bool update_flag = true);
	static void Distrbute(Elevator*, Elevator*, Elevator*, std::vector<Ask>*, const int, int, int);
};
}