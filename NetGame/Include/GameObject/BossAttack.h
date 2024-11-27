#pragma once
#include "GameObject.h"
#include "Flag.h"

class CBossAttack : public CGameObject
{
public:
	CBossAttack() {}
	virtual ~CBossAttack() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;
	void SetTarget(Vector2 target) { m_target = target; }

	float m_timer;
	Vector2 m_target;
};

