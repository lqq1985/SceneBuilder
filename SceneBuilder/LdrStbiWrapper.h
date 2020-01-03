#ifndef LDR_STBI_WRAPPER_H
#define LDR_STBI_WRAPPER_H

namespace LdrStbiWrapper
{
	enum
	{
		STBI_default = 0,
		STBI_grey = 1,
		STBI_grey_alpha = 2,
		STBI_rgb = 3,
		STBI_rgb_alpha = 4
	};

	unsigned char *load(const char *filename, int &width, int &height, int &nrChannels, int desired_channels);
	void free(unsigned char * data);
}

#endif 