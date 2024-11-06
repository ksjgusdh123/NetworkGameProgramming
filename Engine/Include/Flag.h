#pragma once

enum class ERender_Type
{
	Elipse,
	Rectangle
};

enum class EObject_Type
{
	Monster,
	Player,
	Max
};

enum class ECamera_Type
{
	Target,
	Default,
	Nontarget
};

enum class ETexture_Type
{
	Sprite,
	CIMAGE,
	Frame
};

enum class EObject_Dir
{
	Right,
	Left,
	Max
};

enum class EObject_State
{
	Basic,
	Basic_L,
	Walk,
	Walk_L,
	Run,
	Attack,
	Attack_L,
	Max
};