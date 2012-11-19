#pragma once

class PlaybackConfig
{
public:

	PlaybackConfig()
	{
		nStartDate = 0;
		nEndDate = 0;
		memset(bDayOfWeek, TRUE, sizeof(bDayOfWeek));
		nStartTime = nEndTime = 0;
		bRealTime = TRUE;
		nPlaySpeed = 1;
	}

public:

	int nPlaySpeed;

	bool bRealTime;

	int nStartDate;
	int nEndDate;

	int nStartTime;
	int nEndTime;

	BOOL bDayOfWeek[7];
};