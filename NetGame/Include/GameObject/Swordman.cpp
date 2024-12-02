#include "Swordman.h"
#include "..\PlayerAnimation.h"

bool CSwordman::Init()
{
	CPlayer::Init();

	m_attackPivot = Vector2(0.5f, 0.7f);
	m_attackSize = Vector2(2.f, 1.5f);

	SetAnimation(SWORD_BASIC, 8, EObject_State::Basic);
	SetAnimation(SWORD_BASIC_L, 8, EObject_State::Basic_L);
	SetAnimation(SWORD_RUN_R, 8, EObject_State::Walk);
	SetAnimation(SWORD_RUN_L, 8, EObject_State::Walk_L);
	SetAnimation(SWORD_Jump_up_R, 2, EObject_State::Jump);
	SetAnimation(SWORD_Jump_up_L, 2, EObject_State::Jump_L);
	SetAnimation(SWORD_Jump_Down_R, 2, EObject_State::Jump_Down);
	SetAnimation(SWORD_Jump_Down_L, 2, EObject_State::Jump_Down_L);
	SetAnimation(ATTACK_R, 10, EObject_State::Attack);
	SetAnimation(ATTACK_L, 10, EObject_State::Attack_L);
	SetAnimation(DAMAGE_SWORD_R, 9, EObject_State::Damaged);
	SetAnimation(DAMAGE_SWORD_L, 9, EObject_State::Damaged_L);
	SetAnimation(DIE_SWORD_R, 5, EObject_State::Die);
	SetAnimation(DIE_SWORD_L, 5, EObject_State::Die_L);

	return true;
}

void CSwordman::Update(float elapsedTime)
{
	CPlayer::Update(elapsedTime);
}

void CSwordman::PostUpdate(float elapsedTime)
{
	CPlayer::PostUpdate(elapsedTime);
}

void CSwordman::Render(HDC hDC, float elapsedTime)
{
	CPlayer::Render(hDC, elapsedTime);
}
