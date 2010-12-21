


#include "Display.h"


CDisplay * CDisplay::m_display = NULL;

CDisplay * CDisplay::Get()
{
        if(m_display == NULL)
        {
                m_display = new CDisplay();
        }
        return m_display;
}

CDisplay::CDisplay()
{
        changed = false;
	StoreDisplayMode();
}

CDisplay::~CDisplay()
{
	RestoreDisplayMode();
        m_display = NULL;
}

void CDisplay::StoreDisplayMode()
{
        HDC	dc = GetDC(0);
        
        devmode_org.dmSize        = sizeof(devmode_org);
        devmode_org.dmDriverExtra = 0;
        devmode_org.dmPelsWidth   = GetDeviceCaps(dc, HORZRES);
        devmode_org.dmPelsHeight  = GetDeviceCaps(dc, VERTRES);
        devmode_org.dmBitsPerPel  = GetDeviceCaps(dc, BITSPIXEL);
        devmode_org.dmFields      = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
        
        if ((GetVersion() & 0x80000000) == 0) // Windows NT/2000 
        {		
                devmode_org.dmFields |= DM_DISPLAYFREQUENCY;
                devmode_org.dmDisplayFrequency = GetDeviceCaps(dc, VREFRESH);
        }
        ReleaseDC(0, dc);
}

void CDisplay::RestoreDisplayMode()
{
	if (changed) 
	{
		ChangeDisplaySettings(&devmode_org, 0);
		changed = false;
	}
}

bool CDisplay::ChangeDisplayMode(int width, int height)
{
	DEVMODE devmode;

	devmode = devmode_org;
	devmode.dmPelsWidth = width;
	devmode.dmPelsHeight = height;
	devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&devmode, CDS_FULLSCREEN)
                == DISP_CHANGE_SUCCESSFUL) 
        {
		changed = true;
		return true;
	}
	return false;
}




