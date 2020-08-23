// CREDITS
// ------------------------------------------------------------------------
// Sound effects:  freesound.org
// Music and art/assets:  Regular Show: Mordecai and Rigby in 8-bit Land


#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void) : rigby(64, 240, 128, 128, 0),
mordecai(64, 240, 128, 128, 0),
background(6585, 360, "level1.png", 0),
background2(5098, 360, "level2.png", 0),
hitbox(64, 240, 52, 94, CColor::Black(), 0),
topBarCashIcon(1262, 754, 0, 0, 0),
topBarMordecaiLives(30, 740, 0, 0, 0),
topBarRigbyLives(30, 740, 0, 0, 0),
topBarVHS(570, 722, 0, 0, 0),
menuBackground(640, 360, "menu.png", 0),
gameOver(640, 360, "gameOver.png", 0),
continueBackground(640, 360, "continue.png", 0),
levelOneText(641, 360, 0, 0, 0),
levelTwoText(641, 120, 0, 0, 0) {

}

CMyGame::~CMyGame(void) {

}

void CMyGame::OnUpdate() {
	Uint32 t = GetTime();
	if (IsMenuMode() && level < 1) {
		menuBackground.Update(t);
		levelOneText.Update(t);
		levelTwoText.Update(t);
	} else if (timeCompleted == 300) {
		continueBackground.Update(t);
	} else if (IsGameMode()) {
		if (GetLevel() == 1) {
			background.Update(t);
		} else if (GetLevel() == 2) {
			background2.Update(t);
		}
		PlayerControl();
		for (CSprite* platform : platformList) if (platform->GetRight() > abs(xScrollPos) && platform->GetLeft() < abs(xScrollPos)+1400) platform->Update(t);
		for (CSprite* bush : bushList) if (bush->GetRight() > abs(xScrollPos) && bush->GetLeft() < abs(xScrollPos)+1400) bush->Update(t);
		for (CSprite* thisCash : cashList) if (thisCash->GetRight() > abs(xScrollPos) && thisCash->GetLeft() < abs(xScrollPos)+1400) thisCash->Update(t);
		for (CSprite* life : lifeList) if (life->GetRight() > abs(xScrollPos) && life->GetLeft() < abs(xScrollPos)+1400) life->Update(t);
		for (CSprite* vhs : tapeList) if (vhs->GetRight() > abs(xScrollPos) && vhs->GetLeft() < abs(xScrollPos)+1400) vhs->Update(t);
		SnailControl();
	}
	if (IsGameMode()) {
		topBarCashIcon.Update(t);
		topBarMordecaiLives.Update(t);
		topBarRigbyLives.Update(t);
		topBarVHS.Update(t);
	}
	if (IsGameOver()) {
		gameOver.Update(t);
	}
}

void CMyGame::OnDraw(CGraphics* g) {
	if (IsMenuMode() && level < 1) {
		menuBackground.Draw(g);
		levelOneText.Draw(g);
		levelTwoText.Draw(g);
	} else if (timeCompleted == 300) {
		continueBackground.Draw(g);
	} else if (IsGameMode()) {
		int leftScreenLimit = 300, rightScreenLimit;
		if (level == 1) rightScreenLimit = 11995;
		else if (level == 2) rightScreenLimit = 9220;

		if (hitbox.GetX() >= leftScreenLimit && hitbox.GetX() <= rightScreenLimit) g->SetScrollPos(leftScreenLimit - hitbox.GetX(), 0);
		if (hitbox.GetX() > rightScreenLimit) g->SetScrollPos(-rightScreenLimit + leftScreenLimit, 0);
		xScrollPos = g->GetScrollPos().GetX();

		if (level == 1) background.Draw(g);
		else if (level == 2) background2.Draw(g);

		if (debugMode) for (CSprite* platform : platformList) if (platform->GetRight() > abs(xScrollPos) && platform->GetLeft() < abs(xScrollPos)+1280) platform->Draw(g);
		if (debugMode) for (CSprite* bush : bushList) if (bush->GetRight() > abs(xScrollPos) && bush->GetLeft() < abs(xScrollPos)+1280) bush->Draw(g);
		if (debugMode) hitbox.Draw(g);
		(isMordecai ? mordecai : rigby).Draw(g);
		for (CSprite* thisCash : cashList) if (thisCash->GetRight() > abs(xScrollPos) && thisCash->GetLeft() < abs(xScrollPos)+1280) thisCash->Draw(g);
		for (CSprite* life : lifeList) if (life->GetRight() > abs(xScrollPos) && life->GetLeft() < abs(xScrollPos)+1280) life->Draw(g);
		for (CSprite* vhs : tapeList) if (vhs->GetRight() > abs(xScrollPos) && vhs->GetLeft() < abs(xScrollPos)+1280) vhs->Draw(g);
		for (CSprite* snail : snailList) if (snail->GetRight() > abs(xScrollPos) && snail->GetLeft() < abs(xScrollPos)+1280) snail->Draw(g);

	}

	if (IsGameMode()) {
		topBarMordecaiLives.SetX(30 - xScrollPos);
		topBarRigbyLives.SetX(30 - xScrollPos);
		topBarCashIcon.SetX(1200-xScrollPos);
		topBarVHS.SetX(640-topBarVHS.GetWidth()/2 - xScrollPos);
		topBarCashIcon.Draw(g);

		(isMordecai ? topBarMordecaiLives : topBarRigbyLives).Draw(g);
		*g << xy(topBarMordecaiLives.GetWidth() / 2 - xScrollPos + 5, 684) << font("RETRO_GAMING.TTF", 24) << "x" << lives;
		*g << xy(topBarVHS.GetRight()-25, 684) << font("RETRO_GAMING.TTF", 24) << tapes << "/" << totalTapes;
		*g << xy(1220-xScrollPos, 684) << font("RETRO_GAMING.TTF", 24) << "$" << cash;
		topBarVHS.Draw(g);
	}

	if (IsGameOver()) {
		gameOver.Draw(g);
	}
}

void CMyGame::OnInitialize() {
	// menu text
	levelOneText.AddImage("levelSelect.png", "text", 2, 2, 0, 1, 0, 1, CColor::DarkMagenta());
	levelOneText.AddImage("levelSelect.png", "hover", 2, 2, 1, 1, 1, 1, CColor::DarkMagenta());
	levelTwoText.AddImage("levelSelect.png", "text", 2, 2, 0, 0, 0, 0, CColor::DarkMagenta());
	levelTwoText.AddImage("levelSelect.png", "hover", 2, 2, 1, 0, 1, 0, CColor::DarkMagenta());

	// rigby right
	rigby.AddImage("rigbyRight.png", "rightNeutral", 8, 19, 0, 17, 0, 17, CColor::DarkMagenta());
	rigby.AddImage("rigbyRight.png", "rightRun", 8, 19, 4, 15, 7, 15, CColor::DarkMagenta());
	rigby.AddImage("rigbyRight.png", "rightJump", 8, 19, 0, 13, 0, 13, CColor::DarkMagenta());
	rigby.AddImage("rigbyRight.png", "rightFall", 8, 19, 2, 13, 2, 13, CColor::DarkMagenta());
	rigby.AddImage("rigbyRight.png", "rightHurt", 8, 19, 2, 11, 2, 11, CColor::DarkMagenta());
	rigby.AddImage("rigbyRight.png", "dance", 8, 19, 0, 10, 6, 9, CColor::DarkMagenta());

	// rigby left
	rigby.AddImage("rigbyLeft.png", "leftNeutral", 8, 19, 0, 17, 0, 17, CColor::DarkMagenta());
	rigby.AddImage("rigbyLeft.png", "leftRun", 8, 19, 4, 15, 7, 15, CColor::DarkMagenta());
	rigby.AddImage("rigbyLeft.png", "leftJump", 8, 19, 0, 13, 0, 13, CColor::DarkMagenta());
	rigby.AddImage("rigbyLeft.png", "leftFall", 8, 19, 2, 13, 2, 13, CColor::DarkMagenta());
	rigby.AddImage("rigbyLeft.png", "leftHurt", 8, 19, 2, 11, 2, 11, CColor::DarkMagenta());

	// mordecai right
	mordecai.AddImage("mordecaiRight.png", "rightNeutral", 8, 17, 0, 15, 0, 15, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiRight.png", "rightRun", 8, 17, 0, 13, 5, 13, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiRight.png", "rightJump", 8, 17, 0, 12, 0, 12, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiRight.png", "rightFall", 8, 17, 5, 12, 5, 12, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiRight.png", "rightHurt", 8, 17, 2, 10, 2, 10, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiRight.png", "dance", 8, 17, 0, 9, 6, 8, CColor::DarkMagenta());

	// mordecai left
	mordecai.AddImage("mordecaiLeft.png", "leftNeutral", 8, 17, 0, 15, 0, 15, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiLeft.png", "leftRun", 8, 17, 0, 13, 5, 13, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiLeft.png", "leftJump", 8, 17, 0, 12, 0, 12, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiLeft.png", "leftFall", 8, 17, 5, 12, 5, 12, CColor::DarkMagenta());
	mordecai.AddImage("mordecaiLeft.png", "leftHurt", 8, 17, 2, 10, 2, 10, CColor::DarkMagenta());

	// top bar
	topBarCashIcon.AddImage("cash.png", "cash", 10, 1, 0, 0, 10, 0, CColor::DarkMagenta());
	topBarMordecaiLives.AddImage("mordecaiLives.png", "lives", 8, 1, 0, 0, 8, 0, CColor::DarkMagenta());
	topBarRigbyLives.AddImage("rigbyLives.png", "lives", 8, 1, 0, 0, 8, 0, CColor::DarkMagenta());
	topBarVHS.AddImage("vhsSmall.png", "vhs", 8, 1, 0, 0, 8, 0, CColor::DarkMagenta());
}

void CMyGame::OnDisplayMenu() {
	music.Play("menu.wav", -1);
	if (level < 1) {
		levelOneText.SetAnimation("text");
		levelTwoText.SetAnimation("text");
	}
}

void CMyGame::SnailControl() {
	Uint32 t = GetTime();

	for (CSprite* snail : snailList) {
		if (snail->GetHealth() == 0) {
			snail->SetHealth(-1);
			snail->SetYVelocity(200);
		}
		if (snail->GetHealth() < 0) {
			snail->SetYVelocity(snail->GetYVelocity()-25);
			if (snail->GetTop() < 0) {
				snail->Delete();
			}
		}
		if (snail->GetHealth() > 0) {
			for (CSprite* platform : platformList) {
				if (snail->HitTest(platform)) {
					if (snail->GetStatus() == 1 && snail->GetRight()+2 > platform->GetRight()) {
						snail->SetStatus(0);
						snail->SetAnimation("left", 3);
					} else if (snail->GetStatus() == 0 && snail->GetLeft() - 2 < platform->GetLeft()) {
						snail->SetStatus(1);
						snail->SetAnimation("right", 3);
					}
					if (snail->GetStatus() == 1 && snail->GetXVelocity() != 50) {
						snail->SetXVelocity(50);
					} else if (snail->GetStatus() == 0 && snail->GetXVelocity() != -50) {
					    snail->SetXVelocity(-50);
					}
					break;
				}
			}
		}

		if (!snail->IsDeleted()) snail->Update(t);
	}
	snailList.delete_if(deleted);
}

void CMyGame::PlayerControl() {
	int jumpSpeed = 450;
	int slowDown = 15;

	enum { neutral, run } pState;

	if (levelCompleted && timeCompleted < 300) timeCompleted++;
	if (timeCompleted == 300) {
		music.Stop();
		music.Play("continue.wav", -1);
	}

	// set status and animation
	if (!isHurt && !levelCompleted) {
		if (!(IsKeyDown(SDLK_RIGHT) && IsKeyDown(SDLK_LEFT))) {
			if (IsKeyDown(SDLK_RIGHT) && hitbox.GetXVelocity() < 1) {
				if (!facingRight) {
					if (hitbox.GetYVelocity() == 0) {
						mordecai.SetAnimation("rightRun", 12);
						rigby.SetAnimation("rightRun", 12);
					}
					facingRight = true;
				} else if (onPlatform && hitbox.GetStatus() != run && hitbox.GetYVelocity() == 0) {
					mordecai.SetAnimation("rightRun", 12);
					rigby.SetAnimation("rightRun", 12);
				}
				hitbox.SetStatus(run);
			}
			else if (IsKeyDown(SDLK_LEFT) && hitbox.GetXVelocity() > -1) {
				if (facingRight) {
					if (hitbox.GetYVelocity() == 0) {
						mordecai.SetAnimation("leftRun", 12);
						rigby.SetAnimation("leftRun", 12);
					}
					facingRight = false;
				} else if (onPlatform && hitbox.GetStatus() != run && hitbox.GetYVelocity() == 0) {
					mordecai.SetAnimation("leftRun", 12);
					rigby.SetAnimation("leftRun", 12);
				}
				hitbox.SetStatus(run);
			}
		}
		if (IsKeyDown(SDLK_UP) && onPlatform) {
			jumpSFX.Play("jump.wav");
			jumps++;
			hitbox.SetYVelocity(hitbox.GetYVelocity() + jumpSpeed);
			rigby.SetAnimation(facingRight ? "rightJump" : "leftJump");
			mordecai.SetAnimation(facingRight ? "rightJump" : "leftJump");
			onPlatform = 0;
		}
		else if (!IsKeyDown(SDLK_RIGHT) && !IsKeyDown(SDLK_LEFT) && hitbox.GetStatus() == run) {
			hitbox.SetStatus(neutral);
			if (onPlatform) {
				mordecai.SetAnimation(facingRight ? "rightNeutral" : "leftNeutral");
				rigby.SetAnimation(facingRight ? "rightNeutral" : "leftNeutral");
			}
			else {
				if (hitbox.GetYVelocity() > slowDown) {
					mordecai.SetAnimation(facingRight ? "rightJump" : "leftJump");
					rigby.SetAnimation(facingRight ? "rightJump" : "leftJump");
				}
				else {
					mordecai.SetAnimation(facingRight ? "rightFall" : "leftFall");
					rigby.SetAnimation(facingRight ? "rightFall" : "leftFall");
				}
			}
		}
	}

	// movement
	if (((hitbox.GetX() > 12420 && level == 1) || (hitbox.GetX() > 9640 && level == 2)) && !levelCompleted) {
		music.Stop();
		music.Play("victory.wav");
		levelCompleted = true;
		hitbox.SetXVelocity(0);
		rigby.SetAnimation("dance", 8);
		mordecai.SetAnimation("dance", 8);
	}
	if (lives < 0) {
		music.Stop();
		GameOver();
		music.Play("gameOver.wav");
	}

	if (!isHurt && !levelCompleted) {
		if (hitbox.GetStatus() == run) hitbox.SetXVelocity(facingRight ? 300 : -300);
		else hitbox.SetXVelocity(0);
	}

	for (CSprite* thisCash : cashList) {
		if (thisCash->GetRight() > abs(xScrollPos) && thisCash->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(thisCash)) {
			cashSFX.Play("cash.wav");
			cash++;
			thisCash->Delete();
		}
	}
	cashList.delete_if(deleted);

	for (CSprite* life : lifeList) {
		if (life->GetRight() > abs(xScrollPos) && life->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(life)) {
			lifeSFX.Play("life.wav");
			lives++;
			life->Delete();
		}
	}
	lifeList.delete_if(deleted);

	for (CSprite* vhs : tapeList) {
		if (vhs->GetRight() > abs(xScrollPos) && vhs->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(vhs)) {
			vhsSFX.Play("vhs.wav");
			tapes++;
			vhs->Delete();
		}
	}
	tapeList.delete_if(deleted);

	if (!onPlatform) {
		if (falling && isHurt) isHurt = false;
		if (!falling && hitbox.GetYVelocity() < 0) {
			falling = true;
			if (!levelCompleted) {
				mordecai.SetAnimation(facingRight ? "rightFall" : "leftFall");
				rigby.SetAnimation(facingRight ? "rightFall" : "leftFall");
			}
		}
		hitbox.SetYVelocity(hitbox.GetYVelocity()-slowDown);
		if (falling) {
			if (hitbox.GetTop() < 0) {
				hurtSFX.Play("hurt.wav");
				lives--;
				hitbox.SetPos(60, 240);
			}

			float bottomBefore = hitbox.GetBottom();
			float yBefore = hitbox.GetY();
			hitbox.SetY(yBefore+hitbox.GetYVelocity()/30);
			for (CSprite* platform : platformList) {
				if (platform->GetRight() > abs(xScrollPos) && platform->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(platform) && bottomBefore > platform->GetTop()) {
					hitbox.SetYVelocity(0);
					hitbox.SetY(platform->GetTop() + hitbox.GetHeight() / 2);
					onPlatform = 1;
					falling = false;
					jumps = 0;
					if (hitbox.GetStatus() != run && !levelCompleted) {
						mordecai.SetAnimation(facingRight ? "rightNeutral" : "leftNeutral");
						rigby.SetAnimation(facingRight ? "rightNeutral" : "leftNeutral");
					}
					else if (!levelCompleted) {
						mordecai.SetAnimation(facingRight ? "rightRun" : "leftRun");
						rigby.SetAnimation(facingRight ? "rightRun" : "leftRun");
					}
					break;
				}
			}
			if (falling) hitbox.SetY(yBefore);
		}
		if (falling) {
			for (CSprite* bush : bushList) {
				if (bush->GetRight() > abs(xScrollPos) && bush->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(bush) && hitbox.GetBottom()+slowDown > bush->GetTop()) {
					hitbox.SetYVelocity(0);
					hitbox.SetY(bush->GetTop() + hitbox.GetHeight()/2 + 1);
					onPlatform = 2;
					falling = false;
					jumps = 0;
					if (hitbox.GetStatus() != run && !levelCompleted) {
						mordecai.SetAnimation(facingRight ? "rightNeutral" : "leftNeutral");
						rigby.SetAnimation(facingRight ? "rightNeutral" : "leftNeutral");
					} else if (!levelCompleted) {
						mordecai.SetAnimation(facingRight ? "rightRun" : "leftRun");
						rigby.SetAnimation(facingRight ? "rightRun" : "leftRun");
					}
					break;
				}
			}
		}
		if (falling) {
			for (CSprite* snail : snailList) {
				if (snail->GetRight() > abs(xScrollPos) && snail->GetLeft() < abs(xScrollPos)+1280 && snail->GetHealth() > 0 && hitbox.HitTest(snail)) {
					CSprite* thisCash = new CSprite(snail->GetX(), snail->GetTop()-snail->GetHeight()/4, 0, 0, GetTime());
					thisCash->LoadAnimation("cash.png", "cash", CSprite::Sheet(10, 1).Row(0).From(0).To(9), CColor::DarkMagenta());
					thisCash->SetAnimation("cash");
					cashList.push_back(thisCash);

					stompSFX.Play("stomp.wav");
					snail->SetAnimation("dead");
					snail->SetHealth(0);
					hitbox.SetYVelocity(300);
					falling = false;
					jumps = 0;
					if (!levelCompleted) {
						mordecai.SetAnimation(facingRight ? "rightJump" : "leftJump");
						rigby.SetAnimation(facingRight ? "rightJump" : "leftJump");
					}
					break;
				}
			}
		}
		if (!falling) {
			if (hitbox.GetBottom() + hitbox.GetYVelocity()/30 > 718) {
				hitbox.SetYVelocity(0);
				hitbox.SetY(715+hitbox.GetHeight()/2);
			} else {
				for (CSprite* platform : platformList) {
					if (platform->GetRight() > abs(xScrollPos) && platform->GetLeft() < abs(xScrollPos) + 1280 && hitbox.HitTest(platform) && hitbox.GetTop() - hitbox.GetYVelocity() / 15 < platform->GetBottom()) {
						hitbox.SetYVelocity(0);
						hitbox.SetY(platform->GetBottom() - hitbox.GetHeight() / 2);
						break;
					}
				}
			}
		}
	} else {
		hitbox.SetY(hitbox.GetY() - 2);
		onPlatform = 0;
		for (CSprite* platform : platformList) {
			if (platform->GetRight() > abs(xScrollPos) && platform->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(platform)) {
				onPlatform = 1;
				break;
			}
		}
		if (onPlatform) hitbox.SetY(hitbox.GetY() + 2);		
		else {
			for (CSprite* bush : bushList) {
				if (bush->GetRight() > abs(xScrollPos) && bush->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(bush) && hitbox.GetBottom()+2 > bush->GetTop()) {
					onPlatform = 2;
					break;
				}
			}
			if (onPlatform) hitbox.SetY(hitbox.GetY() + 2);
		}
	}

	if (onPlatform) {
		for (CSprite* snail : snailList) {
			if (snail->GetRight() > abs(xScrollPos) && snail->GetLeft() < abs(xScrollPos)+1280 && snail->GetHealth() > 0 && hitbox.HitTest(snail)) {
				isHurt = true;
				hitbox.SetStatus(neutral);
				if (facingRight) {
					if (!levelCompleted) {
						mordecai.SetAnimation("rightHurt");
						rigby.SetAnimation("rightHurt");
					}
					hitbox.SetVelocity(-200, 300);
				} else {
					if (!levelCompleted) {
						mordecai.SetAnimation("leftHurt");
						rigby.SetAnimation("leftHurt");
					}
					hitbox.SetVelocity(200, 300);
				}
				hurtSFX.Play("hurt.wav");
				lives--;
			}
		}
	}

	if (hitbox.GetXVelocity() != 0) {
		float xBefore = hitbox.GetX();
		hitbox.SetX(xBefore+hitbox.GetXVelocity()/30);
		if (hitbox.GetLeft() < 0) {
			hitbox.SetX(xBefore);
			hitbox.SetXVelocity(0);
		} else {
			for (CSprite* platform : platformList) {
				if (platform->GetRight() > abs(xScrollPos) && platform->GetLeft() < abs(xScrollPos) + 1280 && hitbox.HitTest(platform)) {
					hitbox.SetX(hitbox.GetXVelocity() > 0 ? platform->GetLeft() - hitbox.GetWidth() / 2 : platform->GetRight() + hitbox.GetWidth() / 2);
					hitbox.SetXVelocity(0);
					break;
				}
			}
		}
		if (hitbox.GetXVelocity() != 0) hitbox.SetX(xBefore);
	}


	if (isMordecai) mordecai.SetPos(hitbox.GetX(), hitbox.GetY() - hitbox.GetHeight()/2 + mordecai.GetHeight()/2);
	else rigby.SetPos(hitbox.GetX(), hitbox.GetY() - hitbox.GetHeight()/2 + rigby.GetHeight()/2);

	hitbox.Update(GetTime());
	(isMordecai ? mordecai : rigby).Update(GetTime());
}

void CMyGame::SetupLevel() {
	int type, x, y, w, h;
	fstream file;
	if (level == 1) file.open("level1.txt", ios_base::in);
	else if (level == 2) file.open("level2.txt", ios_base::in);
	bool notEOF;
	Uint32 t = GetTime();
	do {
		file >> type >> x >> y >> w >> h;
		notEOF = file.good();
		if (notEOF) {
			if (type == 0) { // PLATFORMS
				CSpriteRect* platform = new CSpriteRect(x, y, w, h, CColor::Black(), t);
				platformList.push_back(platform);
			} else if (type == 1) { // BUSHES
				CSpriteRect* bush = new CSpriteRect(x, y, w, h, CColor::LightGreen(), t);
				bushList.push_back(bush);
			} else if (type == 2) { // CASH
				CSprite* thisCash = new CSprite(x, y, w, h, t);
				thisCash->LoadAnimation("cash.png", "cash", CSprite::Sheet(10, 1).Row(0).From(0).To(9), CColor::DarkMagenta());
				thisCash->SetAnimation("cash");
				cashList.push_back(thisCash);
			} else if (type == 3) { // LIVES
				CSprite* life = new CSprite(x, y, w, h, t);
				life->LoadAnimation("mordecaiLives.png", "mordecai", CSprite::Sheet(8, 1).Row(0).From(0).To(7), CColor::DarkMagenta());
				life->LoadAnimation("rigbyLives.png", "rigby", CSprite::Sheet(8, 1).Row(0).From(0).To(7), CColor::DarkMagenta());
				life->SetAnimation(isMordecai ? "mordecai" : "rigby");
				lifeList.push_back(life);
			} else if (type == 4) { // VHS TAPES
				CSprite* vhs = new CSprite(x, y, w, h, t);
				vhs->LoadAnimation("vhs.png", "vhs", CSprite::Sheet(8, 1).Row(0).From(0).To(7), CColor::DarkMagenta());
				vhs->SetAnimation("vhs");
				tapeList.push_back(vhs);
				totalTapes++;
			} else if (type == 5) { // SNAIL
				CSprite* snail = new CSprite(x, y, w, h, t);
				snail->LoadAnimation("snail.png", "right", CSprite::Sheet(3, 3).Row(2).From(0).To(2), CColor::DarkMagenta());
				snail->LoadAnimation("snail.png", "left", CSprite::Sheet(3, 3).Row(0).From(0).To(2), CColor::DarkMagenta());
				snail->LoadAnimation("snail.png", "dead", CSprite::Sheet(3, 3).Row(1).From(0).To(0), CColor::DarkMagenta());
				snail->SetAnimation("right", 3);
				snail->SetHealth(1);
				snail->SetStatus(1);
				snailList.push_back(snail);
			}
		}
	} while (notEOF);
	file.close();
}

void CMyGame::OnStartGame() {
	hitbox.SetPos(64, 240);
	cash = 0;
	tapes = 0;
	totalTapes = 0;
	platformList.delete_all();
	bushList.delete_all();
	cashList.delete_all();
	lifeList.delete_all();
	tapeList.delete_all();
	snailList.delete_all();

	SetupLevel();
	hitbox.SetStatus(0); // neutral
	mordecai.SetAnimation("rightNeutral");
	rigby.SetAnimation("rightNeutral");
	topBarCashIcon.SetAnimation("cash");
	topBarMordecaiLives.SetAnimation("lives");
	topBarRigbyLives.SetAnimation("lives");
	topBarVHS.SetAnimation("vhs");

	if (lives < 0 || level == 1) lives = 1;

	if (level == 1) music.Play("level1.wav", -1);
	else if (level == 2) music.Play("level2.wav", -1);
}

void CMyGame::OnStartLevel(Sint16 nLevel) {
	
}

void CMyGame::OnGameOver() {

}

void CMyGame::OnTerminate() {

}

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	/*
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
	*/
	if (IsGameMode()) {
		if (sym == SDLK_F1) debugMode = !debugMode;
		if (sym == SDLK_z && !levelCompleted) {
			float beforeY = hitbox.GetY();
			float beforeBottom = hitbox.GetBottom();
			CVector beforeSize = hitbox.GetSize();

			isMordecai = !isMordecai;
			if (isMordecai) hitbox.SetSize(45, 94);
			else hitbox.SetSize(45, 64);
			hitbox.SetY(beforeBottom + hitbox.GetHeight() / 2);
			for (CSprite* platform : platformList) {
				if (platform->GetRight() > abs(xScrollPos) && platform->GetLeft() < abs(xScrollPos)+1280 && hitbox.HitTest(platform)) {
					isMordecai = !isMordecai;
					hitbox.SetY(beforeY);
					hitbox.SetSize(beforeSize);
					break;
				}
			}
			if (hitbox.GetSize() != beforeSize) {
				switchSFX.Play("switch.wav");
				for (CSprite* life : lifeList) {
					life->SetAnimation(isMordecai ? "mordecai" : "rigby");
				}
			}
		}
		if (sym == SDLK_UP && !isHurt) {
			if (!onPlatform && jumps == 0) {
				jumpSFX.Play("jump.wav");
				jumps = 1;
				hitbox.SetYVelocity(500);
				falling = false;
				if (!levelCompleted) {
					mordecai.SetAnimation(facingRight ? "rightJump" : "leftJump");
					rigby.SetAnimation(facingRight ? "rightJump" : "leftJump");
				}
			} else if (isMordecai && jumps == 1) {
				jumpSFX.Play("jump.wav");
				jumps = 2;
				hitbox.SetYVelocity(500);
				falling = false;
				if (!levelCompleted) mordecai.SetAnimation(facingRight ? "rightJump" : "leftJump");
			}
		}
		if (sym == SDLK_DOWN && onPlatform == 2) {
			hitbox.SetY(hitbox.GetY() - 16);
			falling = true;
			onPlatform = 0;
			if (!levelCompleted) {
				mordecai.SetAnimation(facingRight ? "rightFall" : "leftFall");
				rigby.SetAnimation(facingRight ? "rightFall" : "leftFall");
			}
		}
	}
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {

}

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle) {
	if (IsMenuMode() && level < 1) {
		if (levelOneText.HitTest(x, y)) levelOneText.SetAnimation("hover");
		else levelOneText.SetAnimation("text");

		if (levelTwoText.HitTest(x, y)) levelTwoText.SetAnimation("hover");
		else levelTwoText.SetAnimation("text");
	}
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y) {
	if (IsMenuMode() && level < 1) {
		if (levelOneText.HitTest(x, y)) {
			music.Stop();
			level = 1;;
			StartGame();
		} else if (levelTwoText.HitTest(x, y)) {
			music.Stop();
			level = 2;
			StartGame();
		}
	} else if (timeCompleted == 300) {
		levelCompleted = false;
		timeCompleted = 0;
		if (level == 1) {
			level = 2;
			StartGame();
		} else if (level == 2) {
			level = 0;
			ChangeMode(MODE_MENU);
		}
	}
	else if (IsGameOver()) {
		level = 0;
		ChangeMode(MODE_MENU);
	}
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y) {

}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y) {

}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y) {

}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y) {

}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y) {

}
