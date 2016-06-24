#include "Bitmap.h"

Bitmap::Bitmap(HDC hDC, UINT uiBitmapID, int iBitmapWidth, int iBitmapHeight, BOOL bTrans,
               HINSTANCE hInstance, BOUNDSACTION BoundsAction, int iFrames)
{
   m_bTrans = bTrans;
   m_iWidth = iBitmapWidth;
   m_iHeight = iBitmapHeight;
   m_iFrames = iFrames;
   m_BoundsAction = BoundsAction;
   ptPos.x = ptPos.y = 0;
   ptVelocity.x = ptVelocity.y = 0;
   m_rcCollision.left = m_rcCollision.right = m_rcCollision.top = m_rcCollision.bottom = 0;
   m_rcBoundary.left = 0;
   m_rcBoundary.top  = 0;
   m_rcBoundary.right = 640;
   m_rcBoundary.bottom = 480;
   m_iBitmap = uiBitmapID;
   m_iShrinkSize = 0;

   m_hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(uiBitmapID));
}

Bitmap::~Bitmap()
{
}

BOOL Bitmap::CheckCollision(Bitmap* pBitmap)
{
    if((GetCollision().left <= pBitmap->GetCollision().right) && (GetCollision().right >= pBitmap->GetCollision().left) &&
       (GetCollision().top <= pBitmap->GetCollision().bottom) && (GetCollision().bottom >= pBitmap->GetCollision().top))
      return TRUE;

    return FALSE;
}

void Bitmap::SetVelocity(int x, int y)
{
    ptVelocity.x = x;
    ptVelocity.y = y;
}

void Bitmap::SetBounds(RECT rcBoundary)
{
    m_rcBoundary = rcBoundary;
}

void Bitmap::SetBounds(int x, int y, int w, int h)
{
    m_rcBoundary.left   = x;
    m_rcBoundary.top    = y;
    m_rcBoundary.right  = w;
    m_rcBoundary.bottom = h;
}

BOOL Bitmap::Update()
{
    //update the bitmap's position
    ptPos.x += ptVelocity.x;
    ptPos.y += ptVelocity.y;

    BOOL g_bCollision = FALSE;

    //Check to see if the bitmap crossed the boundary
    if(ptPos.x + GetWidth() >= m_rcBoundary.right)
    {
        if(m_BoundsAction == BA_STOP)
        {
            ptVelocity.x = 0;
            ptPos.x = m_rcBoundary.right - GetWidth();
        }
        else if(m_BoundsAction == BA_BOUNCE)
        {
            ptVelocity.x = -ptVelocity.x;
        }
        //If the bounds action is kill, delete the bitmap
        else
            delete this;

        //Tell the game that the bitmap has been deleted to avoid a segmentation fault
        g_bCollision = TRUE;
    }
    else if(ptPos.x <= m_rcBoundary.left)
    {
        if(m_BoundsAction == BA_STOP)
        {
            ptVelocity.x = 0;
            ptPos.x = m_rcBoundary.left;
        }
        else if(m_BoundsAction == BA_BOUNCE)
        {
            ptVelocity.x = -ptVelocity.x;
        }
        else
            delete this;

        //Tell the game that the bitmap has been deleted to avoid a segmentation fault
        g_bCollision = TRUE;
    }
    if(ptPos.y + GetHeight() >= m_rcBoundary.bottom)
    {
        if(m_BoundsAction == BA_STOP)
        {
            ptVelocity.y = 0;
            ptPos.y = m_rcBoundary.bottom - GetHeight();
        }
        else if(m_BoundsAction == BA_BOUNCE)
        {
            ptVelocity.y = -ptVelocity.y;
        }
        else
            delete this;

        //Tell the game that the bitmap has been deleted to avoid a segmentation fault
        g_bCollision = TRUE;
    }
    else if(ptPos.y <= m_rcBoundary.top)
    {
        if(m_BoundsAction == BA_STOP)
        {
            ptVelocity.y = 0;
            ptPos.y = m_rcBoundary.top;
        }
        else if(m_BoundsAction == BA_BOUNCE)
        {
            ptVelocity.y = -ptVelocity.y;
        }
        else
            delete this;

        //Tell the game that the bitmap has been deleted to avoid a segmentation fault
        g_bCollision = TRUE;
    }

    //Update the collision
    m_rcCollision.left   = ptPos.x + m_iShrinkSize;
    m_rcCollision.top    = ptPos.y + m_iShrinkSize;
    m_rcCollision.right  = ptPos.x + GetWidth() - m_iShrinkSize;
    m_rcCollision.bottom = ptPos.y + GetHeight() - m_iShrinkSize;

    //Tell the program that the bitmap has not passed the boundary
    return g_bCollision;
}

void Bitmap::ShrinkCollision(int iPixellsSmaller)
{
    m_iShrinkSize = iPixellsSmaller;
}

void Bitmap::Draw(HDC hDC, int iCurrentFrame, COLORREF crColorRef)
{
    if(m_hBitmap != NULL)
    {
      HDC hMemDC = CreateCompatibleDC(hDC);

      HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

      //Calculate what frame to draw
      int iYPart = 0;
      if(iCurrentFrame <= m_iFrames)
        iYPart = iCurrentFrame * GetHeight();

      if(m_bTrans)
        TransparentBlt(hDC, ptPos.x, ptPos.y, m_iWidth, m_iHeight, hMemDC, 0, iYPart, m_iWidth, m_iHeight, crColorRef);
      else
        BitBlt(hDC, ptPos.x, ptPos.y, m_iWidth, m_iHeight, hMemDC, 0, iYPart, SRCCOPY);

      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
    }
}
