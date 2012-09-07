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

//	��¼��ָ������Ʒ��
typedef struct
{
	char cUserName[USERNAME_LEN];
	char cPassword[PASSWORD_LEN];
	char cVarName[VARIETY_LEN];
} LoginCmd;

//	������һ�����ջ�ı�Ʒ��
typedef struct
{
	char cVarName[VARIETY_LEN];
} NextDayCmd;

typedef struct
{
	//	��Լ��
	char cContractName[CONTRACT_LEN];

	//	��������YYYY/MM/DD/W
	int nDate;

	//	��������

	//	ǰһ������1����K

	//	����tick

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








