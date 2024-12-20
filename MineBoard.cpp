#include "pch.h"
#include "MineBoard.h"

void GameClass::createBoard()
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

void GameClass::deleteBoard()
{
	if (tiles == nullptr)
		return;

	for (int i = 0; i < size; i++) {
		delete[] tiles[i];
	}
	delete[] tiles;
}

void GameClass::drawTile(CPaintDC& dc, int STATE, int x, int y)
{
	Tile& tp = getTile(CPoint(x, y));
	if (!tp.requireUpdate) {
		return;
	}

	tp.requireUpdate = false;
	Sprite* sprite;
	CPoint origin(x * tilestates.spriteWidth + pos.x, y * tilestates.spriteHeight + pos.y);

	switch (STATE) {
	case UNKNOWN_TILE:
		sprite = tilestates.getSprite(0, 0);
		break;
	case SELECTED_TILE:
		sprite = tilestates.getSprite(1, 0);
		break;
	case NOBOMB_TILE:
		sprite = tilestates.getSprite(1, 0);
		break;
	case FLAGGED:
		sprite = tilestates.getSprite(2, 0);
		break;
	case EXPLODED_TILE:
		sprite = tilestates.getSprite(6, 0);
		break;
	case BOMB_TILE:
		sprite = tilestates.getSprite(5, 0);
		break;
	case WRONG_FLAG:
		sprite = tilestates.getSprite(7, 0);
		break;
	default:
		sprite = tilestates.getSprite(STATE-1, 1);
		break;
	}

	//TRACE("%i, %i\n", origin.x, origin.y);

	sprite->blit(dc, origin.x, origin.y);
}

GameClass::GameClass(int size)
{
	//Default stuffs
	started = false;
	finished = false;
	bomb = 0;
	tileLeft = size * size;
	onExcavate = false;

	//shouldn't be default
	tiles = nullptr;
	this->size = size;
	tilestates.setSize(24, 24);

	width = tilestates.spriteWidth * size;
	height = tilestates.spriteHeight * size;

	createBoard();
}

GameClass::~GameClass()
{
	deleteBoard();
}

void GameClass::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;

	bombLeft.rightAlign(pos, CPoint(width, height));
	timePassed.leftAlign(pos, CPoint(0, 0));
}


void GameClass::generateBombs(int count, CPoint bl)
{
	static int a = 0;
	srand(time(NULL) + a); //Seeding for randomization
	bomb = count; 
	bombLeft.score = bomb;

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

void GameClass::draw(CPaintDC& dc)
{
	static bool firstDraw = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			drawTile(dc, tiles[i][j].state, j, i);
			//TRACE("%i\t", board[i][j]);
		}
		//TRACE("\n");
	}

	bombLeft.draw(dc);
	//timePassed.draw(dc);

	if (firstDraw) {
		timePassed.draw(dc);
		firstDraw = false;
	}
}

//Select only
void GameClass::clickDown(CPoint point)
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
void GameClass::clickUp(CPoint point)
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

void GameClass::mouseMove(CPoint point)
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

void GameClass::rightClick(CPoint point)
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

void GameClass::flagTile(CPoint pos)
{
	if (getState(pos) == UNKNOWN_TILE) {
		setState(pos, FLAGGED);
		sound(L"res/flag.wav");
		bombLeft.score--;
		return;
	}
	if (getState(pos) == FLAGGED) {
		setState(pos, UNKNOWN_TILE);
		sound(L"res/unflag.wav");
		bombLeft.score++;
		return;
	}
}

void GameClass::openTile(CPoint pos)
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

CPoint GameClass::screenToBoard(CPoint screenPos)
{
	if (screenPos.x < pos.x || screenPos.y < pos.y) //Upper left boundary
		return CPoint(-1, -1);

	CPoint ret;

	ret.x = (screenPos.x-pos.x) / tilestates.spriteWidth;
	ret.y = (screenPos.y-pos.y) / tilestates.spriteHeight;

	if (ret.x >= size || ret.y >=size) { //Lower right boundary
		return CPoint(-1, -1);
	}

	//TRACE("%i, %i\n", ret.x, ret.y);

	return ret;
}

int GameClass::getState(CPoint pt) const
{
	if (pt.x == -1) {
		return ERRORTYPE;
	}
	return tiles[pt.y][pt.x].state;
}

void GameClass::setState(Tile& tile, int state)
{
	setState(tile.pos, state);
}

void GameClass::setState(CPoint pt, int state)
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

Tile& GameClass::getTile(CPoint pt)
{
	// TODO: insert return statement here
	return tiles[pt.y][pt.x];
}

void GameClass::finishGame(bool win)
{
	if (finished) {
		return;
	}
	finished = true;
	this->win = win;

	if (win) {
		sound(L"res/win.wav");
		bombLeft.score = 0;
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

std::vector<Tile*> GameClass::getNeighbour(
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

int GameClass::queryNeighbour(CPoint pos, int state)
{
	int S = 0;
	for (auto t : getNeighbour(pos)) {
		S += (state == t->state);
	}
	return S;
}

void GameClass::restartGame()
{
	sound(NULL);

	started = false;
	finished = false;
	tileLeft = size * size;

	bombLeft.score = bomb;
	timePassed.score = 0;

	createBoard();
}

void GameClass::startGame(CPoint pos)
{
	started = true;
	bombLeft.score = bomb;
	generateBombs(bomb, pos);
}

void GameClass::sound(LPCTSTR src)
{
	PlaySound(src, NULL, SND_FILENAME | SND_ASYNC | SND_NOWAIT);
}
