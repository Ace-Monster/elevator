#pragma once
class ASK {
public:
	int requesttime;//请求时间
	int waitflt;//起始楼数
	int tegetflt;//目标楼层
	int sta = 0;//当前等待顺序，-1代表已上电梯，0代表未发出请求，1代表未上电梯
	/*bool operator <  (const ASK &p) const
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
	int tegettime = 1;//电梯在当前目标楼层停下需要花费的时间
	Time(int m){ tegettime = m; }
	~Time() { }
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

class ELE {
public:
	Time times;
	Peo peo;
	Flt flt;
	int onecenum()//计算单次运行的总耗时
	{
		return times.onetime * peo.peo_size();
	}
	ELE(int m) : times(m) { }
	~ELE() { }
	
};//电梯状态

int plan(ELE *tempd, int i, ASK tempAsk[], int wait_peo[], bool wait_flt[], int n)//下一层判断前临时改变电梯参数
{
	tempd->times.onetime = abs(tempd->flt.flt - i);
	tempd->times.systime += tempd->times.onetime;
	tempd->times.sumtime += tempd->times.onetime;
	tempd->flt.flt = i;
	wait_flt[tempd->flt.flt] = 0;
	int t = tempd->onecenum();
	for (int k = 0;k < n;k++)
	{
		if (tempAsk[k].waitflt == i && tempAsk[k].sta == 1)
			tempAsk[k].sta = -1, tempd->peo.peo[1]--, tempd->peo.peo[0]++, wait_flt[tempAsk[k].tegetflt] = 1;
		else if (tempAsk[k].tegetflt == i && tempAsk[k].sta == -1)
			tempAsk[k].sta = -2, tempd->peo.peo[0]--;
	}
	wait_peo[i] = 0;
	tempd->times.systime++, tempd->times.sumtime++, t += tempd->peo.peo_size();
	return t;
}

int yc(ELE tempEle, ASK tAsk[], int wait_peo_i[], bool wait_flt_i[], int j, int n)//ycz调用的，第二及之后的判断，返回电梯运行时间
{
	bool wait_flt[11];
	memcpy(wait_flt, wait_flt_i, sizeof(wait_flt));
	ASK *tempAsk = new ASK[n];int wait_peo[11];
	memcpy(tempAsk, tAsk, sizeof(tempAsk));
	memcpy(wait_peo, wait_peo_i, sizeof(wait_peo));
	int t_numi = plan(&tempEle, j, tempAsk, wait_peo, wait_flt, n);
	int tt_num = 1000000;
	for (int i = 1;i <= 3;i++)
	{
		if (!wait_flt[i])
			continue;
		int t_num = t_numi;
		t_num += yc(tempEle, tempAsk, wait_peo, wait_flt, i, n);//递归
		if (tt_num > t_num)
			tt_num = t_num;
	}
	if (tt_num == 1000000)
		return t_numi;
	delete tempAsk;
	return tt_num;//返回估计时间
}

ELE ycz(ELE tempEle, ASK tempAsk[], int wait_peo[], bool wait_flt[], int n)//第一层判断，直接决定电梯的下一步运行目标
{
	int tt_num = 1000;
	int tempflt = 0;
	for (int i = 1;i <= 3;i++)
	{
		if (!wait_flt[i])
			continue;
		int t_num = yc(tempEle, tempAsk, wait_peo, wait_flt, i, n);
		if (tt_num > t_num)
		{
			tempflt = i;
			tt_num = t_num;
			tempEle.flt.tegetflt = i;
			tempEle.times.tegettime = abs(tempEle.flt.flt - tempEle.flt.tegetflt);
		}
	}
	return tempEle;//返回电梯参数
}

void stopmassage(ELE &Ele, ASK Ask[], int wait_peo[], bool wait_flt[], int n)//停下后的状态调整
{
	
	Ele.times.num += Ele.peo.peo_size() * Ele.times.tegettime;
	Ele.times.systime += Ele.times.tegettime;
	Ele.times.sumtime += Ele.times.tegettime;
	Ele.flt.flt = Ele.flt.tegetflt;
	wait_flt[Ele.flt.flt] = 0;
	int temp_peo = Ele.peo.peo_size(), temp_peo2 = Ele.peo.peo[1];
	Ele.peo.peo[0] += wait_peo[Ele.flt.flt], Ele.peo.peo[1] -= wait_peo[Ele.flt.flt], wait_peo[Ele.flt.flt] = 0;
	for (int k = 0;k < n;k++)
	{
		if (Ask[k].waitflt == Ele.flt.flt && Ask[k].sta == 1)
			Ask[k].sta = -1, wait_flt[Ask[k].tegetflt] = 1;
		else if (Ask[k].tegetflt == Ele.flt.flt && Ask[k].sta == -1)
			Ask[k].sta = -2, Ele.peo.peo[0]--;
	}
	/*
	printf("第%d秒，电梯在%d停靠，%d人下电梯，%d人上电梯，电梯内%d人\n",
	Ele.systime, Ele.flt, temp_peo - Ele.peo_size(), temp_peo2 - Ele.peo[2], Ele.peo_size() - Ele.peo[2]);
	*///注释部分用于调试，输出每次停下时的人员变化
	Ele.times.systime++, Ele.times.sumtime++;
	Ele.times.num += Ele.peo.peo_size();
	std::cout << Ele.times.systime << ' ' << Ele.flt.flt << std::endl;
	Ele = ycz(Ele, Ask, wait_peo, wait_flt, n);
}