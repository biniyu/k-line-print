#pragma once

#define CMDID_LOGIN				1
#define CMDID_NEXTDAY			2
#define CMDID_NEXTDAY_ACK		3
#define	CMDID_PLAY				4
#define CMDID_TRADE				5
#define CMDID_LOG				6

#define	USERNAME_LEN			16
#define	PASSWORD_LEN			16
#define VARIETY_LEN				8
#define CONTRACT_LEN			16

//	登录并指定交易品种
typedef struct
{
	char cUserName[USERNAME_LEN];
	char cPassword[PASSWORD_LEN];
	char cVarName[VARIETY_LEN];
} LoginCmd;

//	进入下一交易日或改变品种
typedef struct
{
	char cVarName[VARIETY_LEN];
} NextDayCmd;

typedef struct
{
	//	合约名
	char cContractName[CONTRACT_LEN];

	//	交易日期YYYY/MM/DD/W
	int nDate;

	//	日线数据

	//	前一交易日1分钟K

	//	开盘tick

} NextDayAck;

typedef struct
{

} PlayCmd;

typedef struct
{

} PlayAck;

typedef struct
{

} TradeCmd;

typedef struct
{

} TradeAck;








