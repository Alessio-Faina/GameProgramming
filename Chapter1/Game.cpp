#include "Game.h"

Game::Game()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
{

}


Game::~Game()
{
}

void Game::InitBall(Ball* ball)
{
	int diff = rand() % 20 - 10;
	int xSpeed = rand() % 2 + 1;
	int ySpeed = rand() % 2 + 1;
	ball->ballPos.x = WINDOW_WIDTH / 2.0f + 10 * diff;
	ball->ballPos.y = WINDOW_HEIGHT / 2.0f + 10 * diff;
	ball->ballSpeed.x = -350.0f * (xSpeed == 2 ? -1 : 1) + 10 * diff;
	ball->ballSpeed.y = 375.0f * (ySpeed == 2 ? -1 : 1) + 10 * diff;
	ball->isBallDead = false;
}

void Game::InitBalls()
{
	mRunningBalls = 2;

	Ball temp;
	for (int i = 0; i < mRunningBalls; i++)
	{
		InitBall(&temp);
		mBalls.push_back(temp);
	}
}

bool Game::Initialize()
{
	srand(time(NULL));
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("[%s:%i - Can't init SDL: ", __FUNCTION__, __LINE__, SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(GAME_TITLE, WINDOW_X_COORD, WINDOW_Y_COORD,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (!mWindow)
	{
		SDL_Log("[%s:%i - Can't create SDL Window: ", __FUNCTION__, __LINE__, SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("[%s:%i - Can't create SDL Renderer!: ", __FUNCTION__, __LINE__, SDL_GetError());
		return false;
	}

	mPaddlePosLeft.x = 10.0f;
	mPaddlePosLeft.y = WINDOW_HEIGHT / 2.0f;

	mPaddlePosRight.x = WINDOW_WIDTH - PADDLE_THICKNESS_W - 10.0f;
	mPaddlePosRight.y = WINDOW_HEIGHT / 2.0f;

	InitBalls();
	/*
	mBallPos.x = WINDOW_WIDTH / 2.0f;
	mBallPos.y = WINDOW_HEIGHT / 2.0f;
	mBallSpeed.x = -400.0f;
	mBallSpeed.y = 435.0f;
	*/

	mWallTop.x = 0;
	mWallTop.y = 0;
	mWallTop.h = WALL_THICKNESS;
	mWallTop.w = WINDOW_WIDTH;

	/*
	mWallRight.x = WINDOW_WIDTH - WALL_THICKNESS;
	mWallRight.y = 0;
	mWallRight.h = WINDOW_HEIGHT;
	mWallRight.w = WALL_THICKNESS;
	*/
	mWallBottom.x = 0;
	mWallBottom.y = WINDOW_HEIGHT - WALL_THICKNESS;
	mWallBottom.h = WALL_THICKNESS;
	mWallBottom.w = WINDOW_WIDTH;


	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				return;
		}
	}

	const Uint8* keybState = SDL_GetKeyboardState(NULL);
	if (keybState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
		return;
	}

	mPaddleDirLeft = 0;
	if (keybState[SDL_SCANCODE_W])
	{
		mPaddleDirLeft -= 1;
	}
	if (keybState[SDL_SCANCODE_S])
	{
		mPaddleDirLeft += 1;
	}

	mPaddleDirRight = 0;
	if (keybState[SDL_SCANCODE_I])
	{
		mPaddleDirRight -= 1;
	}
	if (keybState[SDL_SCANCODE_K])
	{
		mPaddleDirRight += 1;
	}
}

void Game::movePaddle(int paddleDir, Vector2* paddlePos,
	float deltaTime)
{
	if (paddleDir != 0)
	{
		paddlePos->y += paddleDir * 300.0f * deltaTime;
		if (paddlePos->y < WALL_THICKNESS)
		{
			paddlePos->y = WALL_THICKNESS;
		}
		else if (paddlePos->y >
			(WINDOW_HEIGHT - PADDLE_THICKNESS_H - WALL_THICKNESS))
		{
			paddlePos->y =
				WINDOW_HEIGHT - PADDLE_THICKNESS_H - WALL_THICKNESS;
		}
	}
}

void Game::checkCollisionLeft(Ball* ball)
{
	if (ball->isBallDead)
	{
		return;
	}
	float diff = ball->ballPos.y - mPaddlePosLeft.y;
	if ((diff <= PADDLE_THICKNESS_H && diff > (-BALL_THICKNESS / 2)) &&
		(ball->ballPos.x <= 25.0f) &&
		(ball->ballSpeed.x < 0.0f)
		)
	{
		ball->ballSpeed.x *= -1;
	}
	if (ball->ballPos.x < 0.0f )
	{
		ball->isBallDead = true;
		mRunningBalls--;
	}

}

void Game::checkCollisionRight(Ball* ball)
{
	if (ball->isBallDead)
	{
		return;
	}
	float diff = ball->ballPos.y - mPaddlePosRight.y;
	if ((diff <= PADDLE_THICKNESS_H && diff >(-BALL_THICKNESS / 2)) &&
		(ball->ballPos.x >= WINDOW_WIDTH - 25.0f) &&
		(ball->ballSpeed.x > 0.0f)
		)
	{
		ball->ballSpeed.x *= -1;
	}
	if (ball->ballPos.x > WINDOW_WIDTH)
	{
		ball->isBallDead = true;
		mRunningBalls--;
	}
}

void Game::moveBall(Ball* ball, float deltaTime)
{
	ball->ballPos.x += ball->ballSpeed.x * deltaTime;
	ball->ballPos.y += ball->ballSpeed.y * deltaTime;
	/*
	if ((mBallPos.x >= WINDOW_WIDTH - BALL_THICKNESS) && mBallSpeed.x > 0)
	{
	mBallSpeed.x *= -1;
	}
	*/
	if ((ball->ballPos.y >= WINDOW_HEIGHT - BALL_THICKNESS / 2 - WALL_THICKNESS)
		&& ball->ballSpeed.y > 0)
	{
		ball->ballSpeed.y *= -1;
	}
	if ((ball->ballPos.y <= BALL_THICKNESS / 2 + WALL_THICKNESS)
		&& ball->ballSpeed.y < 0)
	{
		ball->ballSpeed.y *= -1;
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	{
	}

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	movePaddle(mPaddleDirLeft, &mPaddlePosLeft, deltaTime);
	movePaddle(mPaddleDirRight, &mPaddlePosRight, deltaTime);

	for (std::vector<Ball>::iterator it = mBalls.begin(); it != mBalls.end(); ++it)
	{
		moveBall(&(*it), deltaTime);
	}

	for (std::vector<Ball>::iterator it = mBalls.begin(); it != mBalls.end(); ++it)
	{
		checkCollisionLeft(&(*it));
		checkCollisionRight(&(*it));
	}
	if(mRunningBalls <= 0)
	{
		mIsRunning = false;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
	//SDL_RenderFillRect(mRenderer, &mWallRight);
	SDL_RenderFillRect(mRenderer, &mWallTop);
	SDL_RenderFillRect(mRenderer, &mWallBottom);
 
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_Rect movingObjects{
		0, 0, BALL_THICKNESS, BALL_THICKNESS
	};
	for (std::vector<Ball>::iterator it = mBalls.begin(); it != mBalls.end(); ++it)
	{
		if (!it->isBallDead)
		{
			movingObjects.x = static_cast<int>(it->ballPos.x);
			movingObjects.y = static_cast<int>(it->ballPos.y);
			SDL_RenderFillRect(mRenderer, &movingObjects);
		}
	}

	movingObjects.x = static_cast<int>(mPaddlePosLeft.x);
	movingObjects.y = static_cast<int>(mPaddlePosLeft.y);
	movingObjects.w = PADDLE_THICKNESS_W;
	movingObjects.h = PADDLE_THICKNESS_H;
	SDL_RenderFillRect(mRenderer, &movingObjects);

	movingObjects.x = static_cast<int>(mPaddlePosRight.x);
	movingObjects.y = static_cast<int>(mPaddlePosRight.y);
	SDL_RenderFillRect(mRenderer, &movingObjects);

	SDL_RenderPresent(mRenderer);
}

