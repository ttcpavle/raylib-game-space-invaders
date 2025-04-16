#include "game.h"
#include <iostream>
#include <fstream>

Game::Game() {
	level = 1; // has to be called before init game
	music = LoadMusicStream("assets/Sounds/music.ogg");
    explosionSound = LoadSound("assets/Sounds/explosion.ogg");
    PlayMusicStream(music);
	InitGame();
	score = 0;
}

Game::~Game() {
	Alien::UnloadImages();
	UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Draw() {
	spaceship.Draw();
	for (auto& laser : spaceship.lasers) {
		laser.Draw();
	}

	for (auto& obstacle : obstacles) {
		obstacle.Draw();
	}

	for(auto& alien : aliens){
		alien.Draw();
	}

	for(auto& laser : alienLasers){
		laser.Draw();
	}
	mysteryship.Draw();
}

void Game::Update() {
	if(isRunning){

		if(aliens.empty()){
			NextLevel();
			return;
		}

		double currentTime = GetTime();
		if(currentTime - timeLastSpawn > mysteryShipSpawnInterval) {
			mysteryship.Spawn();
			timeLastSpawn = GetTime();
			mysteryShipSpawnInterval = GetRandomValue(10, 20);
		}

		for (auto& laser : spaceship.lasers) {
			laser.Update();
		}
		MoveAliens();
		AlienShootLaser();
		for(auto& laser : alienLasers){
			laser.Update();
		}
		DeleteInactiveLasers();
		mysteryship.Update();
		CheckForCollisions();
	}else if(IsKeyDown(KEY_ENTER)){
		Reset();
		InitGame();
	}
}

void Game::HandleInput()
{
	if(isRunning){
		// int key = GetKeyPressed();
		if (IsKeyDown(KEY_LEFT)) {
			spaceship.MoveLeft();
		}else if (IsKeyDown(KEY_RIGHT)) {
			spaceship.MoveRight();
		}
		
		if (IsKeyDown(KEY_SPACE)) {
			spaceship.FireLaser();
		}
	}
}

void Game::CreateObstacles()
{
	int obstacleWidth = Obstacle::grid[0].size() * 3;
	float gap = (GetScreenWidth() - (obstacleWidth * 4)) / 5;

	for (int i = 0; i < 4; i++) {
		float offsetX = (i + 1) * gap + i * obstacleWidth; // spread out obstacles evenly
		obstacles.push_back(Obstacle({ offsetX, float(GetScreenHeight() - 200) }));
	}
}

void Game::DeleteInactiveLasers() {
	// using iterators
	for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) {
		if (!it->active) {
			it = spaceship.lasers.erase(it);
		}
		else {
			++it;
		}
	}
	for (auto it = alienLasers.begin(); it != alienLasers.end();) {
		if (!it->active) {
			it = alienLasers.erase(it);
		}
		else {
			++it;
		}
	}
}

std::vector<Alien> Game::CreateAliens()
{
	std::vector<Alien> aliens;
	for(int row = 0; row < 5; row++){
		for(int column = 0; column < 11; column++){
			float x = 75 + column * 55;
			float y = 110 + row * 55;

			int alienType;
			if(row == 0){
				alienType = 3;
			}else if(row == 1 || row == 2){
				alienType = 2;
			}else {
				alienType = 1;
			}
			
			aliens.push_back(Alien(alienType, {x, y}, CalculateHealth(alienType)));
			
		}
	}
    return aliens;
}

void Game::MoveAliens() {
    for(auto& alien: aliens) {
        if(alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25) {
            aliensDirection = -1;
            MoveDownAliens(4);
        }
        if(alien.position.x < 25) {
            aliensDirection = 1;
            MoveDownAliens(4);
        }
    }

	for(auto& alien: aliens) {
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance) {
	for(auto& alien : aliens){
		alien.position.y += distance;
	}
}

void Game::CheckForCollisions()
{
	// spaceship laser
	for(auto& laser : spaceship.lasers){
		// laser-alien collision
		auto it = aliens.begin();
		while(it != aliens.end()){
			if(CheckCollisionRecs(it->getRect(), laser.getRect()) && laser.active){
				PlaySound(explosionSound);
				std::cout << "health before: " << it->health << std::endl;	
				it->health--;
				std::cout << "health after:  " << it->health << std::endl;				
				if(it->health <= 0){
					if(it->type == 1){
						score += 100;
					}else if(it->type == 2){
						score += 200;
					}else if(it->type == 3){
						score += 300;
					}
					it = aliens.erase(it);		
				}
				CheckForHighScore();		
				laser.active = false;
			}else{
				++it;
			}
			
		}
		// laser-obstacle collision
		for(auto& obstacle : obstacles){
			auto it = obstacle.blocks.begin();
			while(it != obstacle.blocks.end()){
				if(CheckCollisionRecs(it->getRect(), laser.getRect())){
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}else{
					++it;
				}
			}
		}
		//laser-mysteryship collision
		if(CheckCollisionRecs(mysteryship.getRect(), laser.getRect())){
			mysteryship.alive= false;
			laser.active = false;
			score += 500;
			CheckForHighScore();
			PlaySound(explosionSound);
		}

	}

	// alien laser	
	for(auto& laser : alienLasers){
		// alienlaser-spaceship
		if(CheckCollisionRecs(laser.getRect(), spaceship.getRect())){
			laser.active = false;
			lives--;
			if(lives == 0){
				GameOver();
			}
		}
		// alienlaser-obstacle
		for(auto& obstacle : obstacles){
			auto it = obstacle.blocks.begin();
			while(it != obstacle.blocks.end()){
				if(CheckCollisionRecs(it->getRect(), laser.getRect())){
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}else{
					++it;
				}
			}
		}
	}

	// alien-obstacle collision
	for(auto& alien : aliens){

		// alien-obstacle
		for(auto& obstacle : obstacles){
			auto it = obstacle.blocks.begin();
			while(it != obstacle.blocks.end()){
				if(CheckCollisionRecs(it->getRect(), alien.getRect())){
					it = obstacle.blocks.erase(it);
				}else{
					++it;
				}
			}
		}

		//alien-spaceship
		if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())){
			GameOver();
		}
	}

}

void Game::CheckForHighScore()
{
	if(score > highScore){
		highScore = score;
		SaveHighScoreToFile(highScore);
	}
}

void Game::AlienShootLaser() {
    double currentTime = GetTime();
    
    while (currentTime - timeLastAlienFired >= alienLaserShootInterval) {
        if (!aliens.empty()) {
            int randomIndex = GetRandomValue(0, aliens.size() - 1);
            
            Alien& alien = aliens[randomIndex];
            alienLasers.push_back(Laser({
                alien.position.x + alien.alienImages[alien.type - 1].width / 2,
                alien.position.y + alien.alienImages[alien.type - 1].height
            }, 6));
            
            timeLastAlienFired += alienLaserShootInterval; // works better than GetTime()
        }
    }
}

void Game::SaveHighScoreToFile(int highScore){
	std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile << highScore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}
int Game::loadHighScoreFromFile(){
	int loadedHighScore = 0;
	std::ifstream highscoreFile("highscore.txt");
	if(highscoreFile.is_open()){
		highscoreFile >> loadedHighScore;
		highscoreFile.close();
	}else{
		std::cerr << "Failed to load highscore from file" << std::endl;
	}
	return loadedHighScore;
}

void Game::GameOver()
{
	isRunning = false;
	score = 0;
}

void Game::NextLevel(){
	isRunning = false;
	level++;
	Reset();
	InitGame();
}

void Game::InitGame(){
	CreateObstacles();
	aliens = CreateAliens();
	aliensDirection = 1;
	timeLastAlienFired = GetTime() + 2.0;
	timeLastSpawn = GetTime();
	mysteryship.alive = false;
	mysteryShipSpawnInterval = GetRandomValue(10, 20);
	lives = 3;
	highScore = loadHighScoreFromFile();
	isRunning = true;
}

// clear everything
void Game::Reset(){
	isRunning = false;
	spaceship.Reset();
	aliens.clear();
	alienLasers.clear();
	obstacles.clear();
	
}

int Game::CalculateHealth(int alienType){
	switch(level){
		case 1:
		switch(alienType){
			case 3:
				return 1;
			case 2:
				return 1;
			case 1:
				return 1;
			}
		case 2:
		switch(alienType){
			case 3:
				return 2;
			case 2:
				return 1;
			case 1:
				return 1;
			}
		case 3:
		switch(alienType){
			case 3:
				return 2;
			case 2:
				return 2;
			case 1:
				return 1;
			}
		default:
			return 2;
	}
}
