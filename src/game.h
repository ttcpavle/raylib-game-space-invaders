#pragma once
#include "spaceship.h"
#include "obstacle.h"
#include "alien.h"
#include "mysteryship.h"

class Game {
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void HandleInput();
	bool isRunning;
	int lives;
	int score;
	int highScore;
	Music music;
	Sound explosionSound;
	int level;
	//static LevelData;
private:
	Spaceship spaceship;
	void CreateObstacles();
	void DeleteInactiveLasers();
	std::vector<Obstacle> obstacles;
	std::vector<Alien> aliens;
	std::vector<Alien> CreateAliens();
	void MoveAliens();
	void MoveDownAliens(int distance);
	void CheckForCollisions();
	void CheckForHighScore();
	void SaveHighScoreToFile(int highScore);
	int loadHighScoreFromFile();
	int aliensDirection;
	std::vector<Laser> alienLasers;
	void AlienShootLaser();
	void GameOver();
	void Reset();
	void InitGame();
	void NextLevel();
	int CalculateHealth(int alienType);
	constexpr static float alienLaserShootInterval = 0.350;
	float timeLastAlienFired;
	MysteryShip mysteryship;
	float mysteryShipSpawnInterval;
	float timeLastSpawn;
};