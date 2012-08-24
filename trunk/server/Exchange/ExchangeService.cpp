#include "ExchangeService.h"
#include "ace/Message_Block.h"
#include "ace/Proactor.h"

void ExchangeService::open(ACE_HANDLE h, ACE_Message_Block&)
{
	this->handle (h);
	if (this->reader_.open (*this) != 0 ||
	this->writer_.open (*this) != 0   )
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
		if (this->writer_.write (mb, mb.length ()) != 0)
		{
			ACE_ERROR ((LM_ERROR,
					  ACE_TEXT ("%p\n"),
					  ACE_TEXT ("starting write")));
			mb.release ();
		}
		else
		{
			ACE_Message_Block *new_mb;
			ACE_NEW_NORETURN (new_mb, ACE_Message_Block (1024));
			this->reader_.read (*new_mb, new_mb->space ());
		}
	}

	return;
}

void ExchangeService::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	result.message_block ().release ();
	return;
}