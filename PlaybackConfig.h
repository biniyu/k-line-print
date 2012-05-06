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
		nGapPercentage = 0;
		nLastDayFluctuationAbove = 0;
		nLastDayFluctuationBelow = 0;
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

	bool bDayOfWeek[7];

	int nGapPercentage;
	
	int nLastDayFluctuationAbove;

	int nLastDayFluctuationBelow;

};