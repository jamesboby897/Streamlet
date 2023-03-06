#include "encode.h"

OpusEncoder* encoder;

void* encodeFrame(unsigned char* frame_data,int width, int height){
    uint32_t magic = 0x6C766666;
    uint32_t size=0;
    qoi_desc desc = {width,height,3,0};
    unsigned char* qoi_data = qoi_encode(frame_data,&desc,(int*)&size);
    memmove(qoi_data,qoi_data+6,size);
    size -= 22;
    writeUInt32be(qoi_data,magic,0);
    writeUInt32be(qoi_data,size,4);
    qoi_data = realloc(qoi_data,size+8);
    return qoi_data;
}

void init_opus_enc(){
    int err;
    encoder = opus_decoder_create(SAMPLE_RATE,CHANNELS,&err);
    if(err<0){
        fprintf(stderr,"Failed to create opus encoder\n");
        exit(-1);
    }
    err = opus_encoder_ctl(encoder,OPUS_BITRATE_MAX,BITRATE);
    if(err<0){
        fprintf(stderr,"Failed to set opus encoder bitrate\n");
        exit(-1);
    }
    err = opus_encoder_ctl(encoder,OPUS_SET_APPLICATION_REQUEST,APPLICATION);
    if(err<0){
        fprintf(stderr,"Failed to set opus encoder application\n");
        exit(-1);
    }
}

void* encode_opus_frame(unsigned char* audio_data,int* size){
    unsigned char* audio_data_le = malloc(MAX_FRAME_SIZE*CHANNELS*2);
    unsigned char* opus_data = malloc(MAX_FRAME_SIZE);
    for(int i=0;i<CHANNELS*FRAME_SIZE;i++){
        audio_data_le[i]=audio_data[2*i+1]<<8|audio_data[2*i];
    }
    size = opus_encode(encoder,audio_data_le,FRAME_SIZE,opus_data,MAX_PACKET_SIZE);
    if(size<0){
        fprintf(stderr,"encode failed: %s\n", opus_strerror(size));
        return NULL;
    }
    return opus_data;
}