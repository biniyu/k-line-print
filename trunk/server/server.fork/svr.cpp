#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <pthread.h>

using namespace std;

#define MAX_ACCOUNT_NUM             1000
#define MAX_CACHE_DATA_ITEM         60
#define MAX_SYMBOL_PER_ACCOUNT      3

//  K线数据结构
typedef struct _bar
{
    unsigned int date;
    unsigned int time;
    float   open;
    float   high;
    float   low;
    float   close;
    float   vol;    
}Bar;

//  分笔数据结构
typedef struct _tick
{
    unsigned int date;
    unsigned int time;
    float price;
    float vol;
    float interest;
    float b_price;
    float b_vol;
    float s_price;
    float s_vol;    
}Tick;

typedef union _u_tick_bar
{
    Tick tick;
    Bar bar;
}UTickBar;

#define DATA_TYPE_TICK  0 
#define DATA_TYPE_BAR   1 

#define BAR_TYPE_1_SECOND    0
#define BAR_TYPE_1_MINUTE    1
#define BAR_TYPE_5_MINUTE    2
#define BAR_TYPE_15_MINUTE   3
#define BAR_TYPE_30_MINUTE   4

typedef struct _symbol
{
    bool active;        // 是否激活
    string name;        // 合约名称
    short datatype;     // 数据类型
    short bartype;      // K线周期
    short startpos;     // 数据起始位置
    short endpos;       // 数据结束位置
    UTickBar tickbar[MAX_CACHE_DATA_ITEM];  //  行情数据
}Symbol;

typedef struct _account
{
    unsigned int id;        //  帐号ID        
    unsigned int lastdate;  //  上一日期
    unsigned int lasttime;  //  上一时间
    unsigned int curdate;   //  当前日期
    unsigned int curtime;   //  当前时间
    unsigned int speed;     //  速度
    Symbol symbols[MAX_SYMBOL_PER_ACCOUNT];     //  合约
    int heartbeat;          //  心跳
}Account;

//  账户数组
Account accouts[MAX_ACCOUNT_NUM];

static void* controlRoutine(void* arg)
{
    while(1)
    {
        sleep(1);
        printf("ctl tick\n");
    }
}

static void* dataRoutine(void* arg)
{
    while(1)
    {
        sleep(2);
        printf("data tick\n");
    }
}

int main()
{
    pthread_t controlThreadId, dataThreadId;

    //  控制线程
    pthread_create(&controlThreadId,
                   NULL, 
                   &controlRoutine,
                   NULL);
    
    //  数据线程
    pthread_create(&dataThreadId,
                   NULL, 
                   &dataRoutine,
                   NULL);

    //  播放线程    
    while(1)
    {
        //  100ms 粒度
        usleep(100000);
        printf("play tick\n");        
    }
}


