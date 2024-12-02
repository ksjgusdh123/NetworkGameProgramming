#pragma once
#include "GameObject.h"
#include "Flag.h"
#include "HPBar.h"

class CRiche : public CGameObject
{
public:
	CRiche() {}
	virtual ~CRiche() {}

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

	void Attack(Vector2 target);
	void CreateHPBar(class CScene* scene);

	int m_hp;
	bool m_bIsAlive;
	Vector2 m_target;
	float m_timer;
	CHPBar* m_hpBar;
};

