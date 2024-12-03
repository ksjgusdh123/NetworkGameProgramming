#pragma once
#include "Define.h"

class TileManager
{
public:
	void CreateTile();
	void CreateBossTile();
	void AddTile(TileInfo& info, vector2 blockSize, int type, int x, int y);
	bool CollisionCheck(GamePlayerInfo& player);
	void SendTilePacket();
public:
	static TileManager& GetInst()
	{
		static TileManager inst;
		return inst;
	}
public:
	vector<TileInfo> tiles;
	std::vector<int> tileNumbers;
	std::vector<vector2> tilePositions;
};

