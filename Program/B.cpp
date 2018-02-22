#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;
struct QSS{
    int requesttime;//请求时间
    int waitflt;//起始楼数
    int tegetflt;//目标楼层
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
    int peo[2] = {0};//电梯人数, 0->电梯内, 1->等待
    int peo_size()//计算电梯总人数
    {
        return peo[0]+peo[1];
    }
    int onecenum ()//计算单次运行的总耗时
    {
        return onetime * peo_size();
    }
} d;//电梯状态

int plan(DT *tempd, int i, QSS tempq[], int wt[], bool tflt[])//下一层判断前临时改变电梯参数
{
    tempd->onetime = abs(tempd->flt - i);
    tempd->systime += tempd->onetime;
    tempd->sumtime += tempd->onetime;
    tempd->flt = i;
    tflt[tempd->flt] = 0;
    int t = tempd->onecenum();
    for(int k = 0;k < 5;k++)
    {
        if(tempq[k].waitflt == i && tempq[k].quan == 1)
            tempq[k].quan = -1, tempd->peo[1]--, tempd->peo[0]++, tflt[tempq[k].tegetflt] = 1;
        else if(tempq[k].tegetflt == i && tempq[k].quan == -1)
            tempq[k].quan = -2, tempd->peo[0]--;
    }
    wt[i] = 0;
    tempd->systime++, tempd->sumtime++,t += tempd->peo_size();
    return t;
}

int yc(DT tempd, QSS tq[], int wti[], bool tf[], int j)//ycz调用的，第二及之后的判断，返回电梯运行时间
{
    bool tflt[11];
    memcpy(tflt, tf, sizeof(tflt));
    QSS tempq[5];int wt[11];
    memcpy(tempq, tq, sizeof(tempq));
    memcpy(wt, wti, sizeof(wt));
    int t_numi = plan(&tempd, j, tempq, wt, tflt);
    int tt_num = 1000000;
    for(int i = 1;i <= 10;i++)
    {
        if(!tflt[i])
            continue;
        int t_num = t_numi;
        t_num += yc(tempd, tempq, wt, tflt, i);//递归
        if (tt_num > t_num)
            tt_num = t_num;
    }
    if (tt_num == 1000000)
        return t_numi;
    return tt_num;//返回估计时间
}

DT ycz(DT tempd, QSS tempq[], int wt[], bool tflt[])//第一层判断，直接决定电梯的下一步运行目标
{
    int tt_num = 1000;
    int tempflt = 0;
    for(int i = 1;i <= 10;i++)
    {
        if(!tflt[i])
            continue;
        int t_num = yc(tempd, tempq, wt, tflt, i);
        if (tt_num > t_num)
        {
            tempflt = i;
            tt_num = t_num;
            tempd.tegetflt = i;
            tempd.tegettime = abs(tempd.flt - tempd.tegetflt);
        }
    }
    return tempd;//返回电梯参数
}

void stopmassage(int wt[], bool tflt[])//停下后的状态调整
{
    d.num += d.peo_size() * d.tegettime;
    d.systime += d.tegettime;
    d.sumtime += d.tegettime;
    d.flt = d.tegetflt;
    tflt[d.flt] = 0;
    int temp_peo = d.peo_size(), temp_peo2 = d.peo[1];
    d.peo[0] += wt[d.flt], d.peo[1] -= wt[d.flt], wt[d.flt] = 0;
    for(int k = 0;k < 5;k++)
    {
        if(q[k].waitflt == d.flt && q[k].quan == 1)
            q[k].quan = -1, tflt[q[k].tegetflt] = 1;
        else if(q[k].tegetflt == d.flt && q[k].quan == -1)
            q[k].quan = -2, d.peo[0]--;
    }
    /*
    printf("第%d秒，电梯在%d停靠，%d人下电梯，%d人上电梯，电梯内%d人\n",
           d.systime, d.flt, temp_peo - d.peo_size(), temp_peo2 - d.peo[2], d.peo_size() - d.peo[2]);
    *///注释部分用于调试，输出每次停下时的人员变化
    d.systime++,d.sumtime++;
    d.num += d.peo_size();
    d = ycz(d,q,wt,tflt);
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    for(int i = 0; i < 5; i++)
        scanf("%d %d %d", &q[i].requesttime, &q[i].waitflt, &q[i].tegetflt);
    sort(q,q+5);//按时间排序
    int wt[11] = {0};
    bool tflt[11] = {0};

    for(int i = 0;i < 5;i++)//未收全请求前，没收到一次请求就判断一次
    {
        if(d.peo_size() == 0)//更新系统时间
            d.systime = q[i].requesttime;
        while((d.systime+d.tegettime) < q[i].requesttime)//该乘客请求未发出时运行
        {
            if(d.peo_size() == 0)
                d.systime = q[i].requesttime;
            else
                stopmassage(wt,tflt);
        }
        d.num += (q[i].requesttime - d.systime) * d.peo_size();
        d.sumtime += q[i].requesttime - d.systime;
        if(d.flt > d.tegetflt)
            d.flt -= q[i].requesttime - d.systime;
        else
            d.flt += q[i].requesttime - d.systime;
        d.systime = q[i].requesttime;
        wt[q[i].waitflt]++;
        tflt[q[i].waitflt] = 1;
        d.peo[1]++;
        q[i].quan = 1;
        d = ycz(d,q,wt,tflt);
    }
    while(d.peo_size() != 0)//直到所有人全部完成请求
        stopmassage(wt, tflt);
    printf("%d", d.num);
    return 0;
}