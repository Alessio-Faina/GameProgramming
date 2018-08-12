#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <vector>
#include <cstdlib>
#include <time.h>
#include <string>

#define WINDOWS_FONTS_FOLDER "c:\\Windows\\Fonts\\arial.ttf"
#define FONT_THICKNESS 15

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
#define GAME_TITLE		"Game1: Pong"
#define WINDOW_X_COORD	250
#define WINDOW_Y_COORD	250

#define WALL_THICKNESS		20
#define PADDLE_THICKNESS_W	10
#define PADDLE_THICKNESS_H	75
#define BALL_THICKNESS		15

enum PlayerSide
{
	Left = 0,
	Right = 1
};

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 ballPos;
	Vector2 ballSpeed;
	bool isBallDead;
};

class Game
{
public:
	Game();
	~Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void InitBall(Ball* ball);
	void InitBalls();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void moveBall(Ball* ball, float deltaTime);
	void movePaddle(int paddleDir, Vector2* Paddle,
		float deltaTime);
	void checkCollisionLeft(Ball* ball);
	void checkCollisionRight(Ball* ball);

	void writeScore(int playerScore,
		enum PlayerSide side);
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	bool mIsRunning;
	bool mEndGameWait;

	int mPaddleDirLeft;
	int mPaddleDirRight;

	//SDL_Rect mWallRight;
	SDL_Rect mWallTop;
	SDL_Rect mWallBottom;

	Vector2 mPaddlePosLeft;
	Vector2 mPaddlePosRight;

	int mPlayerLeftScore;
	int mPlayerRightScore;

	std::vector<Ball> mBalls;
	Uint8 mRunningBalls;

	TTF_Font* mFont;
	
	//Vector2 mBallPos;
	//Vector2 mBallSpeed;
};

