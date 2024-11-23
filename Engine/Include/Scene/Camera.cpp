#include "Camera.h"
#include "../GameObject.h"

void CCamera::Update(float elapsedTime)
{
	if (m_viewType == ECamera_Type::Nontarget)
		return;

	if (m_target)
	{
		if (!m_target->GetActive())
		{
			m_target = nullptr;
		}
		else
		{
			Vector2 targetPos = m_target->GetPos();
			if (targetPos.x > -480.f && targetPos.x < 520.f)
				m_pos.x = targetPos.x - m_targetPivot.x * m_resolution.x + m_targetOffset.x;
			if (targetPos.y <= 180.f)
				m_pos.y = targetPos.y - m_targetPivot.y * m_resolution.y + m_targetOffset.y;
		}
	}

	if (m_viewType == ECamera_Type::Default)
	{
		if (m_pos.x < 0.f)	
		{
			m_pos.x = 0.f;
		}
		else if (m_pos.x + m_resolution.x > m_worldResolution.x)
		{
			m_pos.x = m_worldResolution.x - m_resolution.x;
		}

		if (m_pos.y < 0.f)
		{
			m_pos.y = 0.f;
		}
		else if (m_pos.y + m_resolution.y > m_worldResolution.y)
		{
			m_pos.y = m_worldResolution.y - m_resolution.y;
		}
	}
}
