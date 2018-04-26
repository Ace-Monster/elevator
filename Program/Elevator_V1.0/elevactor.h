#pragma once
class ASK {
public:
	int requesttime;//����ʱ��
	int waitflt;//��ʼ¥��
	int tegetflt;//Ŀ��¥��
	int sta = 0;//��ǰ�ȴ�˳��-1�������ϵ��ݣ�0����δ��������1����δ�ϵ���
	/*bool operator <  (const ASK &p) const
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
	int tegettime = 1;//�����ڵ�ǰĿ��¥��ͣ����Ҫ���ѵ�ʱ��
	Time(int m){ tegettime = m; }
	~Time() { }
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

class ELE {
public:
	Time times;
	Peo peo;
	Flt flt;
	int onecenum()//���㵥�����е��ܺ�ʱ
	{
		return times.onetime * peo.peo_size();
	}
	ELE(int m) : times(m) { }
	~ELE() { }
	
};//����״̬

int plan(ELE *tempd, int i, ASK tempAsk[], int wait_peo[], bool wait_flt[], int n)//��һ���ж�ǰ��ʱ�ı���ݲ���
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

int yc(ELE tempEle, ASK tAsk[], int wait_peo_i[], bool wait_flt_i[], int j, int n)//ycz���õģ��ڶ���֮����жϣ����ص�������ʱ��
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
		t_num += yc(tempEle, tempAsk, wait_peo, wait_flt, i, n);//�ݹ�
		if (tt_num > t_num)
			tt_num = t_num;
	}
	if (tt_num == 1000000)
		return t_numi;
	delete tempAsk;
	return tt_num;//���ع���ʱ��
}

ELE ycz(ELE tempEle, ASK tempAsk[], int wait_peo[], bool wait_flt[], int n)//��һ���жϣ�ֱ�Ӿ������ݵ���һ������Ŀ��
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
	return tempEle;//���ص��ݲ���
}

void stopmassage(ELE &Ele, ASK Ask[], int wait_peo[], bool wait_flt[], int n)//ͣ�º��״̬����
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
	printf("��%d�룬������%dͣ����%d���µ��ݣ�%d���ϵ��ݣ�������%d��\n",
	Ele.systime, Ele.flt, temp_peo - Ele.peo_size(), temp_peo2 - Ele.peo[2], Ele.peo_size() - Ele.peo[2]);
	*///ע�Ͳ������ڵ��ԣ����ÿ��ͣ��ʱ����Ա�仯
	Ele.times.systime++, Ele.times.sumtime++;
	Ele.times.num += Ele.peo.peo_size();
	std::cout << Ele.times.systime << ' ' << Ele.flt.flt << std::endl;
	Ele = ycz(Ele, Ask, wait_peo, wait_flt, n);
}