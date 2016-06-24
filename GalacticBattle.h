#include <windows.h>
#include <vector>
#include <ctime>
#include "Bitmap.h"
#include "GameEngine.h"
#include "Resource.h"

#define VK_S 0x53

using namespace std;

HINSTANCE       g_hInstance;
GameEngine*     g_pGame;
Bitmap*         g_pShip;
Bitmap*         g_pSMissile;
Bitmap*         g_pBackground;
Bitmap*         g_pKamikaze;
Bitmap*         g_pSniper;
Bitmap*         g_pAMissile;
Bitmap*         g_pSplash;
vector<Bitmap*> g_vMissiles;
vector<Bitmap*> g_vAlien;
vector<Bitmap*> g_vPowerUps;
BOOL            g_bEnlrage = FALSE;
BOOL            g_bShield = FALSE;
int             g_iBeginPowerUp;
int             g_iNumAstroids;
int             g_iCurrentShipFrame = 3;
int             g_iDelay = 6;
int             g_iDifficulty = 50;
int             g_iScore;
int             g_iLastScore = 0;
int             g_iHighScore[3];
BOOL            g_bGameOver = TRUE;
BOOL            g_bSound = TRUE;

void NewGame();
void AddAlien();
void UpdateAliens();

void AddPowerUp();

//Functions dealing with the highscore
void UpdateHighScores();
void WriteHighScores();
void GetHighScores();
