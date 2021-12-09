#include "Grid.h"

Grid::Grid(int map_width, int map_height)
{
	this->map_width = map_width;
	this->map_height = map_height;

	this->cell_width = SCREEN_WIDTH / 2;
	this->cell_height = SCREEN_HEIGHT / 2;

	this->numberOfColumns = (int)(map_width/ cell_width) + 1;
	this->numberOfRows = (int)(map_height / cell_height) + 1 ;

	cells.resize(numberOfRows);

	for (int i = 0; i < numberOfRows; i++)
		cells[i].resize(numberOfColumns);

	for (int i = 0; i < numberOfRows; i++)
		for (int j = 0; j < numberOfColumns; j++)
			cells[i][j].clear();
}

Grid::~Grid()
{
}

void Grid::PushObjectIntoGrid(vector<LPGAMEENTITY> listObj)
{
	for (int i = 0; i < listObj.size(); i++)
	{
		int col = (int)(listObj[i]->x / cell_width);
		int row = (int)(listObj[i]->y / cell_height);
		cells[row][col].push_back(listObj[i]);
		//DebugOut(L"Them 1 object vo grid hang %d cot %d \n", row, col);
	}
}

void Grid::GetListObject(float camX, float camY, vector<LPGAMEENTITY>& listObj)
{
	int start_col = (int)(camX / cell_width);
	int start_row = (int)(camY / cell_height);
	//DebugOut(L" %d %d \n", start_row, start_col);
	int end_col = start_col + 2;
	int end_row = start_row + 2;
	if (start_col > 0)
		start_col -= 1;
	if (start_row > 0)
		start_row -= 1;
	if (numberOfRows <= end_row)
		end_row = (numberOfRows-1);
	if (numberOfColumns <= end_col)
		end_col = (numberOfColumns-1);
	//DebugOut(L" end %d %d \n", end_row, end_col);
	for (int j = start_col; j <= end_col; j++)
	{
		for (int k = start_row; k <= end_row; k++)
		{
			for (int i = 0; i < cells[k][j].size(); i++)
			{
				listObj.push_back(cells[k][j][i]);
				//DebugOut(L"get object tư grid hang %d cot %d thu %d \n", k,j,i);
			}
		}
	}
}

void Grid::Reset(int map_width, int map_height)
{
	this->map_width = map_width;
	this->map_height = map_height;

	this->numberOfColumns = (int)(map_width / (SCREEN_WIDTH / 2)) + 1;
	this->numberOfRows = (int)(map_height / (SCREEN_HEIGHT / 2)) + 1;

	cells.resize(numberOfRows);

	for (int i = 0; i < numberOfRows; i++)
		cells[i].resize(numberOfColumns);

	for (int i = 0; i < numberOfRows; i++)
		for (int j = 0; j < numberOfColumns; j++)
			cells[i][j].clear();
}