#ifndef _JFF_TEXTURE_H_
#define _JFF_TEXTURE_H_

#include <string>
#include <d3d11.h>
#include <memory>

namespace jff   {
	class texture   {
	public:
		texture(const std::string& filename,ID3D11Device* device);
		texture(unsigned char * image, unsigned int w, unsigned int h, ID3D11Device* device);
		~texture();
		ID3D11ShaderResourceView* gettexview()   {
			return m_textureview;
		};
	private:
		std::string m_name;
		unsigned int m_width;
		unsigned int m_height;
		ID3D11ShaderResourceView* m_textureview;

		void load();
	};
};  // namespace jff

#endif // _JFF_TEXTURE_H_