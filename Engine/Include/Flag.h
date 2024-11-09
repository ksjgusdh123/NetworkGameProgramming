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
	Item,
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
	Jump,
	Jump_L,
	Jump_Down,
	Jump_Down_L,
	Max
};