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

private:
	void PlayerMoveUp();
	void PlayerMoveDown();
	void PlayerMoveLeft();
	void PlayerMoveRight();

};

