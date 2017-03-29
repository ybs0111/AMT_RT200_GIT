#pragma once

struct IVisionClass;

class AMTVClassWrapper
{
public:
	AMTVClassWrapper(void);
	virtual ~AMTVClassWrapper(void);
	
	inline bool IsSucceeded() { return m_isSucceeded; }
	void ShowMessage();
	int Add(int a, int b);
	void ShowForm();
	void HideForm();
	BOOL ChangeChannel(int ChannelNum);
	BOOL Result(int nNum, BSTR* str);
	BOOL EdgeScore(double *lEdgeScore);
	BOOL PatternMatching(int nNum, double *OffsetX, double *OffsetY);
	void Exit();
	
private:
	bool m_isSucceeded;
	IVisionClass *mp_VisionClass;
};