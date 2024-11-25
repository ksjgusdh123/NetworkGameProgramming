#pragma once
#include "Player.h"

class CMyPlayer :public CPlayer
{
public:
	CMyPlayer() {}
	virtual ~CMyPlayer() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

protected:
	void PlayerMoveLeft();
	void PlayerLeftIdle();
	void PlayerMoveRight();
	void PlayerRightIdle();
	virtual void PlayerAttack();
	void PlayerJump();

	virtual void CheckFrame(float elapsedTime);
	void JumpDown();
	void CalculateJump(float elapsedTime);

	void SendMovePacket(const Vector2& pos);
};

