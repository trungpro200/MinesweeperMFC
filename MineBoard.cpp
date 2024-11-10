#include "pch.h"
#include "MineBoard.h"

void MineBoard::createBoard()
{
	if (tiles != nullptr)
		deleteBoard();

	tiles = new Tile * [size];

	for (int i = 0; i < size; i++) {
		tiles[i] = new Tile[size];
	}

	//Randomize
	
}

void MineBoard::deleteBoard()
{
	if (tiles == nullptr)
		return;

	for (int i = 0; i < size; i++) {
		delete[] tiles[i];
	}
	delete[] tiles;
}

void MineBoard::drawTile(CPaintDC& dc, int STATE, int x, int y)
{
	Tile& tp = getTile(CPoint(x, y));
	if (!tp.requireUpdate) {
		return;
	}

	tp.requireUpdate = false;
	Sprite* sprite;
	CPoint origin(x * spriteSheet.spriteWidth + pos.x, y * spriteSheet.spriteHeight + pos.y);

	switch (STATE) {
	case UNKNOWN_TILE:
		sprite = spriteSheet.getSprite(0, 0);
		break;
	case SELECTED_TILE:
		sprite = spriteSheet.getSprite(1, 0);
		break;
	case NOBOMB_TILE:
		sprite = spriteSheet.getSprite(1, 0);
		break;
	case FLAGGED:
		sprite = spriteSheet.getSprite(2, 0);
		break;
	case EXPLODED_TILE:
		sprite = spriteSheet.getSprite(6, 0);
		break;
	case BOMB_TILE:
		sprite = spriteSheet.getSprite(5, 0);
		break;
	case WRONG_FLAG:
		sprite = spriteSheet.getSprite(7, 0);
		break;
	default:
		sprite = spriteSheet.getSprite(STATE-1, 1);
		break;
	}

	//TRACE("%i, %i\n", origin.x, origin.y);

	sprite->blit(dc, origin.x, origin.y);
}

MineBoard::MineBoard(int size)
{
	//Default stuffs
	started = false;
	finished = false;
	bomb = 1;
	tileLeft = size * size;

	//shouldn't be default
	tiles = nullptr;
	this->size = size;
	spriteSheet.setSize(24, 24);

	width = spriteSheet.spriteWidth * size;
	height = spriteSheet.spriteHeight * size;

	createBoard();
}

MineBoard::~MineBoard()
{
	deleteBoard();
}

void MineBoard::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

void MineBoard::generateBombs(double rate)
{
	static int a = 0;
	srand(time(NULL)+a); //Seeding

	a++;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			double f = (double) rand() / RAND_MAX;
			tiles[i][j].haveBomb = f < rate;
			bomb += f < rate;
		}
	}

	//Calc gradients
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			calcGradient(CPoint(i, j));
		}
}

void MineBoard::generateBombs(int count, CPoint bl)
{
	static int a = 0;
	srand(time(NULL) + a); //Seeding
	bomb = count;

	for (int i = 0; i < count; i++) {
		CPoint ran;
		do
		{
			ran.x = rand() % size;
			ran.y = rand() % size;

			if (getTile(ran).haveBomb) {
				continue;
			}

			CPoint d = ran - bl;
			if (sqrt(d.x * d.x + (d.y * d.y)) < 1.5) {
				continue;
			}

			getTile(ran).haveBomb = 1;
			break;
		} while (true);
	}

	//Calc gradients
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			calcGradient(CPoint(i, j));
		}
}

void MineBoard::calcGradient(CPoint pos) 
{
	if (pos.x == -1)
		return;

	getTile(pos).pos = pos;

	std::vector<Tile*> neighbour = getNeighbour(pos);
	int S = 0;

	for (Tile* a : neighbour) {
		S += a->haveBomb;
	}

	tiles[pos.y][pos.x].gradient = S;
}

void MineBoard::draw(CPaintDC& dc)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			drawTile(dc, tiles[i][j].state, j, i);
			//TRACE("%i\t", board[i][j]);
		}
		//TRACE("\n");
	}
}

//Select only
void MineBoard::clickDown(CPoint point)
{
	if (finished) return;
	CPoint clk = screenToBoard(point);

	if (clk.x == -1)
		return;
	if (getState(clk) != UNKNOWN_TILE)
		return;

	

	getNeighbour(clk);

	sel = clk;
	
	setState(sel, SELECTED_TILE);
}

//Comfirm sweeping
void MineBoard::clickUp(CPoint point)
{
	if (finished) return;

	if (getState(sel) == SELECTED_TILE) {
		setState(sel, UNKNOWN_TILE);
		if (!started) startGame(sel);
		openTile(sel);
		return;
		//TRACE("%i\n", getTile(sel).gradient);
	}

	CPoint up = screenToBoard(point);
	if (up.x == -1) return;

	if (getState(up) >= 1) {
		if (queryNeighbour(up, FLAGGED) == getState(up)) {
			for (auto t : getNeighbour(up)) {
				openTile(t->pos);
			}
			return;
		}
		
		if ((queryNeighbour(up, UNKNOWN_TILE)+queryNeighbour(up, FLAGGED)) == getState(up)) {
			for (auto t : getNeighbour(up)) {
				if (t->state == UNKNOWN_TILE)
					setState(*t, FLAGGED);
			}
		}
	}
	
	sel = CPoint(-1, -1);
}

void MineBoard::mouseMove(CPoint point)
{
	if (finished) return;

	CPoint nPos = screenToBoard(point);

	if (nPos == sel)
		return;

	if (nPos.x == -1 || getState(nPos) != UNKNOWN_TILE) {
		setState(sel, UNKNOWN_TILE);
		return;
	}

	setState(sel, UNKNOWN_TILE);
	setState(nPos, SELECTED_TILE);
	sel = nPos;
}

void MineBoard::rightClick(CPoint point)
{
	if (finished) {
		return;
	}
	CPoint p = screenToBoard(point);

	if (p.x == -1) {
		return;
	}

	flagTile(p);
}

void MineBoard::flagTile(CPoint pos)
{
	if (getState(pos) == UNKNOWN_TILE) {
		setState(pos, FLAGGED);
		return;
	}
	if (getState(pos) == FLAGGED) {
		setState(pos, UNKNOWN_TILE);
		return;
	}
}

void MineBoard::openTile(CPoint pos)
{
	if (getState(pos) != UNKNOWN_TILE)
		return;

	Tile& tile = getTile(pos);

	if (tile.haveBomb) {
		setState(tile, EXPLODED_TILE);
		
		finishGame(false); //Yea you lost. Skill issue :/
		return;
	}
	//No bomb
	setState(tile, tile.gradient);
	tileLeft--;
	TRACE("%i\n", tileLeft);

	if (tile.gradient == 0) {
		for (auto t : getNeighbour(pos)) {
			openTile(t->pos);
		}
	}

}

CPoint MineBoard::screenToBoard(CPoint screenPos)
{
	if (screenPos.x < pos.x || screenPos.y < pos.y) //Upper left boundary
		return CPoint(-1, -1);

	CPoint ret;

	ret.x = (screenPos.x-pos.x) / spriteSheet.spriteWidth;
	ret.y = (screenPos.y-pos.y) / spriteSheet.spriteHeight;

	if (ret.x >= size || ret.y >=size) { //Lower right boundary
		return CPoint(-1, -1);
	}

	//TRACE("%i, %i\n", ret.x, ret.y);

	return ret;
}

int MineBoard::getState(CPoint pt) const
{
	if (pt.x == -1) {
		return ERRORTYPE;
	}
	return tiles[pt.y][pt.x].state;
}

void MineBoard::setState(Tile& tile, int state)
{
	setState(tile.pos, state);
}

void MineBoard::setState(CPoint pt, int state)
{
	if (pt.x == -1)
		return;
	Tile& t = getTile(pt);
	if (t.state != state) {
		t.state = state;
		t.requireUpdate = true;
	}
}

Tile& MineBoard::getTile(CPoint pt)
{
	// TODO: insert return statement here
	return tiles[pt.y][pt.x];
}

void MineBoard::finishGame(bool win)
{
	finished = true;
	this->win = win;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			CPoint pos(j, i);
			Tile& tp = getTile(pos);
			int state = getState(pos);

			if (state == NOBOMB_TILE) {
				continue;
			}

			if (state == FLAGGED && !tp.haveBomb) {
				setState(tp, WRONG_FLAG);
				continue;
			}

			if (state == UNKNOWN_TILE && tp.haveBomb) {
				if (win) {
					setState(tp, FLAGGED);
				}
				else
					setState(tp, BOMB_TILE);
				continue;
			}
		}
}

std::vector<Tile*> MineBoard::getNeighbour(
	CPoint pos //Center pos
)
{
	if (pos.x == -1) {
		return std::vector<Tile*>();
	}

	std::vector<Tile*> neighbour;
	
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;

			int x = pos.x + i;
			int y = pos.y + j;

			if (x < 0 || y < 0)
				continue;
			if (x >= size || y >= size)
				continue;

			neighbour.push_back(&tiles[y][x]);
		}
	return neighbour;
}

int MineBoard::queryNeighbour(CPoint pos, int state)
{
	int S = 0;
	for (auto t : getNeighbour(pos)) {
		S += (state == t->state);
	}
	return S;
}

//int MineBoard::getNeigbourFlags(CPoint pos)
//{
//	int S = 0;
//	for (auto t : getNeighbour(pos)) {
//		if (t->state == FLAGGED) {
//			S++;
//		}
//	}
//	return S;
//}
//
//int MineBoard::getUnknownNeighbour(CPoint pos)
//{
//	int S = 0;
//	for (auto t : getNeighbour(pos)) {
//		S += t->state == UNKNOWN_TILE;
//	}
//}

void MineBoard::restartGame()
{
	started = false;
	finished = false;
	bomb = 0;
	tileLeft = size * size;

	createBoard();
}

void MineBoard::startGame(CPoint pos)
{
	started = true;
	
	while (getTile(pos).gradient != 0 || getTile(pos).haveBomb) {
		generateBombs(15, pos);
	}

	int S = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			S += getState(CPoint(i, j)) == BOMB_TILE;
		}
	}
}
