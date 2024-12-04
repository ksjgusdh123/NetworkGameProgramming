#pragma once
#include "Player.h"
#include "Arrow.h"

class CArcher : public CPlayer
{
public:
	CArcher() {}
	virtual ~CArcher() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;
	virtual void InitInput();

public:
	virtual void CheckFrame(float elapsedTime);	

private:
	void ShotArrow();

public:
};

