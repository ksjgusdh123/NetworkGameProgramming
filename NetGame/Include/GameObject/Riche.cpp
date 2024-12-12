#include "Riche.h"
#include "RicheAttack.h"
#include "Engine.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\MonsterAnimation.h"

bool CRiche::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(50.f, 0.f);
	CreateTexture(4);
	SetTexture("Riche_L", TEXT("Monster/Riche_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Riche_R", TEXT("Monster/Riche_R.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetTexture("Riche_Attack_L", TEXT("Monster/Riche_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Riche_Attack_R", TEXT("Monster/Riche_R.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetAnimation(Riche_L, 6, EObject_State::Basic_L);
	SetAnimation(Riche_R, 6, EObject_State::Basic);
	SetAnimation(Riche_Attack_L, 18, EObject_State::Attack_L);
	SetAnimation(Riche_Attack_R, 18, EObject_State::Attack);
	SetAnimation(Riche_Die_R, 10, EObject_State::Die);
	SetAnimation(Riche_Die_L, 10, EObject_State::Die_L);
	SetSize(107.f, 139.f);
	SetColorKey(180, 213, 255);
	
	m_hp = 100;
	m_objectDir = EObject_Dir::Right;
	m_objectState = EObject_State::Basic;
	m_bIsAlive = true;
	m_timer = 0.f;
	return true;
}

void CRiche::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
	m_hpBar->Update(elapsedTime);

	//if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L) {
	//	m_timer += elapsedTime;

	//	if (m_timer >= 1.f) {
	//		if (m_objectState == EObject_State::Attack)
	//			m_objectState = EObject_State::Basic;
	//		if (m_objectState == EObject_State::Attack_L)
	//			m_objectState = EObject_State::Basic_L;

	//		CRicheAttack* ra = m_scene->CreateObject<CRicheAttack>("riche_attack");
	//		ra->SetPos(m_pos);
	//		ra->SetTarget(m_target);
	//	}
	//}

}

void CRiche::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CRiche::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
	m_hpBar->Render(hDC, elapsedTime);
}


void CRiche::Attack(Vector2 target)
{
	if (target.x >= m_pos.x) {
		m_objectState = EObject_State::Attack;
		m_objectDir = EObject_Dir::Right;
	}
	else{
		m_objectState = EObject_State::Attack_L;
		m_objectDir = EObject_Dir::Left;
	}
	m_target = target;
	m_timer = 0.f;
}

void CRiche::CreateHPBar(CScene* scene)
{
	m_hpBar = std::make_shared<CHPBar>();
	m_hpBar->SetOwner((CGameObject*)this);
}
