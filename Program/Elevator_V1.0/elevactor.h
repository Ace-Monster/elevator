#pragma once
#include <iostream>
class QSS {
public:
	int requesttime;//请求时间
	int waitflt;//起始楼数
	int tegetflt;//目标楼层
	int quan = 0;//当前等待顺序，-1代表已上电梯，0代表未发出请求，1代表未上电梯
	/*bool operator <  (const QSS &p) const
	{
		return this->requesttime < p.requesttime;
	}*/
};//请求

class Time {
public:
	int num = 0;//等待总时间
	int systime = 0;//系统时间
	int onetime = 0;//单次运行时间
	int sumtime = 0;//已运行总时间
	int tegettime = 9;//电梯在当前目标楼层停下需要花费的时间
};

class Peo {
public:
	int peo[2] = { 0 };//电梯人数, 0->电梯内, 1->等待
	int peo_size()//计算电梯总人数
	{
		return peo[0] + peo[1];
	}
};

class Flt {
public:
	int flt = 1;//电梯当前楼层
	int tegetflt = 3;//电梯当前目标楼层
};

class DT {
public:
	Time times;
	Peo peo;
	Flt flt;
	int onecenum()//计算单次运行的总耗时
	{
		return times.onetime * peo.peo_size();
	}
	
};//电梯状态

int plan(DT *tempd, int i, QSS tempq[], int wt[], bool tflt[], int n)//下一层判断前临时改变电梯参数
{
	tempd->times.onetime = abs(tempd->flt.flt - i);
	tempd->times.systime += tempd->times.onetime;
	tempd->times.sumtime += tempd->times.onetime;
	tempd->flt.flt = i;
	tflt[tempd->flt.flt] = 0;
	int t = tempd->onecenum();
	for (int k = 0;k < n;k++)
	{
		if (tempq[k].waitflt == i && tempq[k].quan == 1)
			tempq[k].quan = -1, tempd->peo.peo[1]--, tempd->peo.peo[0]++, tflt[tempq[k].tegetflt] = 1;
		else if (tempq[k].tegetflt == i && tempq[k].quan == -1)
			tempq[k].quan = -2, tempd->peo.peo[0]--;
	}
	wt[i] = 0;
	tempd->times.systime++, tempd->times.sumtime++, t += tempd->peo.peo_size();
	return t;
}

int yc(DT tempd, QSS tq[], int wti[], bool tf[], int j, int n)//ycz调用的，第二及之后的判断，返回电梯运行时间
{
	bool tflt[11];
	memcpy(tflt, tf, sizeof(tflt));
	QSS *tempq = new QSS[n];int wt[11];
	memcpy(tempq, tq, sizeof(tempq));
	memcpy(wt, wti, sizeof(wt));
	int t_numi = plan(&tempd, j, tempq, wt, tflt, n);
	int tt_num = 1000000;
	for (int i = 1;i <= 3;i++)
	{
		if (!tflt[i])
			continue;
		int t_num = t_numi;
		t_num += yc(tempd, tempq, wt, tflt, i, n);//递归
		if (tt_num > t_num)
			tt_num = t_num;
	}
	if (tt_num == 1000000)
		return t_numi;
	delete tempq;
	return tt_num;//返回估计时间
}

DT ycz(DT tempd, QSS tempq[], int wt[], bool tflt[], int n)//第一层判断，直接决定电梯的下一步运行目标
{
	int tt_num = 1000;
	int tempflt = 0;
	for (int i = 1;i <= 3;i++)
	{
		if (!tflt[i])
			continue;
		int t_num = yc(tempd, tempq, wt, tflt, i, n);
		if (tt_num > t_num)
		{
			tempflt = i;
			tt_num = t_num;
			tempd.flt.tegetflt = i;
			tempd.times.tegettime = abs(tempd.flt.flt - tempd.flt.tegetflt);
		}
	}
	return tempd;//返回电梯参数
}

void stopmassage(DT &d, QSS q[], int wt[], bool tflt[], int n)//停下后的状态调整
{
	
	d.times.num += d.peo.peo_size() * d.times.tegettime;
	d.times.systime += d.times.tegettime;
	d.times.sumtime += d.times.tegettime;
	d.flt.flt = d.flt.tegetflt;
	tflt[d.flt.flt] = 0;
	int temp_peo = d.peo.peo_size(), temp_peo2 = d.peo.peo[1];
	d.peo.peo[0] += wt[d.flt.flt], d.peo.peo[1] -= wt[d.flt.flt], wt[d.flt.flt] = 0;
	for (int k = 0;k < n;k++)
	{
		if (q[k].waitflt == d.flt.flt && q[k].quan == 1)
			q[k].quan = -1, tflt[q[k].tegetflt] = 1;
		else if (q[k].tegetflt == d.flt.flt && q[k].quan == -1)
			q[k].quan = -2, d.peo.peo[0]--;
	}
	/*
	printf("第%d秒，电梯在%d停靠，%d人下电梯，%d人上电梯，电梯内%d人\n",
	d.systime, d.flt, temp_peo - d.peo_size(), temp_peo2 - d.peo[2], d.peo_size() - d.peo[2]);
	*///注释部分用于调试，输出每次停下时的人员变化
	d.times.systime++, d.times.sumtime++;
	d.times.num += d.peo.peo_size();
	std::cout << d.times.sumtime << ' ' << d.flt.flt << std::endl;
	d = ycz(d, q, wt, tflt, n);
}