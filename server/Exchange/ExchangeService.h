#pragma once

#include "ace/OS.h"
#include "ace/Asynch_IO.h"

class ExchangeService : public ACE_Service_Handler
{
public:
	~ExchangeService()
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
};