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
			m_pos = m_target->GetPos() - m_targetPivot * m_resolution + m_targetOffset;
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
