#pragma once
#include "GameObject.h"
#include "Flag.h"

class CTile : public CGameObject
{
public:
	CTile() {}
	virtual ~CTile() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

};

