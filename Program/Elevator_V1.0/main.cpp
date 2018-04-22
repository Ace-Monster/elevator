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
	int n;//��������
	cin >> n;
	int m = 3;//¥������
	QSS *q = new QSS[n];
	DT d;
	for (int i = 0; i < n; i++)
		cin >> q[i].requesttime >> q[i].waitflt >> q[i].tegetflt;
	//sort(q, q + 5);//��ʱ������
	int *wt = new int[m];
	bool *tflt = new bool[m];
	for (int i = 0;i < n;i++)//δ��ȫ����ǰ��û�յ�һ��������ж�һ��
	{
		if (d.peo.peo_size() == 0)//����ϵͳʱ��
			d.times.systime = q[i].requesttime;
		while ((d.times.systime + d.times.tegettime) < q[i].requesttime)//�ó˿�����δ����ʱ����
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
	while (d.peo.peo_size() != 0)//ֱ��������ȫ���������
		stopmassage(d, q, wt, tflt, n);
	//cout << d.num;
	return 0;
}