#pragma once
#include "GameObject.h"

class CHPBar : public CGameObject
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;

	void SetOwner(const CGameObject* owner) { m_owner = owner; }
	void SetBarSize(float w, float h);
private:
	const CGameObject* m_owner;
	float m_w = 70, m_h = 5;
};

