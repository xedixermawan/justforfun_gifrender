#ifndef _JFF_TEXTUREHELPER_H_
#define _JFF_TEXTUREHELPER_H_

#include <string>


namespace jff   {
	enum textype   {
		DDS,
		PNG,
		JPG,
		RGBA8888,
		RGBA4444
	};	


	class texhelper   {
	public:
		static unsigned char* loadfile(const std::string& filename,unsigned int& length);
		static textype gettextype(unsigned char* ba);
		static unsigned char* pngtorgba8888( unsigned char* ba, unsigned int& w, unsigned int& h );
	};
};
#endif  // _JFF_TEXTUREHELPER_H_