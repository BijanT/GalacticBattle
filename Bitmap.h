#define WINVER 0x0500
#include <windows.h>

typedef int BOUNDSACTION;
const  BOUNDSACTION BA_STOP = 0,
                    BA_BOUNCE = 1,
                    BA_KILL = 2;

class Bitmap
{
    private:
    HBITMAP      m_hBitmap;
    int          m_iWidth;
    int          m_iHeight;
    int          m_iFrames;
    int          m_iBitmap;
    int          m_iShrinkSize;
    BOOL         m_bTrans;
    RECT         m_rcCollision;
    RECT         m_rcBoundary;
    POINT        ptVelocity;
    BOUNDSACTION m_BoundsAction;

    public:
    Bitmap(HDC hDC, UINT uiBitmapID, int iBitmapWidth, int iBitmapHeight, BOOL bTrans, HINSTANCE hInstace,
           BOUNDSACTION BoundsAction = BA_STOP, int iFrames = 1);
    ~Bitmap();
    POINT ptPos;

    BOOL CheckCollision(Bitmap* pBitmap);
    void Draw(HDC hDC, int iCurrentFrame = 0, COLORREF crColorRef = RGB(255, 0 ,255));
    virtual BOOL Update();
    void SetVelocity(int x, int y);
    void SetBounds(RECT rcBoundary);
    void SetBounds(int x, int y, int w, int h);

    int GetWidth(){return m_iWidth; };
    int GetHeight(){return m_iHeight; };
    RECT GetCollision(){return m_rcCollision; };
    void ShrinkCollision(int iPixelsSmaller);
    int GetBitmap(){return m_iBitmap; };
};
