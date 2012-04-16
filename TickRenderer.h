#pragma once

#include "TickReader.h"

class TickRenderer
{
private:

	Tick m_tick;
	CRect m_Rect;
	bool m_bSelected;

public:
	
	TickRenderer(void);
	~TickRenderer(void);

	//	设置数据
	void SetTick(Tick tick) { m_tick = tick; }

	//	设置绘制范围
	void SetRect(CRect rect) { m_Rect = rect; }

	void Select(CPoint pt);

	//	绘制盘口
	void Render(CDC* pDC);
};
