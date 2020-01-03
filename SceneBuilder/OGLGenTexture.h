#ifndef OGL_GEN_TEXTURE_H
#define OGL_GEN_TEXTURE_H

#include <GL/glew.h>
#include <vector>
#include "LdrStbiWrapper.h"

namespace OglGenTexture {
	void cubemap(std::vector<const char*> filenames, GLuint& textureID, int& width, int& height, int& nrChannels);
	bool bind2dNearest(const char * filename, unsigned int &textureID);
	bool bind2dLinear(const char * filename, unsigned int &textureID);
}

#endif