#ifndef _JFF_GIF_SPRITE_H_
#define _JFF_GIF_SPRITE_H_

#include <vector>
#include "justforfun_texture.h"
#include "SpriteBatch.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "libnsgif.h"

unsigned char *load_file(const char *path, size_t *data_size);
void warning(const char *context, int code);
void *bitmap_create(int width, int height);
void bitmap_set_opaque(void *bitmap, bool opaque);
bool bitmap_test_opaque(void *bitmap);
unsigned char *bitmap_get_buffer(void *bitmap);
void bitmap_destroy(void *bitmap);
void bitmap_modified(void *bitmap);

namespace jff   {
	struct gifdeesc
	{
		texture* ptrtex;
		unsigned int width;
		unsigned int height;
		unsigned int framedelay;
	};

	class gifsprite {
	public:
		gifsprite(const char* gifname, ID3D11Device* device, DirectX::SpriteBatch* spritebatch);
		~gifsprite();
		void render();
		void update(const double delta_time);
	private:
		std::vector< gifdeesc* > m_vectextures;
		ID3D11Device* m_device;
		DirectX::SpriteBatch* m_spritebatch;
		unsigned int m_current_frame;
		unsigned int m_frame_count;
		double m_increment;
	};
};

#endif // _JFF_GIF_SPRITE_H_