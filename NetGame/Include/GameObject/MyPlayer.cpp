#include "MyPlayer.h"
#include "Engine.h"
#include "Input.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\PlayerAnimation.h"
#include "..\NetClient\TCPClient.h"

bool CMyPlayer::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(150.f, 300.f);
	CreateTexture(2);
	SetTexture("Player_L", TEXT("Player/player_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Player", TEXT("Player/player.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetSize(50.f, 60.f);

	CInput::GetInst()->AddBindFunction<CMyPlayer>("Attack", EInput_Type::PUSH, this, &CMyPlayer::PlayerAttack, m_scene);
	CInput::GetInst()->AddBindFunction<CMyPlayer>("Jump", EInput_Type::PUSH, this, &CMyPlayer::PlayerJump, m_scene);
	CInput::GetInst()->AddBindFunction<CMyPlayer>("MoveLeft", EInput_Type::PUSH, this, &CMyPlayer::PlayerMoveLeft, m_scene);
	CInput::GetInst()->AddBindFunction<CMyPlayer>("MoveLeft", EInput_Type::UP, this, &CMyPlayer::PlayerLeftIdle, m_scene);
	CInput::GetInst()->AddBindFunction<CMyPlayer>("MoveRight", EInput_Type::PUSH, this, &CMyPlayer::PlayerMoveRight, m_scene);
	CInput::GetInst()->AddBindFunction<CMyPlayer>("MoveRight", EInput_Type::UP, this, &CMyPlayer::PlayerRightIdle, m_scene);

	return true;
}

void CMyPlayer::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
	if (m_bFrameCheck)
	{
		CheckFrame(elapsedTime);
	}
	if (m_bJump)
	{
		CalculateJump(elapsedTime);
	}
}

void CMyPlayer::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CMyPlayer::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}

void CMyPlayer::PlayerMoveLeft()
{

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
	SendMovePacket(m_pos);
}

void CMyPlayer::PlayerLeftIdle()
{
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L || m_objectState == EObject_State::Jump ||
		m_objectState == EObject_State::Jump_L || m_objectState == EObject_State::Jump_Down_L || m_objectState == EObject_State::Jump_Down)
		return;

	m_objectState = EObject_State::Basic_L;
	m_time = 0;
}

void CMyPlayer::PlayerMoveRight()
{
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
	SendMovePacket(m_pos);
}

void CMyPlayer::PlayerRightIdle()
{
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L || m_objectState == EObject_State::Jump ||
		m_objectState == EObject_State::Jump_L || m_objectState == EObject_State::Jump_Down || m_objectState == EObject_State::Jump_Down_L)
		return;

	m_objectState = EObject_State::Basic;
	m_time = 0;
}

void CMyPlayer::PlayerAttack()
{
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

void CMyPlayer::PlayerJump()
{
	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L || m_objectState == EObject_State::Jump || m_objectState == EObject_State::Jump_L)
		return;

	if ((m_objectState == EObject_State::Jump_Down || m_objectState == EObject_State::Jump_Down_L) && !m_bDoubleJump)
	{
		m_bDoubleJump = true;
	}
	else if (m_objectState == EObject_State::Jump_Down || m_objectState == EObject_State::Jump_Down_L)
	{
		return;
	}
	else
		m_prevHeight = m_pos.y;

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

void CMyPlayer::CheckFrame(float elapsedTime)
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

void CMyPlayer::JumpDown()
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

void CMyPlayer::CalculateJump(float elapsedTime)
{
	// 임시적 수치와 종료 조건

	m_jumpTime += elapsedTime;
	m_pos.y -= elapsedTime * 100 * m_multipleNum;
	if (m_jumpTime >= 1)
	{
		JumpDown();
	}

	if (m_pos.y >= m_prevHeight)
	{
		m_pos.y = m_prevHeight;
		m_bJump = false;
		if (m_objectDir == EObject_Dir::Right)
		{
			m_objectState = EObject_State::Basic;
		}
		else
		{
			m_objectState = EObject_State::Basic_L;
		}
		m_bDoubleJump = false;
	}
}

void CMyPlayer::SendMovePacket(const Vector2& pos)
{
	C_PlayerMovePkt p(pos.x, pos.y);
	TCPClient::GetInst()->SendPacket(&p);
}
