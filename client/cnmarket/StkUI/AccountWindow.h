#pragma once

class CAccountWindow :
	public CGuiControlBar
{
protected:
	
	CGuiTabWnd	m_TabWnd;
	CListCtrl   m_AccountInfo;
	CListCtrl	m_Positions;
	CListCtrl	m_Trades;
	CListCtrl   m_Triggers;
	CListCtrl   m_Stops;
	CListCtrl   m_History;

public:
	CAccountWindow(void);
	virtual ~CAccountWindow(void);

#if 0
	virtual void StyleDispl(DWORD dwDsp=GUISTYLE_XP)
	{
		m_StyleDisplay=dwDsp;
		m_TabWnd.StyleDispl(dwDsp);
		CGuiControlBar::StyleDispl(dwDsp);
		
	}
#endif

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void SetDefaultListCtrlStyle(CListCtrl& ctlList);
};
