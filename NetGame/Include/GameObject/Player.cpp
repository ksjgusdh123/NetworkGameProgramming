#include "Player.h"
#include "Engine.h"
#include "Input.h"
#include <Scene/Scene.h>

bool CPlayer::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.f);
	SetSize(50.f, 60.f);
	SetVelocity(100.f, 100.f);
	SetTexture("Player", TEXT("Player/Bazzi_Lobby.bmp"));
	SetColorKey(255, 0, 255);

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveUp", EInput_Type::PUSH, this, &CPlayer::PlayerMoveUp, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveDown", EInput_Type::PUSH, this, &CPlayer::PlayerMoveDown, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", EInput_Type::PUSH, this, &CPlayer::PlayerMoveLeft, m_scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", EInput_Type::PUSH, this, &CPlayer::PlayerMoveRight, m_scene);

	return true;
}

void CPlayer::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CPlayer::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CPlayer::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}

void CPlayer::PlayerMoveUp()
{
	m_pos.y -= m_velocity.y * 2 * ELAPSED_TIME;
	SetPos(m_pos);
}

void CPlayer::PlayerMoveDown()
{
	m_pos.y += m_velocity.y * 2 * ELAPSED_TIME;
	SetPos(m_pos);
}

void CPlayer::PlayerMoveLeft()
{
	m_pos.x -= m_velocity.x * 2 * ELAPSED_TIME;
	SetPos(m_pos);
}

void CPlayer::PlayerMoveRight()
{
	m_pos.x += m_velocity.x * 2 * ELAPSED_TIME;
	SetPos(m_pos);
}