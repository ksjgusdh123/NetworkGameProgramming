#include "EngineInfo.h"
#include "Collision.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"

CCollision::CCollision()
{
}

CCollision::~CCollision()
{
}

void CCollision::UpdateCollision(Vector2& pos, Vector2& size)
{
	m_info.LT.x = pos.x - size.x / 2.f;
	m_info.LT.y = pos.y - size.y / 2.f;
	m_info.RB.x = pos.x + size.x / 2.f;
	m_info.RB.y = pos.y + size.y / 2.f;
}

void CCollision::Render(HDC hDC, float elapsedTime)
{
#ifdef DEBUG
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 브러쉬 생성

	CScene* scene = CSceneManager::GetInst()->GetScene();
	CCamera* Camera = scene->GetCamera();

	RECT	RenderRC;
	RenderRC.left = (long)(m_info.LT.x - Camera->GetPos().x);
	RenderRC.top = (long)(m_info.LT.y - Camera->GetPos().y);
	RenderRC.right = (long)(m_info.RB.x - Camera->GetPos().x);
	RenderRC.bottom = (long)(m_info.RB.y - Camera->GetPos().y);


	FrameRect(hDC, &RenderRC, hBrush);
	DeleteObject(hBrush);
#endif
}

bool CCollision::CheckCollision(CCollision* box)
{
	Vector2 otherLT = box->m_info.LT;
	Vector2 otherRB = box->m_info.RB;

	if (m_info.RB.x < otherLT.x || m_info.LT.x > otherRB.x) 
		return false; // X축에서 겹치지 않음

	if (m_info.RB.y < otherLT.y || m_info.LT.y > otherRB.y) 
		return false; // Y축에서 겹치지 않음

	return true;
}
