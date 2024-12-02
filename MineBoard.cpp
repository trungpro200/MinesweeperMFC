#include "pch.h"
#include "MineBoard.h"

void MineBoard::createBoard()
{
	if (tiles != nullptr)
		deleteBoard();

	tiles = new Tile * [size];

	for (int i = 0; i < size; i++) {
		tiles[i] = new Tile[size];
		for (int j = 0; j < size; j++) {
			tiles[i][j].pos = CPoint(j, i);
		}
	}
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
	onExcavate = false;

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


void MineBoard::generateBombs(int count, CPoint bl)
{
	static int a = 0;
	srand(time(NULL) + a); //Seeding for randomization
	bomb = count; 

	int i = 0;
	CPoint ran;

	while (i < count) {
		ran.x = rand() % size;
		ran.y = rand() % size;

		if (getTile(ran).haveBomb)
			continue;

		CPoint tp = ran - bl;
		tp.x = abs(tp.x);
		tp.y = abs(tp.y);
		if (tp.x <= 1 && tp.y <= 1)
			continue;

		i++;
		getTile(ran).haveBomb = 1;

		for (auto t : getNeighbour(ran)) {
			t->gradient++;
		}
	}
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

	//getNeighbour(clk);

	//sel = clk;
	
	//setState(sel, SELECTED_TILE);
}

//Comfirm sweeping
void MineBoard::clickUp(CPoint point)
{
	CPoint up = screenToBoard(point);
	if (up.x == -1) return;

	if (finished) return;

	


	if (getState(up) == UNKNOWN_TILE) {
		//setState(sel, UNKNOWN_TILE);
		if (!started) 
			startGame(up);
		openTile(up);

		//PlaySound(L"res/pop.wav", NULL, SND_FILENAME | SND_ASYNC);


		return;
	}

	

	if (getState(up) >= 1) {
		if (queryNeighbour(up, FLAGGED) == getState(up)) {

			for (auto t : getNeighbour(up)) {
				openTile(t->pos);
			}
			return;
		}
		
		/*if ((queryNeighbour(up, UNKNOWN_TILE)+queryNeighbour(up, FLAGGED)) == getState(up)) {
			for (auto t : getNeighbour(up)) {
				if (t->state == UNKNOWN_TILE)
					setState(*t, FLAGGED);
			}
		}*/
	}

	sel = CPoint(-1, -1);
}

void MineBoard::mouseMove(CPoint point)
{
	/*if (finished) return;

	CPoint nPos = screenToBoard(point);

	if (nPos == sel)
		return;

	if (nPos.x == -1 || getState(nPos) != UNKNOWN_TILE) {
		setState(sel, UNKNOWN_TILE);
		sel = CPoint(-1, -1);
		return;
	}

	setState(sel, UNKNOWN_TILE);
	setState(nPos, SELECTED_TILE);
	sel = nPos;*/
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
		sound(L"res/flag.wav");
		return;
	}
	if (getState(pos) == FLAGGED) {
		setState(pos, UNKNOWN_TILE);
		sound(L"res/unflag.wav");
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

		if (state >= 0) {
			onExcavate = true;
		}
	}
}

Tile& MineBoard::getTile(CPoint pt)
{
	// TODO: insert return statement here
	return tiles[pt.y][pt.x];
}

void MineBoard::finishGame(bool win)
{
	if (finished) {
		return;
	}
	finished = true;
	this->win = win;

	if (win) {
		sound(L"res/win.wav");
	}
	else
	{
		sound(L"res/explode.wav");
	}

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

void MineBoard::restartGame()
{
	sound(NULL);

	started = false;
	finished = false;
	bomb = 0;
	tileLeft = size * size;

	createBoard();
}

void MineBoard::startGame(CPoint pos)
{
	started = true;
	
	generateBombs(30, pos);
}

void MineBoard::sound(LPCTSTR src)
{
	PlaySound(src, NULL, SND_FILENAME | SND_ASYNC | SND_NOWAIT);
}
