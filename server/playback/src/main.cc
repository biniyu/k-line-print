/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
 * Copyright (C) 2013 ??? ?????? <oscar@ubuntu>
 * 
 * playback is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * playback is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    time_t  time;
    float   open;
    float   high;
    float   low;
    float   close;
    float   vol;    
}Bar;

//  分笔数据结构
typedef struct _tick
{
    time_t time;
    int   msec;
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
    time_t cur_time;        //  当前时间
    int cur_msec;           //
    unsigned int speed;     //  速度
    Symbol symbols[MAX_SYMBOL_PER_ACCOUNT];     //  合约
    int heartbeat;          //  心跳
}Account;

//  账户数组
Account accounts[MAX_ACCOUNT_NUM];

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
        
        for(int i = 0; i < MAX_ACCOUNT_NUM; i++)
        {
            Account& acc = accounts[i];
			
            if(acc.heartbeat > 0)
            {
				acc.cur_msec += 100 * acc.speed;
				acc.cur_time += acc.cur_msec / 1000;
				acc.cur_msec = acc.cur_msec % 1000;
				
                for(int idx_sym = 0; idx_sym < MAX_SYMBOL_PER_ACCOUNT; idx_sym++)
                {
                    Symbol& sym = acc.symbols[idx_sym];

					if(sym.active)
					{
						while(sym.startpos != sym.endpos)
						{
							// sym.tickbar[sym.startpos]
							if(sym.datatype == DATA_TYPE_TICK)
							{
								if(sym.tickbar[sym.startpos].tick.time < acc.cur_time)
								{
									// encode tick
									//sym.tickbar[sym.startpos].tick;
								}
								else if(sym.tickbar[sym.startpos].tick.time == acc.cur_time)
								{
									if(sym.tickbar[sym.startpos].tick.msec <= acc.cur_msec)
									{
										// encode tick
									}
								}
							}
							else  // DATA_TYPE_BAR 
							{
								if(sym.tickbar[sym.startpos].bar.time <= acc.cur_time)
								{
									//  encode bar
									//sym.tickbar[sym.startpos].bar;
								}
							}
							
							sym.startpos++;
							if(sym.startpos == MAX_CACHE_DATA_ITEM)
								sym.startpos = 0;
						}
					}
                }

				//  publish tick/bar for account
				
            }
        }
		
        printf("play tick\n");        
    }
}



