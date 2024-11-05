#pragma once
#include "GameObject.h"
#include "Flag.h"



class CGhost : public CGameObject
{
public:
	CGhost() {}
	virtual ~CGhost() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

	
	int m_hp;
	bool m_bIsAlive;

};

