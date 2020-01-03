#include "Input.h"

using namespace std;

Input::Input()
{
}

Input::~Input()
{
}

void Input::update(float deltaTime)
{
	while (SDL_PollEvent(&this->event)) {
		this->mouseMotion = false;

		switch (this->event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_w && event.key.repeat == 0) { this->w = true; }
			if (event.key.keysym.sym == SDLK_s && event.key.repeat == 0) { this->s = true; }
			if (event.key.keysym.sym == SDLK_a && event.key.repeat == 0) { this->a = true; }
			if (event.key.keysym.sym == SDLK_d && event.key.repeat == 0) { this->d = true; }
			if (event.key.keysym.sym == SDLK_o && event.key.repeat == 0) { this->o = true; }
			if (event.key.keysym.sym == SDLK_p && event.key.repeat == 0) { this->p = true; }
			if (event.key.keysym.sym == SDLK_z && event.key.repeat == 0) { this->z = true; }
			if (event.key.keysym.sym == SDLK_x && event.key.repeat == 0) { this->x = true; }
			if (event.key.keysym.sym == SDLK_l && event.key.repeat == 0) { this->l = true; }
			// arrow keys
			if (event.key.keysym.sym == SDLK_UP && event.key.repeat == 0) { this->upArrow = true; }
			if (event.key.keysym.sym == SDLK_DOWN && event.key.repeat == 0) { this->downArrow = true; }
			if (event.key.keysym.sym == SDLK_LEFT && event.key.repeat == 0) { this->leftArrow = true; }
			if (event.key.keysym.sym == SDLK_RIGHT && event.key.repeat == 0) { this->rightArrow = true; }
			// left shift
			if (event.key.keysym.sym == SDLK_LSHIFT && event.key.repeat == 0) { this->lShift = true; }
			// tag
			if (event.key.keysym.sym == SDLK_TAB && event.key.repeat == 0) { this->tab = true; }
			// quit
			if (event.key.keysym.sym == SDLK_ESCAPE) { this->quit = true; }
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_w && event.key.repeat == 0) { this->w = false; }
			if (event.key.keysym.sym == SDLK_s && event.key.repeat == 0) { this->s = false; }
			if (event.key.keysym.sym == SDLK_a && event.key.repeat == 0) { this->a = false; }
			if (event.key.keysym.sym == SDLK_d && event.key.repeat == 0) { this->d = false; }
			if (event.key.keysym.sym == SDLK_o && event.key.repeat == 0) { this->o = false; }
			if (event.key.keysym.sym == SDLK_p && event.key.repeat == 0) { this->p = false; }
			if (event.key.keysym.sym == SDLK_z && event.key.repeat == 0) { this->z = false; }
			if (event.key.keysym.sym == SDLK_x && event.key.repeat == 0) { this->x = false; }
			if (event.key.keysym.sym == SDLK_l && event.key.repeat == 0) { this->l = false; }
			// arrow keys
			if (event.key.keysym.sym == SDLK_UP && event.key.repeat == 0) { this->upArrow = false; }
			if (event.key.keysym.sym == SDLK_DOWN && event.key.repeat == 0) { this->downArrow = false; }
			if (event.key.keysym.sym == SDLK_LEFT && event.key.repeat == 0) { this->leftArrow = false; }
			if (event.key.keysym.sym == SDLK_RIGHT && event.key.repeat == 0) { this->rightArrow = false; }
			// left shift
			if (event.key.keysym.sym == SDLK_LSHIFT && event.key.repeat == 0) { this->lShift = false; }
			// tab
			if (event.key.keysym.sym == SDLK_TAB && event.key.repeat == 0) { this->tab = false; }
			break;
		case SDL_MOUSEMOTION:
			this->mouseMotion = true;
		default:
			break;
		}
	}
}

bool Input::isW() { return this->w; }
bool Input::isS() { return this->s; }
bool Input::isA() { return this->a; }
bool Input::isD() { return this->d; }
bool Input::isO() { return this->o; }
bool Input::isP() { return this->p; }
bool Input::isZ() { return this->z; }
bool Input::isX() { return this->x; }
bool Input::isL() { return this->l; }
bool Input::isUpArrow() { return this->upArrow; }
bool Input::isDownArrow() { return this->downArrow; }
bool Input::isLeftArrow() { return this->leftArrow; }
bool Input::isRightArrow() { return this->rightArrow; }
bool Input::isMouseMotion() { return this->mouseMotion; }
bool Input::isLShift() { return this->lShift; }
bool Input::isTab() { return this->tab; }
bool Input::isQuit() { return this->quit; }
