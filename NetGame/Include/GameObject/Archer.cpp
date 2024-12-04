#include "Archer.h"
#include "..\PlayerAnimation.h"
#include "Input.h"
#include <Scene/Scene.h>

bool CArcher::Init()
{
	CPlayer::Init();

	m_attackPivot = Vector2(0.5f, 0.5f);
	m_attackSize = Vector2(1.3f, 1.f);

	SetAnimation(Basic, 8, EObject_State::Basic);
	SetAnimation(Basic_L, 8, EObject_State::Basic_L);
	SetAnimation(RUN_R, 8, EObject_State::Walk);
	SetAnimation(RUN_L, 8, EObject_State::Walk_L);
	SetAnimation(Jump_up_R, 2, EObject_State::Jump);
	SetAnimation(Jump_up_L, 2, EObject_State::Jump_L);
	SetAnimation(Jump_down_R, 2, EObject_State::Jump_Down);
	SetAnimation(Jump_down_L, 2, EObject_State::Jump_Down_L);
	SetAnimation(ATTACK_A_R, 12, EObject_State::Attack);
	SetAnimation(ATTACK_A_L, 12, EObject_State::Attack_L);

	return true;
}

void CArcher::Update(float elapsedTime)
{
	CPlayer::Update(elapsedTime);
}

void CArcher::PostUpdate(float elapsedTime)
{
	CPlayer::PostUpdate(elapsedTime);
}

void CArcher::Render(HDC hDC, float elapsedTime)
{
	CPlayer::Render(hDC, elapsedTime);
}

void CArcher::InitInput()
{
	CPlayer::InitInput();
	CInput::GetInst()->AddBindFunction<CArcher>("Attack", EInput_Type::PUSH, this, &CArcher::PlayerAttack, m_scene);
}

void CArcher::CheckFrame(float elapsedTime)
{
	m_nowFrame += elapsedTime;
	if (m_nowFrame >= m_frame)
	{
		if (m_objectDir == EObject_Dir::Right)
		{
			m_objectState = EObject_State::Basic;
		}
		else
		{
			m_objectState = EObject_State::Basic_L;
		}
		SetSize(m_size.x / m_attackSize.x, m_size.y / m_attackSize.y);
		SetPivot(Vector2(0.5, 0.5f));
		m_bFrameCheck = false;
		//ShotArrow();
	}
}

void CArcher::ShotArrow()
{
}
