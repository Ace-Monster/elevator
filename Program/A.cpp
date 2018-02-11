#include <iostream>
#include <cstdio>
#include <cmath>
#import <algorithm>
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
    int num = 0;//预估总时间
    int systime = 0;//系统时间
    int sumtime = 0;//已运行总时间
    int tegettime;//电梯在当前目标楼层停下需要花费的时间
    int flt = 1;//电梯当前楼层
    int tegetflt = 1;//电梯当前目标楼层
    bool enflt = 0;//电梯最终停止楼层, 0->10, 1->1
    bool fgo = 0;//电梯在系统时间时运行方向, 0->10, 1->1
    int peo[3] = {0};//电梯人数, 0->上行, 1->下行, 2->等待
    int peo_size()
    {
        return peo[0]+peo[1]+peo[2];
    }
} d;//电梯状态

int t = 0;
//int yc();

int yc (DT temp)
{
    int num[7] = {-2,-1,0,1,2,3,4},sum[7];//小于0为楼层编号，-2->0->1，-1->1->10，大于0为乘客编号
    do{
        for(int j = 0;j < 7;j++)
        {
            if(num[j] < 0 && temp.flt/10 != num[j]+2 && temp.peo[!(num[j]+2)] != 0)
            {
                temp.systime += abs(temp.flt - ((num[j]+2) ? 10 : 1)) + 1;
                temp.sumtime += abs(temp.)
            }
            if(q[num[j]].quan == 1)
            {

            }
        }
    }while(next_permutation(num,num+7));
}

int main()
{
    for(int i = 0; i < 5; i++)
        scanf("%d %d %d", &q[i].requesttime, &q[i].waitflt, &q[i].fgo);
    sort(q,q+5);

    if(q[0].waitflt != 1)
        d.tegetflt = q[0].waitflt, d.tegettime = d.tegetflt, d.num = d.tegetflt + abs(d.tegetflt - !q[0].fgo * 10) + 1;
    else
        d.tegetflt = 10, d.tegettime = 10, d.num = 10;
    d.enflt = q[0].fgo;

    for(int i = 1;i < 5;i++)
    {
        while((d.systime+d.tegettime) < q[i].requesttime)
        {
            d.systime += d.tegettime;
            d.sumtime += d.tegettime;
            d.flt = d.tegetflt;
            yc(i - 1);
        }
        d.sumtime += q[i].requesttime - d.systime;
        d.systime = q[i].requesttime;

        yc(i);
    }
    return 0;
}