#include "SysOpenGLSetting.h"

void SysOpenGLSetting::defaults(SDL_Window*& window, unsigned int screenWidth, unsigned int screenHeight)
{
	SysOpenGLSetting::depthTest(true);
	SysOpenGLSetting::culling(true);
	SysOpenGLSetting::mouseCursor(true);
	SysOpenGLSetting::mouseCapture(true);
	SysOpenGLSetting::mouseCenter(window, screenWidth, screenHeight);
	SysOpenGLSetting::wireframe(false);
}

void SysOpenGLSetting::depthTest(bool enable)
{
	enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void SysOpenGLSetting::culling(bool enable)
{
	if (enable)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		return;
	}

	glDisable(GL_CULL_FACE);
}

void SysOpenGLSetting::wireframe(bool enable)
{
	enable ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SysOpenGLSetting::mouseCapture(bool enable)
{
	enable ? SDL_CaptureMouse(SDL_TRUE) : SDL_CaptureMouse(SDL_FALSE);
}

void SysOpenGLSetting::mouseCursor(bool enable)
{
	enable ? SDL_ShowCursor(1) : SDL_ShowCursor(0);
}

void SysOpenGLSetting::mouseCenter(SDL_Window* window, unsigned int width, unsigned int height)
{
	SDL_WarpMouseInWindow(window, width / 2.0f, height / 2.0f);
}
