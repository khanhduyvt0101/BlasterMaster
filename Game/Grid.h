#pragma once

#include <vector>
#include "Entity.h"
#include "define.h"

class Grid
{
	int map_width;
	int map_height;

	int cell_width;
	int cell_height;

	int numberOfColumns;
	int numberOfRows;
	vector<vector<vector<LPGAMEENTITY>>> cells;
public:
	Grid(int map_width, int map_height);
	~Grid();
	void PushObjectIntoGrid(vector<LPGAMEENTITY> listObj);
	void GetListObject(float camX, float camY, vector<LPGAMEENTITY>& listObj);
	void Reset(int map_width, int map_height);
};

