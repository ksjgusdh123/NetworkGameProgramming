#include "Player.h"
#include "Engine.h"
#include "Input.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\PlayerAnimation.h"

bool CPlayer::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(150.f, 300.f);
	CreateTexture(2);
	SetTexture("Player_L", TEXT("Player/player_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Player", TEXT("Player/player.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetAnimation(Basic, 8, EObject_State::Basic_L);
	SetAnimation(Basic_L, 8, EObject_State::Basic);
	SetSize(150.f, 300.f);
	//SetColorKey(255, 0, 255);

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
	m_objectDir = EObject_Dir::Left;
	m_objectState = EObject_State::Basic_L;
	m_pos.x -= m_velocity.x * 2 * ELAPSED_TIME;
	SetPos(m_pos);
}

void CPlayer::PlayerMoveRight()
{	
	m_objectDir = EObject_Dir::Right;
	m_objectState = EObject_State::Basic;
	m_pos.x += m_velocity.x * 2 * ELAPSED_TIME;
	SetPos(m_pos);
}