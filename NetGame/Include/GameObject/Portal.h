#pragma once
#include "GameObject.h"

class CPortal : public CGameObject
{
public:
	CPortal() {}
	virtual ~CPortal() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

};
