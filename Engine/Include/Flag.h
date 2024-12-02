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
	WALL,
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
	Up,
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
	Attack2,
	Attack_L2,
	Jump,
	Jump_L,
	Jump_Down,
	Jump_Down_L,
	Telpo,
	Telpo_L,
	Damaged,
	Damaged_L,
	Die,
	Die_L,
	Max
};

enum class ETile_Num
{
	Tile_NONE,
	Tile_1,
	Tile_2,
	Tile_3,
	Tile_4,
	Tile_5,
	Tile_6,
	Tile_7,
	Tile_8,
	Tile_9,
	Tile_10,
	Tile_11,
	Tile_12,
	Tile_13,
	Tile_14,
	Tile_15,
	Tile_16,
	Tile_17,
	max
};

enum class EPlayer_Job
{
	Sword,
	Archer,
	Max
};