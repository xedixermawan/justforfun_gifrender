#include "justforfun_texture.h"
#include "justforfun_texturehelper.h"
#include "justforfun_textureloader.h"

namespace jff   {

texture::texture(const std::string& filename,ID3D11Device* device)   {
	unsigned int file_length = 0;
	unsigned char* file_ba   = texhelper::loadfile(filename,file_length);
	textype filetextype      = texhelper::gettextype(file_ba);

	switch(filetextype)   {
		case textype::DDS :
				CreateDDSTextureFromMemory( device, file_ba, file_length, nullptr, &m_textureview, MAXSIZE_T );
			break;
		case textype::PNG :   {
				unsigned int width,height;
				unsigned char* ba_rgba8888 = texhelper::pngtorgba8888( file_ba, width, height );
				CreateRawTextureFromMemory( device, &m_textureview , (void*) ba_rgba8888 , jff::RAW_8888 , width , height );
			}
			break;
		case textype::JPG :
			//CreateRawTextureFromMemory();
			break;
	};
	delete[] file_ba;
}

texture::texture(unsigned char * image,unsigned int w, unsigned int h, ID3D11Device* device)
{
	CreateRawTextureFromMemory(device, &m_textureview, (void*)image, jff::RAW_8888, w, h);
}

texture::~texture()   {
	delete m_textureview;
}

}