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

#define MAX_LISTEN 5
#define PORT 1987
#define IP "127.0.0.1"

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

void serve_proc(int conn_fd)
{
	int recv_num;
	int send_num;
	char recv_buf[100];
	char send_buf[100];	

	int pid = getpid();

	printf("client %d started\n", pid);

	while (1) {
		//receive
		recv_num = recv(conn_fd, recv_buf, sizeof(recv_buf), 0);
		if (recv_num < 0) {
			close(conn_fd);
			exit(1);
		}
		recv_buf[recv_num] = '\0';
		printf("child proc %d got : %s\n", pid, recv_buf);

		if (strcmp(recv_buf,"quit") == 0) {
			printf("child proc %d quit\n", pid);
			break;
		}

		//send
		sprintf(send_buf, "server proc got %d bytes\n", recv_num);
		send_num = send(conn_fd, send_buf, strlen(send_buf), 0);
		if (send_num < 0) {
			close(conn_fd);
			exit(1);
		}
		printf("child proc %d sent : %s\n", pid, send_buf);
	}

	close(conn_fd);
}

int main()
{
	
	signal(SIGCHLD, SIG_IGN);
	
	pid_t pid;

	int conn_fd;
	int sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if (sock_fd < 0) {
		perror("create socket failed");
		exit(1);
	}

	struct sockaddr_in addr_client;
	socklen_t client_size = sizeof(struct sockaddr_in);

	struct sockaddr_in addr_serv;
	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(PORT);
	addr_serv.sin_addr.s_addr = inet_addr(IP);

	if (bind(sock_fd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr_in)) < 0) {
		perror("bind error");
		exit(1);
	}

	if (listen(sock_fd,MAX_LISTEN) < 0) {
		perror("listen failed");
		exit(1);
	}

	while (1) {
		conn_fd = accept(sock_fd, (struct sockaddr *)&addr_client, &client_size);
		if (conn_fd < 0) {
			perror("accept failed");
			exit(1);
		}

		pid = fork();
		
		if (pid == 0) { // child proc 
			close(sock_fd);
			serve_proc(conn_fd);
		} else { // main proc
			close(conn_fd);
		}
	}
		
	close(sock_fd);
	return 0;
}
