#pragma once
#include "GameObject.h"

class CArrow : public CGameObject
{
public:
	CArrow() {}
	virtual ~CArrow() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

	float m_timer;
};

