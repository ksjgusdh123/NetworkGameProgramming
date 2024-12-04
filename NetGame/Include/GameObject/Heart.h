#pragma once
#include "GameObject.h"
class CHeart : public CGameObject
{
public:
	CHeart() {}
	virtual ~CHeart() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;
};

