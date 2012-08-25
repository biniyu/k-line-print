// Test.cpp : 定义控制台应用程序的入口点。
//

#include "ace/OS.h"
#include <ace/Process.h>
#include <ace/Thread_Manager.h>

#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Asynch_Connector.h"
#include "ace/Asynch_IO.h"
#include "ace/Proactor.h"
#include "ace/WIN32_Proactor.h" 

class Exchange_Proactive_Service : public ACE_Service_Handler
{
public:
  ~Exchange_Proactive_Service ()
   {
     if (this->handle () != ACE_INVALID_HANDLE)
       ACE_OS::closesocket (this->handle ());
   }

  virtual void open (ACE_HANDLE h, ACE_Message_Block&);

  // This method will be called when an asynchronous read
  // completes on a stream.
  virtual void handle_read_stream
    (const ACE_Asynch_Read_Stream::Result &result);

  // This method will be called when an asynchronous write
  // completes on a stream.
  virtual void handle_write_stream
    (const ACE_Asynch_Write_Stream::Result &result);

private:
  ACE_Asynch_Read_Stream reader_;
  ACE_Asynch_Write_Stream writer_;

  int nCurSize; 
};

void
Exchange_Proactive_Service::open (ACE_HANDLE h, ACE_Message_Block&)
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

  nCurSize = 100;

  ACE_Message_Block *mb = new ACE_Message_Block(nCurSize);

  unsigned char buffer[100] = {0xEE,0xAD,0x00,0x03, 0x1,0x2,0x3 };

  mb->copy((char*)buffer, 7);

  if (this->writer_.write (*mb, mb->length()) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("starting write")));
      mb->release ();
    }

  // mb is now controlled by Proactor framework.
  return;
}

void
Exchange_Proactive_Service::handle_read_stream
  (const ACE_Asynch_Read_Stream::Result &result)
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

void
Exchange_Proactive_Service::handle_write_stream
(const ACE_Asynch_Write_Stream::Result &result)
{
  result.message_block ().release ();
  return;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
	ACE_INET_Addr peer_addr(12345, "127.0.0.1");    // Set up peer addr

	ACE_Asynch_Connector<Exchange_Proactive_Service> aio_connect;

	aio_connect.open();
	aio_connect.connect (peer_addr);

	int success = 1;

	while (success > 0)
	{
		// Dispatch events via Proactor singleton.
		success = ACE_Proactor::instance ()->handle_events ();
	}

	return 0;
}

