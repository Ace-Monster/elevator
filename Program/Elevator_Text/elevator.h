#pragma once
namespace myelevator
{
class Ask {
public:
	int request_time;//请求时间
	int wait_floor;//起始楼数
	int target_floor;//目标楼层
	int status = 0;//当前等待顺序，-1代表已上电梯，0代表未发出请求，1代表未上电梯
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
	int once_time = 0;//单次运行时间
	int run_time = 0;//已运行总时间
	int target_time = 1;//电梯在当前目标楼层停下需要花费的时间
	Time(int floor){ target_time = floor; }
	~Time() { }
};//时间模块

class People {
public:
	int people[2] = { 0 };//电梯人数, 0->电梯内, 1->等待
	int size()//计算电梯总人数
	{
		return people[0] + people[1];
	}
};//电梯乘客模块

class Floor {
public:
	int floor = 1;//电梯当前楼层
	int target_floor = 3;//电梯当前目标楼层
};//电梯楼层模块

class Elevator {
public:
	Time times;
	People people;
	Floor floor;
	int once_time_num()//计算单次运行的总耗时
	{
		return times.once_time * people.size();
	}
	Elevator(int floor) : times(floor) { }
	~Elevator() { }
	
};//电梯对象

class Run {
private:
	int plan(Elevator*, Ask[], int[], bool[], int, int);//下一层判断前临时改变电梯参数
	int yc(Elevator, Ask[], int[], bool[], int, int);//ycz调用的，第二及之后的判断，返回电梯运行时间
public:
	Elevator ycz(Elevator, Ask[], int[], bool[], int);//第一层判断，直接决定电梯的下一步运行目标
	void stopmassage(Elevator&, Ask[], int[], bool[], int);//停下后的状态调整
};//运行类


}
