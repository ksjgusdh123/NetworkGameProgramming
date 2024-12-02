#pragma once
#include "GameObject.h"
#include "Player.h"

class CHPBar : public CGameObject
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

	void SetOwner(const CGameObject* owner) { m_owner = owner; }
private:
	const CGameObject* m_owner;
	RECT m_rect = { 0, 0, 80, 8 };
};

