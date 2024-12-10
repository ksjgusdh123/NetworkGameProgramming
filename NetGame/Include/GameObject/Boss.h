#pragma once
#include "GameObject.h"
#include "Flag.h"
#include "HPBar.h"

class CBoss : public CGameObject
{
public:
	CBoss() {}
	virtual ~CBoss() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

	void Attack(Vector2 target);
	void CreateHPBar(CScene* scene);

	bool m_bIsAlive;
	Vector2 m_target;
	float m_timer;
	CHPBar* m_hpBar;
};

