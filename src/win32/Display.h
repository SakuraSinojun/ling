

#pragma once

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <windows.h>

class CDisplay 
{
public:
	static CDisplay * Get();
	~CDisplay();

	void StoreDisplayMode();
	void RestoreDisplayMode();
	bool ChangeDisplayMode(int width, int height);

private:
        CDisplay();
        static CDisplay * m_display;
	DEVMODE	devmode_org;
	bool	changed;
};

#endif
