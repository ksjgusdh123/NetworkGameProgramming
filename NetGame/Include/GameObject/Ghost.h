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

	void SetRange(float range) { m_range = range; }

	int m_hp;
	bool m_bIsAlive;
	float m_range;
	float m_originalPosX;
};

