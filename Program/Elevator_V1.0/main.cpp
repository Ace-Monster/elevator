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
	cin >> n;
	int m = 3;//楼层数量
	QSS *q = new QSS[n];
	DT d;
	for (int i = 0; i < n; i++)
		cin >> q[i].requesttime >> q[i].waitflt >> q[i].tegetflt;
	//sort(q, q + 5);//按时间排序
	int *wt = new int[m];
	bool *tflt = new bool[m];
	for (int i = 0;i < n;i++)//未收全请求前，没收到一次请求就判断一次
	{
		if (d.peo.peo_size() == 0)//更新系统时间
			d.times.systime = q[i].requesttime;
		while ((d.times.systime + d.times.tegettime) < q[i].requesttime)//该乘客请求未发出时运行
		{
			if (d.peo.peo_size() == 0)
				d.times.systime = q[i].requesttime;
			else
				stopmassage(d, q, wt, tflt, n);
		}
		d.times.num += (q[i].requesttime - d.times.systime) * d.peo.peo_size();
		d.times.sumtime += q[i].requesttime - d.times.systime;
		if (d.flt.flt > d.flt.tegetflt)
			d.flt.flt -= q[i].requesttime - d.times.systime;
		else
			d.flt.flt += q[i].requesttime - d.times.systime;
		d.times.systime = q[i].requesttime;
		wt[q[i].waitflt]++;
		tflt[q[i].waitflt] = 1;
		d.peo.peo[1]++;
		q[i].quan = 1;
		d = ycz(d, q, wt, tflt, n);
	}
	while (d.peo.peo_size() != 0)//直到所有人全部完成请求
		stopmassage(d, q, wt, tflt, n);
	//cout << d.num;
	return 0;
}