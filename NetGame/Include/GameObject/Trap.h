#pragma once
#include "GameObject.h"

class CTrap : public CGameObject
{
public:
	CTrap() {}
	virtual ~CTrap() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

};
