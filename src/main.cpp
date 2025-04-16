#include <iostream>
#include <string>
#include "raylib.h"
#include "game.h"
#include "obstacle.h"
#include "alien.h"

std::string FormatWithLeadingZeros(int number, int width){
	std::string numberText = std::to_string(number);
	int leadingZeros = width - numberText.length();
	numberText = std::string(leadingZeros, '0') + numberText;
	return numberText;
}

int main() {

	Color grey = {29, 29, 27, 255};
    Color yellow = {243, 216, 63, 255};
	int offset = 50;
	constexpr int windowWidth = 750;
	constexpr int windowHeight = 700;

	InitAudioDevice();
	SetAudioStreamBufferSizeDefault(4096);  // fixes lag
	InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Raylib Space Invaders");
	Font font  = LoadFontEx("assets/Font/monogram.ttf", 64, 0, 0);
	Texture2D spaceshipImage = LoadTexture("assets/Graphics/spaceship.png");
	Game game;

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		UpdateMusicStream(game.music);
		game.HandleInput();
		game.Update();
		BeginDrawing();
		ClearBackground(grey);
		DrawRectangleRoundedLinesEx({10, 10, 780, 780}, 0.18f, 20, 2, yellow);
		DrawLineEx({25, 730}, {775, 730}, 3, yellow);

		if(game.isRunning){
			std::string level = "LEVEL " + FormatWithLeadingZeros(game.level, 2);
            DrawTextEx(font, level.c_str(), {570, 740}, 34, 2, yellow);
        } else {
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, yellow);
			DrawTextEx(font, "PRESS ENTER TO RESTART", {100, 750}, 15, 2, yellow);
        }
		float x = 50;
		for(int i = 0; i < game.lives; i++){
			DrawTextureV(spaceshipImage, {x , 745}, WHITE);
			x += 50;
		}
		DrawTextEx(font, "SCORE", {50, 15}, 34, 2, yellow);
		std::string scoreText = FormatWithLeadingZeros(game.score, 6);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, yellow);

		DrawTextEx(font, "HIGH-SCORE", {570, 15}, 34, 2, yellow);
        std::string highscoreText = FormatWithLeadingZeros(game.highScore, 6);
        DrawTextEx(font, highscoreText.c_str(), {620, 40}, 34, 2, yellow);

		game.Draw();
		EndDrawing();
	}
	CloseWindow();
	CloseAudioDevice();
	return 0;
}