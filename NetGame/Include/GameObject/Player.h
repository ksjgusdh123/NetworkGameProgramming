#pragma once
#include "GameObject.h"

class CPlayer :public CGameObject
{
public:
	CPlayer() {}
	virtual ~CPlayer() {}

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
protected:
	int m_frame = 0;
	float m_nowFrame = 0;
	bool m_bFrameCheck = false;

	float m_prevHeight = 0;
	float m_jumpTime = 0;
	bool m_bJump = false;
	bool m_bDoubleJump = false;
	int m_multipleNum = 1;

	Vector2 m_attackSize;
	Vector2 m_attackPivot;
};

