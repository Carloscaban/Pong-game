#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].hasChanged)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].hasChanged)


//         derivative of playerPos (velocity)
//                    |
//                    V
float playerPos1, playerdPos1, playerPos2, playerdPos2;
float arenaHalfSizeX = 85, arenaHalfSizeY = 45;
float playerHalfSizeX = 2.5, playerHalfSizeY = 12;
float ballPosX, ballPosY, balldPosX = 130, balldPosY, ballHalfSize = 1;

int playerScore1, playerScore2;

internal void
simulatePlayer(float* pos, float* dPos, float ddPos, float dt) {
	// friction
	ddPos -= *dPos * 10.f;

	*pos = *pos + *dPos * dt + ddPos * dt * dt * .5f;
	*dPos = *dPos + ddPos * dt;

	// collision detection
	if (*pos + playerHalfSizeY > arenaHalfSizeY) {
		*pos = arenaHalfSizeY - playerHalfSizeY;
		*dPos = 0;
	}
	else if (*pos - playerHalfSizeY < -arenaHalfSizeY) {
		*pos = -arenaHalfSizeY + playerHalfSizeY;
		*dPos = 0;
	}
}

internal bool
onCollision(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

internal void
simulateGame(Input* input, float dt) {
	clearScreen(0x808080);
	drawRect(0, 0, arenaHalfSizeX, arenaHalfSizeY, 0x000000);

	// derivative of derivative
	//            |
	//            V
	float playerddPos1 = 0.f; // acceleration

#if 0
	if (isDown(BUTTON_UP)) playerddPos1 += 2000;
	if (isDown(BUTTON_DOWN)) playerddPos1 -= 2000;
#else
	//if (ballPosY > playerPos1) playerddPos1 += 1300;
	//if (ballPosY < playerPos1) playerddPos1 -= 1300;
	playerddPos1 = (ballPosY - playerPos1) * 100;
	if (playerddPos1 > 1300) playerddPos1 = 1300;
	if (playerddPos1 < -1300) playerddPos1 = -1300;
#endif

	float playerddPos2 = 0.f;
	if (isDown(BUTTON_W)) playerddPos2 += 2000;
	if (isDown(BUTTON_S)) playerddPos2 -= 2000;

	simulatePlayer(&playerPos1, &playerdPos1, playerddPos1, dt);
	simulatePlayer(&playerPos2, &playerdPos2, playerddPos2, dt);

	// ball movement
	ballPosX += balldPosX * dt;
	ballPosY += balldPosY * dt;

	// collision detection ball
	if (onCollision(ballPosX, ballPosY, ballHalfSize, ballHalfSize, 80, playerPos1, playerHalfSizeX, playerHalfSizeY)) {
		ballPosX = 80 - playerHalfSizeX - ballHalfSize;
		balldPosX *= -1;
		balldPosY = (ballPosY - playerPos1) * 2 + playerdPos1 * .75f;
	}
	else if (onCollision(ballPosX, ballPosY, ballHalfSize, ballHalfSize, -80, playerPos2, playerHalfSizeX, playerHalfSizeY)) {
		ballPosX = -80 + playerHalfSizeX + ballHalfSize;
		balldPosX *= -1;
		balldPosY = (ballPosY - playerPos2) * 2 + playerdPos2 * .75f;
	}

	// vertical boundary collisions
	if (ballPosY + ballHalfSize > arenaHalfSizeY) { // upper boundary
		ballPosY = arenaHalfSizeY - ballHalfSize;
		balldPosY *= -1;
	}
	else if (ballPosY - ballHalfSize < -arenaHalfSizeY) { // lower boundary
		ballPosY = -arenaHalfSizeY + ballHalfSize;
		balldPosY *= -1;
	}

	// horizontal boundary collisions
	if (ballPosX + ballHalfSize > arenaHalfSizeX) {
		balldPosX *= -1;
		balldPosY *= 0;
		ballPosX = 0;
		ballPosY = 0;
		playerScore1++;
	}
	else if (ballPosX - ballHalfSize < -arenaHalfSizeX) {
		balldPosX *= -1;
		balldPosY *= 0;
		ballPosX = 0;
		ballPosY = 0;
		playerScore2++;
	}

	drawNumber(playerScore1, -10, 40, 1.f, 0xbbffbb);
	drawNumber(playerScore2, 10, 40, 1.f, 0xbbffbb);

	// rendering
	// ball
	drawRect(ballPosX, ballPosY, ballHalfSize, ballHalfSize, 0xffffff);

	// players
	drawRect(80, playerPos1, playerHalfSizeX, playerHalfSizeY, 0xffffff);
	drawRect(-80, playerPos2, playerHalfSizeX, playerHalfSizeY, 0xffffff);
}