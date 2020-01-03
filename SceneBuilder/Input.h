#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <iostream>

class Input
{
public:
	Input();
	~Input();
	int mouseX;
	int mouseY;
	void update(float deltaTime);
	bool isW();
	bool isS();
	bool isA();
	bool isD();
	bool isO();
	bool isP();
	bool isZ();
	bool isX();
	bool isL();
	bool isUpArrow();
	bool isDownArrow();
	bool isLeftArrow();
	bool isRightArrow();
	bool isMouseMotion();
	bool isTab();
	bool isLShift();
	bool isQuit();
private:
	SDL_Event event;
	bool w = false;
	bool s = false;
	bool a = false;
	bool d = false;
	bool o = false;
	bool p = false;
	bool x = false;
	bool z = false;
	bool l = false;
	bool upArrow = false;
	bool downArrow = false;
	bool leftArrow = false;
	bool rightArrow = false;
	bool lShift = false;
	bool mouseMotion = false;
	bool tab = false;
	bool quit = false;
};

#endif
