#include <iostream>
#include "elevactor.h"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;
int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int n;//请求人数
	int m = 3;//楼层数量
	cin >> n;
	ASK *Ask = new ASK[n];
	ELE Ele(m);
	for (int i = 0; i < n; i++)
	cin >> Ask[i].requesttime >> Ask[i].waitflt >> Ask[i].tegetflt;
	//sort(q, q + 5);//按时间排序
	int *wait_peo = new int[m];
	bool *wait_flt = new bool[m];
	memset(wait_peo, 0, sizeof(wait_peo));
	memset(wait_flt, 0, sizeof(wait_flt));
	for (int i = 0;i < n;i++)//未收全请求前，没收到一次请求就判断一次
	{
		if (Ele.peo.peo_size() == 0)//更新系统时间
			Ele.times.systime = Ask[i].requesttime;
		while ((Ele.times.systime + Ele.times.tegettime) < Ask[i].requesttime)//该乘客请求未发出时运行
		{
			if (Ele.peo.peo_size() == 0)
				Ele.times.systime = Ask[i].requesttime;
			else
				stopmassage(Ele, Ask, wait_peo, wait_flt, n);
		}
		Ele.times.num += (Ask[i].requesttime - Ele.times.systime) * Ele.peo.peo_size();
		Ele.times.sumtime += Ask[i].requesttime - Ele.times.systime;
		if (Ele.flt.flt > Ele.flt.tegetflt)
			Ele.flt.flt -= Ask[i].requesttime - Ele.times.systime;
		else
			Ele.flt.flt += Ask[i].requesttime - Ele.times.systime;
		Ele.times.systime = Ask[i].requesttime;
		wait_peo[Ask[i].waitflt]++;
		wait_flt[Ask[i].waitflt] = 1;
		Ele.peo.peo[1]++;
		Ask[i].sta = 1;
		Ele = ycz(Ele, Ask, wait_peo, wait_flt, n);
	}
	while (Ele.peo.peo_size() != 0)//直到所有人全部完成请求
		stopmassage(Ele, Ask, wait_peo, wait_flt, n);
	//cout << Ele.num;
	//system("pause");
	return 0;
}