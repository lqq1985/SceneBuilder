#ifndef SYS_OPENGL_H
#define SYS_OPENG_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace SysOpenGLSetting {
	void defaults(SDL_Window* &window, unsigned int screenWidth, unsigned int screenHeight);
	void depthTest(bool enable);
	void culling(bool enable);
	void wireframe(bool enable);
	void mouseCapture(bool enable);
	void mouseCursor(bool enable);
	void mouseCenter(SDL_Window* window, unsigned int width, unsigned int height);
}

#endif