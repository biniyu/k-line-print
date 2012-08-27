#include "Exchange.h"
#include "ExchangeService.h"
#include "ace/Message_Block.h"
#include "ace/Proactor.h"

CommandDispatch g_dispatch;

void ExchangeService::open(ACE_HANDLE h, ACE_Message_Block&)
{
	memset(buffer_, 0, sizeof(buffer_));

	buffer_start = buffer_end = 0;

	this->handle (h);
	if (this->reader_.open (*this) != 0 ||
		this->writer_.open (*this) != 0 )
	{
		ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
				  ACE_TEXT ("HA_Proactive_Service open")));
		delete this;
		return;
	}

	ACE_Message_Block *mb;
	ACE_NEW_NORETURN (mb, ACE_Message_Block (1024));
	if (this->reader_.read (*mb, mb->space ()) != 0)
	{
		ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
				  ACE_TEXT ("HA_Proactive_Service begin read")));
		mb->release ();
		delete this;
		return;
	}

	ACE_Time_Value initialDelay (3);
	ACE_Time_Value interval (5);

	ACE_Proactor::instance()->schedule_timer(*this, 
											0, 
											initialDelay, 
											interval);

	// mb is now controlled by Proactor framework.
	return;
}

void ExchangeService::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	ACE_Message_Block &mb = result.message_block ();

	if (!result.success () || result.bytes_transferred () == 0)
	{
		mb.release ();
		delete this;
	}
	else
	{
		//	检查剩余空间

		if(BUFFER_SIZE - buffer_end < mb.length())
		{
			memcpy(&buffer_[buffer_end], mb.rd_ptr(), BUFFER_SIZE - buffer_end);
			memcpy(&buffer_[0], 
					mb.rd_ptr() + BUFFER_SIZE - buffer_end, 
					mb.length() - (BUFFER_SIZE - buffer_end));

			buffer_end = mb.length() - (BUFFER_SIZE - buffer_end);
		}
		else
		{
			memcpy(&buffer_[buffer_end], mb.rd_ptr(), mb.length());
			buffer_end += mb.length();	
		}

		//	处理命令

		//	0xEE 0xAD LEN(2bytes) MSGID(4bytes) data....
		
		while(1)
		{
			int data_len = GetDataLen();

			//	只有报文头，继续接收
			if(data_len > 4)
			{
				ReadFromBuffer(bufHead, 4);

				if(bufHead[0] == 0xEE && bufHead[1] == 0xAD)
				{
					int npacksize = bufHead[2] * 256 + bufHead[3];
					//	如果数据不够
					if(data_len - 4 < npacksize) goto do_read;
					ReadFromBuffer(bufData, npacksize);
					ProcessPack(bufData, npacksize);					
				}
			}
			else
				break;
		}

do_read:

		this->reader_.read (mb, mb.space ());
	}

	return;
}

int ExchangeService::GetDataLen()
{
	return (buffer_end >= buffer_start)? 
			buffer_end - buffer_start : BUFFER_SIZE - (buffer_start - buffer_end);
}

void ExchangeService::ReadFromBuffer(unsigned char* tmpbuf, int nsize)
{
	if(buffer_start + nsize < BUFFER_SIZE)
	{
		memcpy(tmpbuf, &buffer_[buffer_start], nsize);
		buffer_start += nsize;
	}
	else
	{
		memcpy(tmpbuf, &buffer_[buffer_start], BUFFER_SIZE - buffer_start);
		memcpy(tmpbuf + BUFFER_SIZE - buffer_start, &buffer_[0], 
				nsize - (BUFFER_SIZE - buffer_start));
		buffer_start = BUFFER_SIZE - buffer_start;
	}
}

void ExchangeService::ProcessPack(unsigned char* tmpbuf, int nsize)
{
	int nMsgId = *(int*)tmpbuf;

	switch(nMsgId)
	{
	case CMDID_LOGIN:
		ProcessLogin(tmpbuf + sizeof(int), nsize - sizeof(int));
		break;
	case CMDID_NEXTDAY:
		ProcessNextDay(tmpbuf + sizeof(int), nsize - sizeof(int));
		break;
	case CMDID_PLAY:
		ProcessPlay(tmpbuf + sizeof(int), nsize - sizeof(int));
		break;
	case CMDID_TRADE:
		ProcessTrade(tmpbuf + sizeof(int), nsize - sizeof(int));
		break;
	}
}

void ExchangeService::ProcessLogin(unsigned char* buf, int nsize)
{
	//	读取用户参数，创建账户对象
	//	定时发送账户状态
}

void ExchangeService::ProcessNextDay(unsigned char* buf, int nsize)
{
	//	品种
	//	加载分时数据
	//	前日1分钟线，日K线等
}

void ExchangeService::ProcessPlay(unsigned char* buf, int nsize)
{
	//	播放、暂停、加速、减速	
}

void ExchangeService::ProcessTrade(unsigned char* buf, int nsize)
{
	//	开仓，平仓
}

void ExchangeService::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	result.message_block ().release ();
	return;
}

void ExchangeService::handle_time_out(const ACE_Time_Value &tv, const void *act)
{

}
