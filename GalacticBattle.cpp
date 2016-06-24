#include "GalacticBattle.h"

void NewGame()
{
    g_pShip->ptPos.x = g_pShip->ptPos.y = 0;

    g_vAlien.clear();
    g_vMissiles.clear();
    g_vPowerUps.clear();

    g_bShield = g_bEnlrage = FALSE;

    g_iCurrentShipFrame = 3;

    g_iScore = 0;
    g_iDifficulty = 70;

    g_bGameOver = FALSE;
}

void AddAlien()
{
    HDC hDC = GetDC(g_pGame->GetWindow());
    Bitmap* pBitmap;

    //Make it so that the kamikaze alien apears a little bit more than the sniper
    if((rand() % 20) < 12)
      pBitmap = new Bitmap(hDC, IDB_KAMIKAZE, 50, 50, TRUE, g_hInstance);
    else
    {
      pBitmap = new Bitmap(hDC, IDB_SNIPER, 50, 50, TRUE, g_hInstance, BA_KILL);
      int iXVel, iYVel;
      if(rand() % 2 == 0)
        iXVel = 2;
      else
        iXVel = -2;
      if(rand() % 2 == 0)
        iYVel = 2;
      else
        iYVel = -2;

      pBitmap->SetVelocity(iXVel, iYVel);
    }

    pBitmap->ptPos.x = rand() % 640;
    pBitmap->ptPos.y = rand() % 480;
    //Make sure aliens don't pop up too close to the ship
    while(abs(pBitmap->ptPos.x - g_pShip->ptPos.x) <= 80 || abs(pBitmap->ptPos.y - g_pShip->ptPos.y) <= 80)
    {
        pBitmap->ptPos.x = rand() % 640;
        pBitmap->ptPos.y = rand() % 480;
    }

    g_vAlien.push_back(pBitmap);
}

void UpdateAliens()
{
    vector<Bitmap*>::iterator itAlien;
    for(itAlien = g_vAlien.begin(); itAlien != g_vAlien.end(); itAlien++)
    {
        if((*itAlien)->GetBitmap() == g_pKamikaze->GetBitmap())
        {
            if(g_pShip->CheckCollision((*itAlien)) && !g_bShield)
            {
               g_bGameOver = TRUE;
               UpdateHighScores();
               if(g_bSound)
                 PlaySound((LPCSTR)IDW_CRASH, g_hInstance, SND_ASYNC | SND_RESOURCE);
            }

            //Update the alien's velocity so it chases the ship
            int iXVel, iYVel;
            if((*itAlien)->ptPos.x < g_pShip->ptPos.x)
              iXVel = 2;
            else if((*itAlien)->ptPos.x > g_pShip->ptPos.x)
              iXVel = -2;
            else
              iXVel = 0;
            if((*itAlien)->ptPos.y < g_pShip->ptPos.y)
              iYVel = 2;
            else if((*itAlien)->ptPos.y > g_pShip->ptPos.y)
               iYVel = -2;
            else
              iYVel = 0;

            (*itAlien)->SetVelocity(iXVel, iYVel);
            (*itAlien)->Update();

            vector<Bitmap*>::iterator itMissile;
            for(itMissile = g_vMissiles.begin(); itMissile != g_vMissiles.end(); itMissile++)
            {
                if((*itMissile)->CheckCollision((*itAlien)) && ((*itMissile)->GetBitmap() == g_pSMissile->GetBitmap()
                                                                || (*itMissile)->GetBitmap() == IDB_BIGSMISSILE))
                {
                    //Delete and remove the missile from the vector
                    delete(*itMissile);
                    g_vMissiles.erase(itMissile);
                    itMissile--;

                    //Delete and remove the alien from the vector
                    delete(*itAlien);
                    g_vAlien.erase(itAlien);
                    itAlien--;

                    g_iScore += 10;
                    break;
                }
            }
        }
        //Update the Blue alian
        else if((*itAlien)->GetBitmap() == g_pSniper->GetBitmap())
        {
            if(g_pShip->CheckCollision((*itAlien)) && !g_bShield)
            {
              g_bGameOver = TRUE;
              UpdateHighScores();
              if(g_bSound)
                PlaySound((LPCSTR)IDW_CRASH, g_hInstance, SND_ASYNC | SND_RESOURCE);
            }

            if(rand() % 40 == 0)
            {
                //Create the Sniper's missile
                HDC hDC = GetDC(g_pGame->GetWindow());
                Bitmap* pBitmap;
                pBitmap = new Bitmap(hDC, IDB_AMISSILE, 10, 10, TRUE, g_hInstance, BA_KILL);
                int iXVel, iYVel;
                //Set the position and velocity of the Sniper's missile
                if((*itAlien)->ptPos.x < g_pShip->ptPos.x)
                {
                    iXVel = 5;
                    pBitmap->ptPos.x = (*itAlien)->ptPos.x;
                }
                else if((*itAlien)->ptPos.x > g_pShip->ptPos.x)
                {
                    iXVel = -5;
                    pBitmap->ptPos.x = (*itAlien)->GetCollision().right;
                }
                else
                {
                    iXVel = 0;
                    pBitmap->ptPos.x = (*itAlien)->ptPos.x;
                }
                if((*itAlien)->ptPos.y < g_pShip->ptPos.y)
                {
                    iYVel = 5;
                    pBitmap->ptPos.y = (*itAlien)->ptPos.y;
                }
                else if((*itAlien)->ptPos.y > g_pShip->ptPos.y)
                {
                    iYVel = -5;
                    pBitmap->ptPos.y = (*itAlien)->GetCollision().bottom;
                }
                else
                {
                    iYVel = 0;
                    pBitmap->ptPos.y = (*itAlien)->ptPos.y;
                }

                pBitmap->SetVelocity(iXVel, iYVel);
                g_vMissiles.push_back(pBitmap);
                if(g_bSound)
                  PlaySound((LPCSTR)IDW_MISSILESHOT, g_hInstance, SND_ASYNC | SND_RESOURCE);
            }

            if((*itAlien)->Update())
            {
                g_vAlien.erase(itAlien);
                itAlien--;
                g_iScore += 5;
            }
            else
            {
                vector<Bitmap*>::iterator itMissile;
                for(itMissile = g_vMissiles.begin(); itMissile != g_vMissiles.end(); itMissile++)
                {
                    if((*itMissile)->CheckCollision((*itAlien)) && ((*itMissile)->GetBitmap() == g_pSMissile->GetBitmap()
                                                                    || (*itMissile)->GetBitmap() == IDB_BIGSMISSILE))
                    {
                        //Delete and remove the missile from the vector
                        delete(*itMissile);
                        g_vMissiles.erase(itMissile);
                        itMissile--;

                        //Delete and remove the alien from the vector
                        delete(*itAlien);
                        g_vAlien.erase(itAlien);
                        itAlien--;

                        g_iScore += 15;
                        break;
                    }
            }   }
        }
    }
}

void AddPowerUp()
{
    Bitmap* pBitmap;
    //Create the shield power up
    if(rand() % 2 == 0)
    {
        pBitmap = new Bitmap(GetDC(g_pGame->GetWindow()), IDB_SHIELD, 25, 25, TRUE, g_hInstance);
        pBitmap->ptPos.x = rand() % g_pGame->GetWidth() - pBitmap->GetWidth();
        pBitmap->ptPos.y = rand() % g_pGame->GetHeight() - pBitmap->GetHeight();
        g_bShield = TRUE;
    }

    else
    {
        pBitmap = new Bitmap(GetDC(g_pGame->GetWindow()), IDB_ENLARGE, 25, 25, TRUE, g_hInstance);
        pBitmap->ptPos.x = rand() % g_pGame->GetWidth() - pBitmap->GetWidth();
        pBitmap->ptPos.y = rand() % g_pGame->GetHeight() - pBitmap->GetHeight();
        g_bEnlrage = TRUE;
    }

    //Calling the update function sets up the collision system
    pBitmap->Update();
    g_vPowerUps.push_back(pBitmap);
}

BOOL GameInitialize(HINSTANCE hInstance)
{
    g_hInstance = hInstance;

    g_pGame = new GameEngine(hInstance, "Galactic Battle", "Galactic Battle", IDI_ICON, NULL);
    if(g_pGame == NULL)
      return FALSE;

    g_pGame->SetFrameRate(30);
}

void GameStart(HWND hWindow)
{
    srand(GetTickCount());

    HDC hDC = GetDC(hWindow);
    g_pBackground = new Bitmap(hDC, IDB_BACKGROUND, 640, 480, FALSE, g_hInstance);
    g_pShip       = new Bitmap(hDC, IDB_SHIP, 64, 64, TRUE, g_hInstance, BA_STOP, 8);
    g_pSMissile   = new Bitmap(hDC, IDB_SMISSILE, 10, 10, TRUE, g_hInstance);
    g_pKamikaze   = new Bitmap(hDC, IDB_KAMIKAZE, 50, 50, TRUE, g_hInstance);
    g_pSniper     = new Bitmap(hDC, IDB_SNIPER, 50, 50, TRUE, g_hInstance);
    g_pAMissile   = new Bitmap(hDC, IDB_AMISSILE, 10, 10, TRUE, g_hInstance);
    g_pSplash     = new Bitmap(hDC, IDB_SPLASH, 300, 200, TRUE, g_hInstance);
    g_pSplash->ptPos.x = (640 / 2) - (g_pSplash->GetHeight() / 2);
    g_pSplash->ptPos.y = (480 / 2) - (g_pSplash->GetHeight());

    g_pShip->ShrinkCollision(10);

    GetHighScores();
}

void GameEnd()
{
    delete g_pGame;
    delete g_pBackground;
    delete g_pShip;
    delete g_pSMissile;
    delete g_pKamikaze;
    delete g_pSniper;
    delete g_pAMissile;
    delete g_pSplash;

    g_vAlien.clear();
    g_vMissiles.clear();
    g_vPowerUps.clear();

    WriteHighScores();
}

void GamePaint(HDC hDC)
{
    g_pBackground->Draw(hDC);
    //Set the draw text specifications
    TCHAR szText[64];
    RECT rect = {500, 0, 600, 20};
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(255, 255, 244));

    wsprintf(szText, "Score: %i", g_iScore);
    DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    g_pShip->Draw(hDC, g_iCurrentShipFrame);

    vector<Bitmap*>::iterator iterator;
    for(iterator = g_vAlien.begin(); iterator != g_vAlien.end(); iterator++)
      (*iterator)->Draw(hDC);

    for(iterator = g_vMissiles.begin(); iterator != g_vMissiles.end(); iterator++)
      (*iterator)->Draw(hDC);

    for(iterator = g_vPowerUps.begin(); iterator != g_vPowerUps.end(); iterator++)
        (*iterator)->Draw(hDC);

    if(g_bShield || g_bEnlrage)
    {
        rect = {g_pGame->GetWidth() / 2, 0, 40, 20};
        int i = wait(g_iBeginPowerUp) / 1000;
        if(g_bShield)
            wsprintf(szText, "Shield Time: %i", 5 - i);
        else
            wsprintf(szText, "Enlarge Time: %i", 5 - i);
        DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        if(i >= 5)
        {
            g_bShield = FALSE;
            g_bEnlrage = FALSE;
        }
    }

    if(g_bGameOver)
    {
        rect = { 320, 240, 370, 270};
        //Display the high scores
        for(int i = 0; i < 3; i++)
        {
            wsprintf(szText, "%d", g_iHighScore[i]);
            DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            rect.top += 20;
            rect.bottom += 20;
        }

        g_pSplash->Draw(hDC);
    }
}

void GameCycle()
{
    if(!g_bGameOver)
    {
      if((rand() % g_iDifficulty) == 0)
        AddAlien();

      g_pShip->Update();
      UpdateAliens();

      vector<Bitmap*>::iterator itMissile;
      for(itMissile = g_vMissiles.begin(); itMissile != g_vMissiles.end(); itMissile++)
      {
          if((*itMissile)->Update())
          {
            g_vMissiles.erase(itMissile);
            itMissile--;
          }
          else if((*itMissile)->GetBitmap() == g_pAMissile->GetBitmap() && !g_bShield)
          {
              if(g_pShip->CheckCollision((*itMissile)))
              {
                g_bGameOver = TRUE;
                UpdateHighScores();
                if(g_bSound)
                  PlaySound((LPCSTR)IDW_CRASH, g_hInstance, SND_ASYNC | SND_RESOURCE);
              }
          }
      }

         //Create Power Ups
        if(!g_bEnlrage && !g_bShield && ((rand() % 500) == 0))
            AddPowerUp();

        //Initiate the power up when the ship collides with one
        vector<Bitmap*>::iterator itPowerUp;
        for(itPowerUp = g_vPowerUps.begin(); itPowerUp != g_vPowerUps.end(); itPowerUp++)
        {
            if((g_pShip->CheckCollision((*itPowerUp))) && ((*itPowerUp)->GetBitmap() == IDB_SHIELD))
            {
                g_bShield = TRUE;
                //Delete all other power ups
                delete (*itPowerUp);
                g_vPowerUps.erase(itPowerUp);
                itPowerUp--;
                g_vPowerUps.clear();
                g_iBeginPowerUp = clock();
                break;
            }
            else if((g_pShip->CheckCollision((*itPowerUp))) && ((*itPowerUp)->GetBitmap() == IDB_ENLARGE))
            {
                g_bEnlrage = TRUE;
                delete (*itPowerUp);
                g_vPowerUps.erase(itPowerUp);
                itPowerUp--;
                g_vPowerUps.clear();
                g_iBeginPowerUp = clock();
                break;
            }
        }

      if(g_iScore - g_iLastScore >= 50)
      {
          if(g_iDifficulty >= 20)
            g_iDifficulty -= 5;
          g_iLastScore = g_iScore;
      }

      if(g_iDelay <= 7)
        g_iDelay++;
    }

    InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
}

void HandleKeys()
{
    if(GetAsyncKeyState(VK_RETURN) < 0)
    {
        if(g_bGameOver)
          NewGame();
    }
    if(GetAsyncKeyState(VK_RIGHT) < 0)
    {
        if(g_iCurrentShipFrame >= 7)
          g_iCurrentShipFrame = 0;
        else
          g_iCurrentShipFrame++;
    }
    if(GetAsyncKeyState(VK_LEFT) < 0)
    {
        if(g_iCurrentShipFrame <= 0)
          g_iCurrentShipFrame = 7;
        else
          g_iCurrentShipFrame--;
    }
    if(GetAsyncKeyState(VK_UP) < 0)
    {
        switch(g_iCurrentShipFrame)
        {
            case 0:
              g_pShip->SetVelocity(0, -5);
              break;
            case 1:
              g_pShip->SetVelocity(4, -4);
              break;
            case 2:
              g_pShip->SetVelocity(5, 0);
              break;
            case 3:
              g_pShip->SetVelocity(4, 4);
              break;
            case 4:
              g_pShip->SetVelocity(0, 5);
              break;
            case 5:
              g_pShip->SetVelocity(-4, 4);
              break;
            case 6:
              g_pShip->SetVelocity(-5, 0);
              break;
            case 7:
              g_pShip->SetVelocity(-4, -4);
              break;
        }
    }
    else
      g_pShip->SetVelocity(0, 0);

    if(GetAsyncKeyState(VK_SPACE) < 0)
    {
        if(g_iDelay >= 8)
        {
          HDC hDC = GetDC(g_pGame->GetWindow());
          Bitmap* pBitmap;
          if(!g_bEnlrage)
            pBitmap = new Bitmap(hDC, IDB_SMISSILE, 10, 10, TRUE, g_hInstance, BA_KILL);
          else
            pBitmap = new Bitmap(hDC, IDB_BIGSMISSILE, 20, 20, TRUE, g_hInstance, BA_KILL);
          switch(g_iCurrentShipFrame)
          {
              case 0:
                pBitmap->ptPos.x = g_pShip->GetCollision().right - (g_pShip->GetWidth() / 2);
                pBitmap->ptPos.y = g_pShip->ptPos.y;
                pBitmap->SetVelocity(0, -8);
                break;
              case 1:
                pBitmap->ptPos.x = g_pShip->GetCollision().right;
                pBitmap->ptPos.y = g_pShip->ptPos.y;
                pBitmap->SetVelocity(8, -8);
                break;
              case 2:
                pBitmap->ptPos.x = g_pShip->ptPos.x + g_pShip->GetWidth();
                pBitmap->ptPos.y = (g_pShip->ptPos.y + g_pShip->GetHeight())- (g_pShip->GetHeight() / 2);
                pBitmap->SetVelocity(8, 0);
               break;
              case 3:
                pBitmap->ptPos.x = g_pShip->GetCollision().right;
                pBitmap->ptPos.y = g_pShip->GetCollision().bottom;
                pBitmap->SetVelocity(8, 8);
                break;
              case 4:
                pBitmap->ptPos.x = g_pShip->GetCollision().right - (g_pShip->GetWidth() / 2);
                pBitmap->ptPos.y = g_pShip->GetCollision().bottom;
                pBitmap->SetVelocity(0, 8);
                break;
              case 5:
                pBitmap->ptPos.x = g_pShip->ptPos.x;
                pBitmap->ptPos.y = g_pShip->GetCollision().bottom;
                pBitmap->SetVelocity(-8, 8);
                break;
              case 6:
                pBitmap->ptPos.x = g_pShip->ptPos.x;
                pBitmap->ptPos.y = g_pShip->GetCollision().bottom - (g_pShip->GetHeight() / 2);
                pBitmap->SetVelocity(-8, 0);
                break;
              case 7:
                pBitmap->ptPos.x = g_pShip->ptPos.x;
                pBitmap->ptPos.y = g_pShip->ptPos.y;
                pBitmap->SetVelocity(-8, -8);
                break;
          }
          g_vMissiles.push_back(pBitmap);
          if(g_bSound)
            PlaySound((LPCSTR)IDW_MISSILESHOT, g_hInstance, SND_ASYNC | SND_RESOURCE);
          g_iDelay = 0;
        }
    }
    if(GetAsyncKeyState(VK_S) < 0)
    {
        if(g_bSound)
          g_bSound = FALSE;
        else
          g_bSound = TRUE;
    }
}

void UpdateHighScores()
{
    int i;
    for(i = 0; i < 3; i++)
    {
        if(g_iScore >= g_iHighScore[i])
          break;
    }

    if(i < 3)
    {
        for(int j = 2; j > i; j--)
        {
            g_iHighScore[j] = g_iHighScore[j - 1];
        }
        g_iHighScore[i] = g_iScore;
    }
}

void GetHighScores()
{
    HANDLE hFile = CreateFile(TEXT("High.score"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    //Read the scores
    for(int i = 0; i < 3; i++)
    {
        char cData[4];
        DWORD dwRead;
        if(!ReadFile(hFile, &cData, 4, &dwRead, NULL))
          g_iHighScore[i] = 0;

        else
          g_iHighScore[i] = atoi(cData);
    }
    CloseHandle(hFile);
}

void WriteHighScores()
{
    HANDLE hFile = CreateFile(TEXT("High.score"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    for(int i = 0; i < 3; i++)
    {
        CHAR cData[4];
        wsprintf(cData, "%04d", g_iHighScore[i]);

        //Write the high score
        DWORD dwWrite;
        if(!WriteFile(hFile, &cData, 4, &dwWrite, NULL))
          MessageBox(g_pGame->GetWindow(), "NOO", "NOO", MB_OK);
    }
    //Close the file handle
    CloseHandle(hFile);
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}
