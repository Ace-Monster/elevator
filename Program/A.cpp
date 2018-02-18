#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;
struct QSS{
    int requesttime;//请求时间
    int waitflt;//起始楼数
    bool fgo;//方向，0->10，1->1
    int quan = 0;//当前等待顺序，-1代表已上电梯，0代表未发出请求，1代表未上电梯
    bool operator <  (const QSS &p) const
    {
        return this->requesttime < p.requesttime;
    }
} q[5];//请求

struct DT{
    int num = 0;//等待总时间
    int systime = 0;//系统时间
    int onetime = 0;//单次运行时间
    int sumtime = 0;//已运行总时间
    int tegettime = 9;//电梯在当前目标楼层停下需要花费的时间
    int flt = 1;//电梯当前楼层
    int tegetflt = 10;//电梯当前目标楼层
    int peo[3] = {0};//电梯人数, 0->上行, 1->下行, 2->等待
    int peo_size()//计算电梯总人数
    {
        return peo[0]+peo[1]+peo[2];
    }
    int onecenum ()//计算单次运行的总耗时
    {
        return onetime * peo_size();
    }
} d;//电梯状态

int plan_A(DT *tempd,int i,QSS tempq[], int wt[11][2])
{
    int j = (i+2) ? 10 : 1;
    tempd->onetime = abs(tempd->flt - j);
    tempd->systime += tempd->onetime;
    tempd->sumtime += tempd->onetime;
    tempd->flt = j;
    int t = tempd->onecenum();
    tempd->peo[!(j/10)] = 0;
    if(wt[j][0] != 0 || wt[j][1] != 0)
    {
        for(int k = 0;k < 5;k++)
            if(tempq[k].waitflt == j && tempq[k].quan == 1)
                tempq[k].quan = -1, tempd->peo[2]--, tempd->peo[tempq[k].fgo]++;
        wt[j][0] = 0,wt[j][1] = 0;
    }
    tempd->systime++, tempd->sumtime++,t += tempd->peo_size();
    return t;
}

int plan_B(DT *tempd, int i, QSS tempq[], int wt[11][2])
{
    int j = tempq[i].waitflt;
    tempd->onetime = abs(tempd->flt - j);
    tempd->systime += tempd->onetime;
    tempd->sumtime += tempd->onetime;
    tempd->flt = j;
    int t = tempd->onecenum();
    for(int k = 0;k < 5;k++)
        if(tempq[k].waitflt == j && tempq[k].quan == 1)
            tempq[k].quan = -1, tempd->peo[2]--, tempd->peo[tempq[k].fgo]++;
    wt[j][0] = 0,wt[j][1] = 0;
    tempd->systime++, tempd->sumtime++,t += tempd->peo_size();
    return t;
}

int plan_AB(DT tempd, QSS tq[], const int num[], int wti[11][2])
{
    QSS tempq[5];int wt[11][2];
    memcpy(tempq, tq, sizeof(tempq));
    memcpy(wt, wti, sizeof(wt));
    int t_num = 0;
    for(int j = 0;j < 7;j++)
    {
        if(num[j] < 0 && tempd.flt != ((num[j]+2) ? 10 : 1) && tempd.peo[!(num[j]+2)] != 0)//枚举类型，判断是否到达，判断是否需要停靠
            t_num += plan_A(&tempd,num[j],tempq,wt);
        if(num[j] >= 0 && tempq[num[j]].quan == 1)//枚举类型，该乘客是否在等待
            t_num += plan_B(&tempd,num[j],tempq,wt);
    }
    if(tempd.flt != 1 && tempd.peo[1] != 0)//判断是否到达，判断是否需要停靠
        t_num += plan_A(&tempd,-2,tempq,wt);
    if(tempd.flt != 10 && tempd.peo[0] != 0)//判断是否到达，判断是否需要停靠
        t_num += plan_A(&tempd,-1,tempq,wt);
    return t_num;
}

DT ycz(DT tempd, QSS tempq[], int wt[11][2])//第一层判断，直接决定电梯的下一步运行目标
{
    int num[7] = {-2,-1,0,1,2,3,4}, tt_num = 1000;//小于0为楼层编号，-2->0->1，-1->1->10，大于0为乘客编号
    do{
        if(num[0] < 0 && (tempd.flt == ((num[0]+2) ? 10 : 1) || tempd.peo[!(num[0]+2)] == 0))
            continue;
        if(num[0] >= 0 && tempq[num[0]].quan != 1)
            continue;
        int t_num = plan_AB(tempd, tempq, num, wt);
        int tempflt = (num[0] < 0) ? ((num[0]+2) ? 10:1) : tempq[num[0]].waitflt;
        if(tt_num > t_num)
        {
            tt_num = t_num;
            tempd.tegetflt = tempflt;
            tempd.tegettime = abs(tempd.flt - tempd.tegetflt);
        }
    }while(next_permutation(num,num+7));
    return tempd;
}

void stopmassage(int wt[11][2])
{
    d.num += d.peo_size() * d.tegettime;
    d.systime += d.tegettime;
    d.sumtime += d.tegettime;
    d.flt = d.tegetflt;
    int temp_peo = d.peo_size(), temp_peo2 = d.peo[2];
    if(d.flt == 10 || d.flt == 1)
        d.peo[!(d.flt/10)] = 0;
    d.peo[0] += wt[d.flt][0], d.peo[2] -= wt[d.flt][0], wt[d.flt][0] = 0;
    d.peo[1] += wt[d.flt][1], d.peo[2] -= wt[d.flt][1], wt[d.flt][1] = 0;
    for(int k = 0;k < 5;k++)
        if(q[k].waitflt == d.flt && q[k].quan == 1)
            q[k].quan = -1;
    printf("第%d秒，电梯在%d停靠，%d人下电梯，%d人上电梯，电梯内%d人\n",
           d.systime, d.flt, temp_peo - d.peo_size(), temp_peo2 - d.peo[2], d.peo_size() - d.peo[2]);
    d.systime++,d.sumtime++;
    d.num += d.peo_size();
    d = ycz(d,q,wt);
}

int main()
{
    for(int i = 0; i < 5; i++)
        scanf("%d %d %d", &q[i].requesttime, &q[i].waitflt, &q[i].fgo);
    sort(q,q+5);
    int wt[11][2] = {0};//某层几人等待

    for(int i = 0;i < 5;i++)
    {
        if(d.peo_size() == 0)
            d.systime = q[i].requesttime;
        while((d.systime+d.tegettime) <= q[i].requesttime)
            stopmassage(wt);
        d.num += (q[i].requesttime - d.systime) * d.peo_size();
        d.sumtime += q[i].requesttime - d.systime;
        if(d.flt > d.tegetflt)
            d.flt -= q[i].requesttime - d.systime;
        else
            d.flt += q[i].requesttime - d.systime;
        d.systime = q[i].requesttime;
        wt[q[i].waitflt][q[i].fgo]++;
        d.peo[2]++;
        q[i].quan = 1;
        d = ycz(d,q,wt);
    }
    while(d.peo_size() != 0)
        stopmassage(wt);
    printf("共花时间:%d", d.num);
    return 0;
}