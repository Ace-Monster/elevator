#pragma once
namespace myelevator
{
class Ask {
public:
	enum Status {NONE, WAIT, RUNING, COMPLETED};
	//-1代表已上电梯，0代表未发出请求，1代表未上电梯
private:
	int request_time;//请求时间
	int wait_floor;//起始楼数
	int target_floor;//目标楼层
	Status status;//当前等待顺序
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
};//请求

class Time {
public:
	int num_time = 0;//等待总时间
	int system_time = 0;//系统时间
	int run_time = 0;//已运行总时间
	int target_time = 1;//电梯在当前目标楼层停下需要花费的时间
	Time(int floor){ target_time = floor; }
	~Time() { }
};//时间模块

class People {
public:
	enum Status{INSIDE = 0, WAIT = 1};
	int people[2] = { 0 };//电梯人数, 0->电梯内, 1->等待
};//电梯乘客模块

class Floor {
public:
	int floor = 1;//电梯当前楼层
	int target_floor = 3;//电梯当前目标楼层
};//电梯楼层模块

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
	int size();//计算电梯总人数
	void people_up(int people_sum = 1);
	void people_down(int people_num = 1);

	void ask_update(int);
	
	Elevator(int floor) : times(floor) { }
	~Elevator() { }
	
};//电梯对象

class Run {
private:
	static int time_compute(Elevator*, Ask[], int[], bool[], int, int);//下一层判断前临时改变电梯参数
	static int dfs(Elevator, Ask[], int[], bool[], int, int);//ycz调用的，第二及之后的判断，返回电梯运行时间
public:
	static Elevator elevator_update(Elevator, Ask[], int[], bool[], int);//第一层判断，直接决定电梯的下一步运行目标
	static void stop_massage(Elevator&, Ask[], int[], bool[], int);//停下后的状态调整
};//运行类
}
