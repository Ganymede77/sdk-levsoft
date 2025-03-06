// World_Window.h: interface for the CWorld_Window class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLD_WINDOW_H__33EBD0C8_0DC4_11D7_9960_000103D57990__INCLUDED_)
#define AFX_WORLD_WINDOW_H__33EBD0C8_0DC4_11D7_9960_000103D57990__INCLUDED_


class CWorld_Window
{
public:
	CWorld_Window();
	virtual ~CWorld_Window();
protected:
	double x_min, x_max, y_min, y_max;

public:
	double GetXMin();
	double GetXMax();
	double GetYMin();
	double GetYMax();
	void   SetXMin(double posx);
	void   SetXMax(double posx);
	void   SetYMin(double posy);
	void   SetYMax(double posy);
};

#endif // !defined(AFX_WORLD_WINDOW_H__33EBD0C8_0DC4_11D7_9960_000103D57990__INCLUDED_)
