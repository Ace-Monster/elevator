#pragma once
namespace myelevator
{
class Ask {
public:
	int request_time;//����ʱ��
	int wait_floor;//��ʼ¥��
	int target_floor;//Ŀ��¥��
	int status = 0;//��ǰ�ȴ�˳��-1�������ϵ��ݣ�0����δ��������1����δ�ϵ���
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
	int once_time = 0;//��������ʱ��
	int run_time = 0;//��������ʱ��
	int target_time = 1;//�����ڵ�ǰĿ��¥��ͣ����Ҫ���ѵ�ʱ��
	Time(int floor){ target_time = floor; }
	~Time() { }
};//ʱ��ģ��

class People {
public:
	int people[2] = { 0 };//��������, 0->������, 1->�ȴ�
	int size()//�������������
	{
		return people[0] + people[1];
	}
};//���ݳ˿�ģ��

class Floor {
public:
	int floor = 1;//���ݵ�ǰ¥��
	int target_floor = 3;//���ݵ�ǰĿ��¥��
};//����¥��ģ��

class Elevator {
public:
	Time times;
	People people;
	Floor floor;
	int once_time_num()//���㵥�����е��ܺ�ʱ
	{
		return times.once_time * people.size();
	}
	Elevator(int floor) : times(floor) { }
	~Elevator() { }
	
};//���ݶ���

class Run {
private:
	int plan(Elevator*, Ask[], int[], bool[], int, int);//��һ���ж�ǰ��ʱ�ı���ݲ���
	int yc(Elevator, Ask[], int[], bool[], int, int);//ycz���õģ��ڶ���֮����жϣ����ص�������ʱ��
public:
	Elevator ycz(Elevator, Ask[], int[], bool[], int);//��һ���жϣ�ֱ�Ӿ������ݵ���һ������Ŀ��
	void stopmassage(Elevator&, Ask[], int[], bool[], int);//ͣ�º��״̬����
};//������


}
