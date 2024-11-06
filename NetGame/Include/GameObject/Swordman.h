#pragma once
#include "Player.h"

class CSwordman : public CPlayer
{
public:
	CSwordman() {}
	virtual ~CSwordman() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

};

