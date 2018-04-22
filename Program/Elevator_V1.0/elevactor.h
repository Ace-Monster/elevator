#pragma once
#include <iostream>
class QSS {
public:
	int requesttime;//����ʱ��
	int waitflt;//��ʼ¥��
	int tegetflt;//Ŀ��¥��
	int quan = 0;//��ǰ�ȴ�˳��-1�������ϵ��ݣ�0����δ��������1����δ�ϵ���
	/*bool operator <  (const QSS &p) const
	{
		return this->requesttime < p.requesttime;
	}*/
};//����

class Time {
public:
	int num = 0;//�ȴ���ʱ��
	int systime = 0;//ϵͳʱ��
	int onetime = 0;//��������ʱ��
	int sumtime = 0;//��������ʱ��
	int tegettime = 9;//�����ڵ�ǰĿ��¥��ͣ����Ҫ���ѵ�ʱ��
};

class Peo {
public:
	int peo[2] = { 0 };//��������, 0->������, 1->�ȴ�
	int peo_size()//�������������
	{
		return peo[0] + peo[1];
	}
};

class Flt {
public:
	int flt = 1;//���ݵ�ǰ¥��
	int tegetflt = 3;//���ݵ�ǰĿ��¥��
};

class DT {
public:
	Time times;
	Peo peo;
	Flt flt;
	int onecenum()//���㵥�����е��ܺ�ʱ
	{
		return times.onetime * peo.peo_size();
	}
	
};//����״̬

int plan(DT *tempd, int i, QSS tempq[], int wt[], bool tflt[], int n)//��һ���ж�ǰ��ʱ�ı���ݲ���
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

int yc(DT tempd, QSS tq[], int wti[], bool tf[], int j, int n)//ycz���õģ��ڶ���֮����жϣ����ص�������ʱ��
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
		t_num += yc(tempd, tempq, wt, tflt, i, n);//�ݹ�
		if (tt_num > t_num)
			tt_num = t_num;
	}
	if (tt_num == 1000000)
		return t_numi;
	delete tempq;
	return tt_num;//���ع���ʱ��
}

DT ycz(DT tempd, QSS tempq[], int wt[], bool tflt[], int n)//��һ���жϣ�ֱ�Ӿ������ݵ���һ������Ŀ��
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
	return tempd;//���ص��ݲ���
}

void stopmassage(DT &d, QSS q[], int wt[], bool tflt[], int n)//ͣ�º��״̬����
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
	printf("��%d�룬������%dͣ����%d���µ��ݣ�%d���ϵ��ݣ�������%d��\n",
	d.systime, d.flt, temp_peo - d.peo_size(), temp_peo2 - d.peo[2], d.peo_size() - d.peo[2]);
	*///ע�Ͳ������ڵ��ԣ����ÿ��ͣ��ʱ����Ա�仯
	d.times.systime++, d.times.sumtime++;
	d.times.num += d.peo.peo_size();
	std::cout << d.times.sumtime << ' ' << d.flt.flt << std::endl;
	d = ycz(d, q, wt, tflt, n);
}