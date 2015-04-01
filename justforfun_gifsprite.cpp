#include "justforfun_gifsprite.h"

unsigned char *load_file(const char *path, size_t *data_size)
{
	FILE *fd;
	struct stat sb;
	unsigned char *buffer;
	size_t size;
	size_t n;

	fd = fopen(path, "rb");
	if (!fd) {
		perror(path);
		exit(EXIT_FAILURE);
	}

	if (stat(path, &sb)) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	size = sb.st_size;

	buffer = (unsigned char *) malloc(size);
	if (!buffer) {
		fprintf(stderr, "Unable to allocate %lld bytes\n",
			(long long)size);
		exit(EXIT_FAILURE);
	}

	n = fread(buffer, 1, size, fd);
	if (n != size) {
		perror(path);
		exit(EXIT_FAILURE);
	}

	fclose(fd);

	*data_size = size;
	return buffer;
}


void warning(const char *context, gif_result code)
{
	fprintf(stderr, "%s failed: ", context);
	switch (code)
	{
	case GIF_INSUFFICIENT_FRAME_DATA:
		fprintf(stderr, "GIF_INSUFFICIENT_FRAME_DATA");
		break;
	case GIF_FRAME_DATA_ERROR:
		fprintf(stderr, "GIF_FRAME_DATA_ERROR");
		break;
	case GIF_INSUFFICIENT_DATA:
		fprintf(stderr, "GIF_INSUFFICIENT_DATA");
		break;
	case GIF_DATA_ERROR:
		fprintf(stderr, "GIF_DATA_ERROR");
		break;
	case GIF_INSUFFICIENT_MEMORY:
		fprintf(stderr, "GIF_INSUFFICIENT_MEMORY");
		break;
	default:
		fprintf(stderr, "unknown code %i", code);
		break;
	}
	fprintf(stderr, "\n");
}


void *bitmap_create(int width, int height)
{
	return calloc(width * height, 4);
}


void bitmap_set_opaque(void *bitmap, bool opaque)
{
	(void)opaque;  /* unused */
	assert(bitmap);
}


bool bitmap_test_opaque(void *bitmap)
{
	assert(bitmap);
	return false;
}


unsigned char *bitmap_get_buffer(void *bitmap)
{
	assert(bitmap);
	return (unsigned char *) bitmap;
}


void bitmap_destroy(void *bitmap)
{
	assert(bitmap);
	free(bitmap);
}


void bitmap_modified(void *bitmap)
{
	assert(bitmap);
	return;
}

using namespace jff;

gifsprite::gifsprite(const char* gifname, ID3D11Device* device, DirectX::SpriteBatch* spritebatch)
{
	// init 
	m_device      = device;
	m_spritebatch = spritebatch;
	m_current_frame = 0;
	m_increment = 0.0f;
	m_frame_count = 0;
	// decode gif
	gif_bitmap_callback_vt bitmap_callbacks = {
		bitmap_create,
		bitmap_destroy,
		bitmap_get_buffer,
		bitmap_set_opaque,
		bitmap_test_opaque,
		bitmap_modified
	};
	gif_animation gif;
	size_t size;
	gif_result code;
	unsigned int i;

	/* create our gif animation */
	gif_create(&gif, &bitmap_callbacks);

	/* load file into memory */
	unsigned char *data = load_file(gifname, &size);

	/* begin decoding */
	do {
		code = gif_initialise(&gif, size, data);
		if (code != GIF_OK && code != GIF_WORKING) {
			warning("gif_initialise", code);
			exit(1);
		}
	} while (code != GIF_OK);

	printf("P3\n");
	printf("# %s\n", gifname);
	printf("# width                %u \n", gif.width);
	printf("# height               %u \n", gif.height);
	printf("# frame_count          %u \n", gif.frame_count);
	printf("# frame_count_partial  %u \n", gif.frame_count_partial);
	printf("# loop_count           %u \n", gif.loop_count);
	printf("%u %u 256\n", gif.width, gif.height * gif.frame_count);

	m_frame_count = gif.frame_count;

	/* decode the frames */
	for (i = 0; i != gif.frame_count; i++) {
		unsigned int row, col;
		unsigned char *image;

		code = gif_decode_frame(&gif, i);
		if (code != GIF_OK)
			warning("gif_decode_frame", code);

		//printf("# frame %u:\n", i);
		image = (unsigned char *)gif.frame_image;
		
		
		for (row = 0; row != gif.height; row++) {
			for (col = 0; col != gif.width; col++) {
				size_t z = (row * gif.width + col) * 4;
				/*printf("%u %u %u ",
					(unsigned char)image[z],    //r
					(unsigned char)image[z + 1],  //g
					(unsigned char)image[z + 2]); //b
					*/
				unsigned char temp_ = image[z];
				image[z]  = image[z + 2];
				image[z + 2] = temp_;
			}
			//printf("\n");
		}
		
		gifdeesc* _gifdesc = new gifdeesc;
		texture* tex_frame_i = new texture(image, gif.width, gif.height, m_device);
		_gifdesc->ptrtex = tex_frame_i;
		_gifdesc->width  = gif.width;
		_gifdesc->height = gif.height;
		_gifdesc->framedelay = gif.frames[i].frame_delay;// .frames->frame_delay; ->frames[frame].frame_delay
		m_vectextures.push_back( _gifdesc );
	}

	/* clean up */
	gif_finalise(&gif);
	free(data);
}

void gifsprite::render()
{
	RECT rect1;
	rect1.top = 0;
	rect1.left = 0;
	rect1.bottom = m_vectextures[m_current_frame]->height;
	rect1.right = m_vectextures[m_current_frame]->width;

	m_spritebatch->Draw(m_vectextures[m_current_frame]->ptrtex->gettexview(), DirectX::XMFLOAT2(0, 0), &rect1, DirectX::Colors::White, 0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::SpriteEffects_None, 0.0f);
}

void gifsprite::update(const double delta_time)
{
	m_increment += delta_time;
	if (m_increment > 0.01*m_vectextures[m_current_frame]->framedelay )
	{
		m_increment = 0.0f;
		m_current_frame += 1;
		if (m_current_frame > m_frame_count-2)
		{
			m_current_frame = 0;
		}
	}
}

gifsprite::~gifsprite()
{

}