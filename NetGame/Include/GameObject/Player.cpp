#include "Player.h"
#include "Engine.h"
#include "Input.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\PlayerAnimation.h"
#include "..\NetClient\TCPClient.h"

bool CPlayer::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(150.f, 150.f);
	CreateTexture(2);
	SetTexture("Player_L", TEXT("Player/player_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Player", TEXT("Player/player.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetSize(50.f, 60.f);
	m_objectState = EObject_State::Jump_Down;
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

	if (!m_bIsLanded && (m_objectState != EObject_State::Basic && m_objectState != EObject_State::Basic_L
		&& m_objectState != EObject_State::Jump && m_objectState != EObject_State::Jump_L))
	{
		if (m_objectDir == EObject_Dir::Right)
			m_objectState = EObject_State::Jump_Down;
		else if (m_objectDir == EObject_Dir::Left)
			m_objectState = EObject_State::Jump_Down_L;
		m_pos.y += elapsedTime * (m_velocity.y);
	}

	if (m_bFrameCheck)
	{
		CheckFrame(elapsedTime);
	}
	if (m_bJump)
	{
		CalculateJump(elapsedTime);
	}

}

void CPlayer::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CPlayer::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
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
	case EObject_State::Jump:
		m_objectState = EObject_State::Jump_L;
		break;
	case EObject_State::Jump_Down:
		m_objectState = EObject_State::Jump_Down_L;
		break;
	case EObject_State::Attack:
	case EObject_State::Attack_L:
		return;
	}

	m_objectDir = EObject_Dir::Left;
	m_pos.x -= m_velocity.x * 2 * ELAPSED_TIME;
		
}

void CPlayer::PlayerLeftIdle()
{
	if (m_hp <= 0)
		return;
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L || m_objectState == EObject_State::Jump ||
		m_objectState == EObject_State::Jump_L || m_objectState == EObject_State::Jump_Down_L || m_objectState == EObject_State::Jump_Down)
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
	case EObject_State::Jump_L:
		m_objectState = EObject_State::Jump;
		break;
	case EObject_State::Jump_Down_L:
		m_objectState = EObject_State::Jump_Down;
		break;
	case EObject_State::Attack:
	case EObject_State::Attack_L:
		return;
	}

	m_objectDir = EObject_Dir::Right;
	m_pos.x += m_velocity.x * 2 * ELAPSED_TIME;
}

void CPlayer::PlayerRightIdle()
{
	if (m_hp <= 0)
		return;
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L || m_objectState == EObject_State::Jump ||
		m_objectState == EObject_State::Jump_L || m_objectState == EObject_State::Jump_Down || m_objectState == EObject_State::Jump_Down_L)
		return;

	m_objectState = EObject_State::Basic;
	m_time = 0;
}

void CPlayer::PlayerAttack()
{
	if (m_hp <= 0)
		return;
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L || m_objectState == EObject_State::Jump ||
		m_objectState == EObject_State::Jump_L || m_objectState == EObject_State::Jump_Down || m_objectState == EObject_State::Jump_Down_L)
		return;

	SetSize(m_size.x * m_attackSize.x, m_size.y * m_attackSize.y);
	SetPivot(m_attackPivot);
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

	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L ||
		m_objectState == EObject_State::Jump || m_objectState == EObject_State::Jump_L)
		return;


	if ((m_objectState == EObject_State::Jump_Down || m_objectState == EObject_State::Jump_Down_L) && m_bJump && !m_bDoubleJump)
	{
		m_bDoubleJump = true;
	}
	else if (m_objectState == EObject_State::Jump_Down || m_objectState == EObject_State::Jump_Down_L)
	{
		return;
	}
	else
		m_prevHeight = m_pos.y;

	m_pos.y -= 3.f;
	m_bIsLanded = false;

	m_multipleNum = 1;
	m_jumpTime = 0;
	m_bJump = true;

	if (m_objectDir == EObject_Dir::Right)
	{
		m_objectState = EObject_State::Jump;
	}
	else
	{
		m_objectState = EObject_State::Jump_L;
	}
	m_time = 0;
}

void CPlayer::CheckFrame(float elapsedTime)
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
	}
}

void CPlayer::JumpDown()
{
	if (m_objectDir == EObject_Dir::Right)
	{
		m_objectState = EObject_State::Jump_Down;
	}
	else
	{
		m_objectState = EObject_State::Jump_Down_L;
	}
	m_jumpTime = 0;
	m_multipleNum = -1;
}

void CPlayer::CalculateJump(float elapsedTime)
{
	// 임시적 수치와 종료 조건
	if (m_objectState != EObject_State::Jump_Down && m_objectState != EObject_State::Jump_Down_L &&
		m_objectState != EObject_State::Jump && m_objectState != EObject_State::Jump_L)
	{
		m_bJump = false;
		return;
	}

	if (m_jumpTime >= 0.5)
	{
		if (m_objectDir == EObject_Dir::Right)
		{
			m_objectState = EObject_State::Jump_Down;
		}
		else
		{
			m_objectState = EObject_State::Jump_Down_L;
		}
		return;
		//m_bJump = false;
		//m_bDoubleJump = false;
	}

	m_jumpTime += elapsedTime;
	m_pos.y -= elapsedTime * m_velocity.y * m_multipleNum;



	//if (m_pos.y >= m_prevHeight)
	//{
	//	m_pos.y = m_prevHeight;
	//	m_bJump = false;
	//	if (m_objectDir == EObject_Dir::Right)
	//	{
	//		m_objectState = EObject_State::Basic;
	//	}
	//	else
	//	{
	//		m_objectState = EObject_State::Basic_L;
	//	}
	//	m_bDoubleJump = false;
	//}
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
