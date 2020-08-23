#pragma once

class CMyGame : public CGame
{
	// Define sprites and other instance variables here
	bool debugMode = false;

	int level = 0;
	int cash = 0;
	int lives = 1;
	int tapes = 0;

	int totalTapes = 0;

	CSprite mordecai;
	CSprite rigby;
	CSpriteRect hitbox;
	int onPlatform = 0;
	bool facingRight = true;
	bool falling = false;
	bool isMordecai = true;
	bool isHurt = false;
	bool levelCompleted = false;
	int jumps = 0;

	int timeCompleted = 0;

	int xScrollPos = 0;

	CSprite continueBackground;

	CSprite menuBackground;
	CSprite levelOneText;
	CSprite levelTwoText;

	CSprite gameOver;

	CSprite background;
	CSprite background2;

	CSprite topBarCashIcon;
	
	CSprite topBarMordecaiLives;
	CSprite topBarRigbyLives;

	CSprite topBarVHS;

	CSpriteList platformList;
	CSpriteList bushList;
	CSpriteList cashList;
	CSpriteList lifeList;
	CSpriteList tapeList;
	CSpriteList snailList;

	CSoundPlayer music;
	CSoundPlayer jumpSFX;
	CSoundPlayer cashSFX;
	CSoundPlayer stompSFX;
	CSoundPlayer vhsSFX;
	CSoundPlayer lifeSFX;
	CSoundPlayer switchSFX;
	CSoundPlayer hurtSFX;

public:
	CMyGame(void);
	~CMyGame(void);

	void PlayerControl();
	void SnailControl();

	void SetupLevel();

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
