#pragma once

class PlaybackConfig
{
public:

	PlaybackConfig()
	{
		enPlaybackOrder = PLAYBACK_RANDOM;
		nStartDate = 0;
		nEndDate = 0;
		memset(bDayOfWeek, TRUE, sizeof(bDayOfWeek));
		fGapPercentage = 0;
		fLastDayFluctuationAbove = 0;
		fLastDayFluctuationBelow = 0;
		nStartTime = nEndTime = 0;
	}

public:

	enum PlaybackOrder
	{
		PLAYBACK_SEQUENTIAL,
		PLAYBACK_RANDOM
	};

	PlaybackOrder enPlaybackOrder;

	int nStartDate;
	int nEndDate;

	int nStartTime;
	int nEndTime;

	BOOL bDayOfWeek[7];

	float fGapPercentage;
	
	float fLastDayFluctuationAbove;

	float fLastDayFluctuationBelow;

};