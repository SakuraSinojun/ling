#ifndef BITMAP_H_101207
#define BITMAP_H_101207


class CBitmap
{
public:
        CBitmap();
        ~CBitmap();

        int GetWidth() const {return m_width;}
        int GetHeight() const {return m_height;}
        bool IsLoaded() const {return m_bLoaded;}
        char *GetBuffer() const {return m_buffer;}

        bool Load(const char *fileName);
        void Unload();


private:
        char *m_buffer;
        bool m_bLoaded;
        int m_width;
        int m_height;
        int m_bpp;
};

#endif