#include "../common/common.h"
#include "bitmap.h"

CBitmap::CBitmap()
{
        m_buffer = NULL;
        m_bLoaded = false;
}

CBitmap::~CBitmap()
{
        this->Unload();
}


bool CBitmap::Load(const char *fileName)
{
        BMPINFOHEADER info;
        char *tempbuf = (char *)res::loadimg(fileName, &info);
        if(NULL == tempbuf)
        {
                common::Error("º”‘ÿŒªÕº [%s]  ß∞‹", fileName);
                return false;
        }

        memcpy(m_buffer, tempbuf, info.biWidth * info.biHeight);

        m_width = info.biWidth;
        m_height = info.biWidth;

        //m_bpp = info.biBitCount;

        m_bLoaded = true;

        return true;
}
void CBitmap::Unload()
{
        if(m_buffer)
        {
                delete[] m_buffer;
                m_buffer = NULL;
                m_bLoaded = false;
        }
}