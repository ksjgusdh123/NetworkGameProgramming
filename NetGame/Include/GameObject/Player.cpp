#include "Player.h"
#include "Engine.h"
#include "Input.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\PlayerAnimation.h"
#include "..\NetClient\TCPClient.h"
#include <Scene\Camera.h>
#include <Collision.h>

bool CPlayer::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(150.f, 150.f);
	CreateTexture(2);
	SetTexture("Player_L", TEXT("Player/player_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Player", TEXT("Player/player.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetSize(50.f, 60.f);
	m_objectState = EObject_State::Basic;
	return true;
}

void CPlayer::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);

	if (m_hp <= 0 || m_pos.y >= 600)
	{
		m_hp = 0;
		DieEvent();
	}

	if (m_jumpState == EJump_State::Jumping)
	{
		//CalculateJump(elapsedTime);
	}

}

void CPlayer::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CPlayer::Render(HDC hDC, float elapsedTime)
{
	if (!m_bRender)
		return;

//#ifdef DEBUG
//	m_collisionBox->Render(hDC, elapsedTime);
//#endif

	if (m_prevObjectState != m_objectState) 
	{
		m_idx = 0;
		m_time = 0;
	}

	m_prevObjectState = m_objectState;

	Vector2	pos;
	Vector2	cameraPos;
	Vector2	resolution;

	if (m_scene)
	{
		cameraPos = m_scene->GetCamera()->GetPos();
		resolution = m_scene->GetCamera()->GetResolution();
		pos = m_pos - m_scene->GetCamera()->GetPos();
	}

	if (m_texture)
	{
		Vector2	renderLT = pos - m_pivot * m_size;
		Vector2	cullPos = m_pos - m_pivot * m_size;

		if (cullPos.x > cameraPos.x + resolution.x)
			return;

		else if (cullPos.x + m_size.x < cameraPos.x)
			return;

		else if (cullPos.y > cameraPos.y + resolution.y)
			return;

		else if (cullPos.y + m_size.y < cameraPos.y)
			return;


		int stateIdx = 0;
		if (m_jumpState == EJump_State::Landed)
			stateIdx = (int)m_objectState;
		else if (m_jumpState == EJump_State::Jumping)
		{
			if (m_objectDir == EObject_Dir::Right)
				stateIdx = (int)EObject_State::Jump;
			else
				stateIdx = (int)EObject_State::Jump_L;
		}
		else if (m_jumpState == EJump_State::JumpDown)
		{
			if (m_objectDir == EObject_Dir::Right)
				stateIdx = (int)EObject_State::Jump_Down;
			else
				stateIdx = (int)EObject_State::Jump_Down_L;
		}

		if (m_objectState != EObject_State::Die && m_objectState != EObject_State::Die_L)
		{
			if (m_texture[(int)m_objectDir]->GetTextureType() == ETexture_Type::CIMAGE)
			{
				m_time += elapsedTime;
				m_idx = (((int)(m_time * m_animationBox[(int)stateIdx].size())) % m_animationBox[(int)stateIdx].size());
				Vector2 size{ (float)m_animationBox[(int)stateIdx][m_idx].right, (float)m_animationBox[(int)stateIdx][m_idx].bottom };
				renderLT = pos - m_pivot * size;
				m_texture[(int)m_objectDir]->GetCImage().Draw(hDC, (int)renderLT.x, (int)renderLT.y, m_animationBox[(int)stateIdx][m_idx].right, m_animationBox[(int)stateIdx][m_idx].bottom, m_animationBox[(int)stateIdx][m_idx].left, m_animationBox[(int)stateIdx][m_idx].top, m_animationBox[(int)stateIdx][m_idx].right, m_animationBox[(int)stateIdx][m_idx].bottom);
			}
		}
		else
		{
			m_time += elapsedTime;
			m_idx = min(((int)(m_time * m_animationBox[(int)stateIdx].size())), 4);
			Vector2 size{ (float)m_animationBox[(int)stateIdx][m_idx].right, (float)m_animationBox[(int)stateIdx][m_idx].bottom };
			renderLT = pos - m_pivot * size;
			m_texture[(int)m_objectDir]->GetCImage().Draw(hDC, (int)renderLT.x, (int)renderLT.y, m_animationBox[(int)stateIdx][m_idx].right, m_animationBox[(int)stateIdx][m_idx].bottom, m_animationBox[(int)stateIdx][m_idx].left, m_animationBox[(int)stateIdx][m_idx].top, m_animationBox[(int)stateIdx][m_idx].right, m_animationBox[(int)stateIdx][m_idx].bottom);
		}
	}
}

void CPlayer::InitInput()
{
	CInput::GetInst()->AddBindFunction<CPlayer>("Attack", EInput_Type::PUSH, this, &CPlayer::PlayerAttack, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Jump", EInput_Type::PUSH, this, &CPlayer::PlayerJump, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", EInput_Type::PUSH, this, &CPlayer::PlayerMoveLeft, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", EInput_Type::UP, this, &CPlayer::PlayerLeftIdle, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", EInput_Type::PUSH, this, &CPlayer::PlayerMoveRight, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", EInput_Type::UP, this, &CPlayer::PlayerRightIdle, m_scene);
}

void CPlayer::PlayerMoveLeft()
{
	if (m_hp <= 0)
		return;

	switch (m_objectState)
	{
	case EObject_State::Basic:
	case EObject_State::Basic_L:
	case EObject_State::Walk:
	case EObject_State::Walk_L:
		m_objectState = EObject_State::Walk_L;
		break;
	case EObject_State::Attack:
	case EObject_State::Attack_L:
		return;
	}

	m_objectDir = EObject_Dir::Left;
		
}

void CPlayer::PlayerLeftIdle()
{
	if (m_hp <= 0)
		return;
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L)
		return;

	m_objectState = EObject_State::Basic_L;
	m_time = 0;
}

void CPlayer::PlayerMoveRight()
{
	if (m_hp <= 0)
		return;
	switch (m_objectState)
	{
	case EObject_State::Basic:
	case EObject_State::Basic_L:
	case EObject_State::Walk:
	case EObject_State::Walk_L:
		m_objectState = EObject_State::Walk;
		break;
	case EObject_State::Attack:
	case EObject_State::Attack_L:
		return;
	}

	m_objectDir = EObject_Dir::Right;
}

void CPlayer::PlayerRightIdle()
{
	if (m_hp <= 0)
		return;
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L)
		return;

	m_objectState = EObject_State::Basic;
	m_time = 0;
}

void CPlayer::PlayerAttack()
{
	if (m_hp <= 0)
		return;
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L)
		return;

	if (m_jumpState != EJump_State::Landed)
		return;

	//SetSize(m_size.x * m_attackSize.x, m_size.y * m_attackSize.y);
	//SetPivot(m_attackPivot);
	if (m_objectDir == EObject_Dir::Right)
	{
		m_objectState = EObject_State::Attack;
	}
	else
	{
		m_objectState = EObject_State::Attack_L;
	}
	m_frame = 1;
	m_nowFrame = 0;
	m_bFrameCheck = true;
	m_time = 0;
}

void CPlayer::PlayerJump()
{
	if (m_hp <= 0)
		return;

	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L)
		return;

	if (m_jumpState == EJump_State::Jumping)
		return;

	if (m_jumpState == EJump_State::JumpDown || (m_jumpState == EJump_State::Jumping && m_jumpTime >= 0.4f))
	{
		if (m_bDoubleJump)
			return;
		else
			m_bDoubleJump = true;
	}
	

	m_jumpState = EJump_State::Jumping;
	m_time = 0;
}

void CPlayer::CheckFrame(float elapsedTime)
{
	//m_nowFrame += elapsedTime;
	//if (m_nowFrame >= m_frame)
	//{
	//	if (m_objectDir == EObject_Dir::Right)
	//	{
	//		m_objectState = EObject_State::Basic;
	//	}
	//	else
	//	{
	//		m_objectState = EObject_State::Basic_L;
	//	}
	//	m_bFrameCheck = false;
	//}
}

void CPlayer::CalculateJump(float elapsedTime)
{
	// 임시적 수치와 종료 조건
	if (m_jumpTime >= 0.5)
	{
		m_jumpState = EJump_State::JumpDown;
		m_jumpTime = 0.f;
		return;
	}

	m_jumpTime += elapsedTime;
}

void CPlayer::DieEvent()
{
	if (m_objectState != EObject_State::Die || m_objectState != EObject_State::Die_L)
	{
		if (m_objectDir == EObject_Dir::Right)
		{
			m_objectState = EObject_State::Die;
		}
		else
		{
			m_objectState = EObject_State::Die_L;
		}
	}
}

void CPlayer::CreateHPBar(CScene* scene)
{
	m_hpBar = scene->CreateObject<CHPBar>("hpBar");
	m_hpBar->SetOwner((CGameObject*)this);
}
