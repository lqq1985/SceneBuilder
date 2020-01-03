#include "SysOpenGLInit.h"

int SystemOpenGLInit::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return SystemOpenGLInit::ERROR_SDL_INIT;
	}

	return 0;
}

int SystemOpenGLInit::defaults(SDL_Window*& window, SDL_GLContext& context, unsigned int screenWidth, unsigned int screenHeight, unsigned int sMinor, unsigned int sMajor)
{
	unsigned int engineError;

	if (engineError = SystemOpenGLInit::initSDL() > 0) return engineError;
	if (engineError = SystemOpenGLInit::setGlAttributes(sMinor, sMajor) > 0) return engineError;
	if (engineError = SystemOpenGLInit::initWindow(window, screenWidth, screenHeight) > 0) return engineError;
	if (engineError = SystemOpenGLInit::initContext(context, window) > 0) return engineError;
	if (engineError = SystemOpenGLInit::initGlew() > 0) return engineError;
}

int SystemOpenGLInit::setGlAttributes(unsigned int major, unsigned int minor)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return 0;
}

int SystemOpenGLInit::initWindow(SDL_Window*& window, unsigned int width, unsigned int height)
{
	window = SDL_CreateWindow(
		"Terrain Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (!window) {
		return SystemOpenGLInit::ERROR_CREATE_WINDOW;
	}
	return 0;
}

int SystemOpenGLInit::initContext(SDL_GLContext& context, SDL_Window*& window)
{
	context = SDL_GL_CreateContext(window);

	if (!context) {
		return SystemOpenGLInit::ERROR_CREATE_CONTEXT;
	}
	return 0;
}

int SystemOpenGLInit::initGlew()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err) {
		return SystemOpenGLInit::ERROR_GLEW_INIT;
	}

	return 0;
}

int SystemOpenGLInit::shutDown(SDL_Window*& window, SDL_GLContext& context)
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}