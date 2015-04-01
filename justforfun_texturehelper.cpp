#include "justforfun_texturehelper.h"
#include <fstream>
#include <assert.h>
#include "png.h"

namespace jff   {

unsigned char* texhelper::loadfile(const std::string& filename,unsigned int& length)   {
	unsigned char* f_bytearray = NULL;
	// open the file
    std::ifstream fileopened(filename, std::ios::in | std::ios::binary | std::ios::ate);
    
    // if open was successful
	bool is_open_ok = fileopened.is_open() ;
	assert( is_open_ok ) ;
    if(is_open_ok)
    {
        // find the length of the file
        int f_length = (int)fileopened.tellg();
		length = f_length;
        // collect the file data
        f_bytearray = new unsigned char[f_length];
        fileopened.seekg(0, std::ios::beg);
        fileopened.read(reinterpret_cast<char*>(f_bytearray), f_length);
        fileopened.close();
    }
    return f_bytearray;
}

textype texhelper::gettextype(unsigned char* ba)   {
	return textype::DDS;
}

unsigned char* texhelper::pngtorgba8888(unsigned char* ba,unsigned int& w,unsigned int& h)   {
	return 0;
}

}
