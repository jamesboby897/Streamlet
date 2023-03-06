#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opus.h>
#include <qoi.h>
#define FRAME_SIZE 960
#define SAMPLE_RATE 48000
#define CHANNELS 2
#define APPLICATION OPUS_APPLICATION_AUDIO
#define BITRATE 96000
#define MAX_FRAME_SIZE 6*960
#define MAX_PACKET_SIZE (3*1276)
struct file_header{
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint16_t framerate;
    uint16_t enc_version;
};

struct frame_header{
    uint32_t magic;
    uint32_t size;
};

void writeUInt32be(unsigned char* bytes, uint32_t value,int p){
    bytes[p++] = (0xff000000 & value) >> 24;
    bytes[p++] = (0x00ff0000 & value) >> 16;
    bytes[p++] = (0x0000ff00 & value) >> 8;
    bytes[p++] = (0x000000ff & value);
}
