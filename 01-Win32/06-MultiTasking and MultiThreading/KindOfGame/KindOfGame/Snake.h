// Directions
#define DIR_UP 0
#define DIR_DW 1
#define DIR_LF 2
#define DIR_RT 3

// Global Variables
int sizeCanvas = 50;

class SnakeBody {
private:
	int iX;
	int iY;

public:
	SnakeBody *lpsbNext;
	SnakeBody *lpsbPrev;

	inline int GetX() {
		return iX;
	};
	inline int GetY() {
		return iY;
	};
	inline void SetX(int _iX) {
		iX = _iX;
	};
	inline void SetY(int _iY) {
		iY = _iY;
	};
};

class Snake {

private:
	int iDirection;
	int iLength;
	SnakeBody *lpsbSnake;

public:
	void SetDirection(int iDir);
	int GetDirection();
	void GrowSnake();
	void MoveSnake();
};

int Snake::GetDirection()
{
	return iDirection;
}

void Snake::SetDirection(int iNewDirection)
{
	iDirection = iNewDirection;
}

void Snake::GrowSnake()
{
	SnakeBody snakeHead = *lpsbSnake;

	switch (iDirection)
	{
	case DIR_UP:
		break;

	case DIR_DW:
		break;

	case DIR_LF:
		break;

	case DIR_RT:
		break;
	}
/*
	sbBody.SetX(lpsbSnake->GetX());
	sbBody.SetY(lpsbSnake->GetY());*/
	lpsbSnake->GetY();

	//while(lpsbSnake)
}

void Snake::MoveSnake()
{
	//lpsbSnake.
}