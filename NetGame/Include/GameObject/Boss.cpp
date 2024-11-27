#include "Boss.h"
#include "Engine.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\BossAnimation.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "Collision.h"

bool CBoss::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(50.f, 0.f);
	CreateTexture(4);
	SetTexture("Boss_L", TEXT("Monster/Boss_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Boss_R", TEXT("Monster/Boss_R.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetTexture("Telpo", TEXT("Monster/telpo2.png"), EObject_Dir::Up, ETexture_Type::CIMAGE);
	
	SetAnimation(Boss_Basic_L, 8, EObject_State::Basic_L);
	SetAnimation(Boss_Basic_R, 8, EObject_State::Basic);
	SetAnimation(Boss_Attack1_L, 8, EObject_State::Attack_L);
	SetAnimation(Boss_Attack1_R, 8, EObject_State::Attack);
	SetAnimation(Boss_Attack2_L, 8, EObject_State::Attack_L2);
	SetAnimation(B_TELPO, 17, EObject_State::Telpo);
	SetAnimation(B_TELPO, 17, EObject_State::Telpo_L);
	SetSize(130.f, 160.f);
	
	m_hp = 100;
	m_objectDir = EObject_Dir::Up;
	m_objectState = EObject_State::Telpo;
	m_bIsAlive = true;
	m_timer = 0.f;
	return true;
}

void CBoss::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);

	if (m_objectState == EObject_State::Attack || m_objectState == EObject_State::Attack_L) {
		m_timer += elapsedTime;

		if (m_timer >= 1.f) {
			if (m_objectState == EObject_State::Attack)
				m_objectState = EObject_State::Basic;
			if (m_objectState == EObject_State::Attack_L)
				m_objectState = EObject_State::Basic_L;
		}
	}
	
}

void CBoss::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}


void CBoss::Attack(Vector2 target)
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


void CBoss::Render(HDC hDC, float elapsedTime)
{
#ifdef DEBUG
	m_collisionBox->Render(hDC, elapsedTime);
#endif

	if (m_prevObjectState != m_objectState) {
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



		if (m_texture[(int)m_objectDir]->GetTextureType() == ETexture_Type::Sprite)
		{
			if (m_texture[(int)m_objectDir]->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_objectDir]->GetDC(), 0, 0, (int)m_size.x, (int)m_size.y, m_texture[(int)m_objectDir]->GetColorKey());
			}
			else
			{
				BitBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_objectDir]->GetDC(), 0, 0, SRCCOPY);
			}
		}
		else if (m_texture[(int)m_objectDir]->GetTextureType() == ETexture_Type::CIMAGE)
		{

			m_time += elapsedTime;
			m_idx = (((int)(m_time * m_animationBox[(int)m_objectState].size())) % m_animationBox[(int)m_objectState].size());
			Vector2 size{ (float)m_animationBox[(int)m_objectState][m_idx].right, (float)m_animationBox[(int)m_objectState][m_idx].bottom };
			if ((int)m_objectDir == 2) size = Vector2(130, 150);
				
			renderLT = pos - m_pivot * size;
			m_texture[(int)m_objectDir]->GetCImage().Draw(hDC, (int)renderLT.x, (int)renderLT.y, (int)size.x, (int)size.y, m_animationBox[(int)m_objectState][m_idx].left, m_animationBox[(int)m_objectState][m_idx].top, m_animationBox[(int)m_objectState][m_idx].right, m_animationBox[(int)m_objectState][m_idx].bottom);
		}
	}
}
