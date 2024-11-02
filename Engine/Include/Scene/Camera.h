#pragma once

#include "../EngineInfo.h"

class CCamera
{
public:
	Vector2 GetPos() const					{ return m_pos; }
	Vector2 GetResolution() const			{ return m_resolution; }
	Vector2 GetWorldResolution() const		{ return m_worldResolution; }
	class CGameObject* GetTarget() const	{ return m_target; }
	Vector2 GetTargetOffset() const			{ return m_targetOffset; }
	Vector2 GetTargetPivot() const			{ return m_targetPivot; }
	ECamera_Type GetViewType() const		{ return m_viewType; }

	void SetPos(const Vector2& pos)							{ m_pos = pos; }
	void SetPos(float x, float y)							{ m_pos = Vector2(x, y); }
	void SetResolution(const Vector2& resolution)			{ m_resolution = resolution; }
	void SetResolution(float x, float y)					{ m_resolution = Vector2(x, y); }
	void SetWorldResolution(const Vector2& resolution)		{ m_worldResolution = resolution; }
	void SetWorldResolution(float x, float y)				{ m_worldResolution = Vector2(x, y); }
	void SetTarget(class CGameObject* target)				{ m_target = target; }
	void SetTargetOffset(const Vector2& offset)				{ m_targetOffset = offset; }
	void SetTargetOffset(float x, float y)					{ m_targetOffset = Vector2(x, y); }
	void SetTargetPivot(const Vector2& pivot)				{ m_targetPivot = pivot; }
	void SetTargetPivot(float x, float y)					{ m_targetPivot = Vector2(x, y); }
	void SetViewType(ECamera_Type type)						{ m_viewType = type; }

public:
	void Update(float elapsedTime);

private:
	Vector2	m_pos;
	Vector2	m_resolution;
	Vector2	m_worldResolution;
	class CGameObject*	m_target	= nullptr;
	Vector2	m_targetOffset;
	Vector2	m_targetPivot			= { 0.5f, 0.5f };
	ECamera_Type m_viewType			= ECamera_Type::Default;
};

