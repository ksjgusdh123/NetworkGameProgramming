#include "Player.h"
#include "Engine.h"
#include "Input.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
bool CPlayer::Init()
{
	CGameObject::Init();
	RECT Boss_Growl_L[16] = {
		{8, 619, 155, 153},
		{352, 616, 155, 156},
		{698, 590, 156, 182},
		{874, 591, 157, 181},
		{874, 591, 157, 181},
		{874, 591, 157, 181},
		{874, 591, 157, 181},
		{874, 591, 157, 181},
		{1050, 592, 158, 180},
		{1050, 592, 158, 180},
		{1050, 592, 158, 180},
		{1050, 592, 158, 180},
		{1050, 592, 158, 180},
		{1050, 592, 158, 180},
		{1050, 592, 158, 180},
		{1050, 592, 158, 180},
	};

	RECT Boss_Growl[16] = {
	{2955, 619, 155, 153},
	{2611, 616, 155, 156},
	{2264, 590, 156, 182},
	{2087, 591, 157, 181},
	{2087, 591, 157, 181},
	{2087, 591, 157, 181},
	{2087, 591, 157, 181},
	{2087, 591, 157, 181},
	{1910, 592, 158, 180},
	{1910, 592, 158, 180},
	{1910, 592, 158, 180},
	{1910, 592, 158, 180},
	{1910, 592, 158, 180},
	{1910, 592, 158, 180},
	{1910, 592, 158, 180},
	{1910, 592, 158, 180},
	};



	SetPivot(0.5f, 0.5f);
	SetVelocity(150.f, 300.f);
	CreateTexture(2);
	SetTexture("Player_L", TEXT("Player/boss.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Player", TEXT("Player/boss_L.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetAnimation(Boss_Growl, 16, EObject_State::Basic);
	SetAnimation(Boss_Growl_L, 16, EObject_State::Basic_L);
	SetSize(150.f, 300.f);
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