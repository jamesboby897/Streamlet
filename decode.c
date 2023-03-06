#include "decode.h"

OpusDecoder* decoder;

static uint32_t readUInt32be(const unsigned char *bytes, int p) {
	unsigned int a = bytes[(p)++];
	unsigned int b = bytes[(p)++];
	unsigned int c = bytes[(p)++];
	unsigned int d = bytes[(p)++];
	return a << 24 | b << 16 | c << 8 | d;
}
void* decodeFrame(unsigned char* frame,struct file_header file_header, int* type){
	uint32_t frame_magic = readUInt32be(frame,0);
	switch(frame_magic){
		case 0x6C766666:
			void* data = decode_LVF_frame();
			type = 1;
			return data;
			break;
		case 0x4F505553:
			void* data = decode_OPUS_frame();
			type = 2;
			return data;
			break;
		case 0x77617665:
			void* data = decode_WAVE_frame();
			type = 3;
			return data;
			break;
		default:
			type = 0;
			return NULL;
	}
}

static void* decode_LVF_frame(){

}

static void* decode_OPUS_frame(){

}

static void* decode_WAVE_frame(){

}