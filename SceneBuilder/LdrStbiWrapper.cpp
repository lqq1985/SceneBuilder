#include "LdrStbiWrapper.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char * LdrStbiWrapper::load(const char * filename, int & width, int & height, int & nrChannels, int desired_channels)
{
	return stbi_load(filename, &width, &height, &nrChannels, desired_channels);
}

void LdrStbiWrapper::free(unsigned char * data)
{
	stbi_image_free(data);
}
