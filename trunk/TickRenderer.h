#pragma once

#include "TickReader.h"

class TickRenderer
{
private:

	Tick m_tick;
	CRect m_Rect;

public:
	
	TickRenderer(void);
	~TickRenderer(void);

	//	设置数据
	void SetTick(Tick tick) { m_tick = tick; }

	//	设置绘制范围
	void SetRect(CRect rect) { m_Rect = rect; }

	//	绘制盘口
	void Render(CDC* pDC);
};
