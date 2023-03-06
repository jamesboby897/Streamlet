#include <stdio.h>
#define QOI_IMPLEMENTATION
#include <qoi.h>
#include <stdlib.h>
struct file_header{
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint16_t framerate;
    uint16_t enc_version;
}file_header = {0x44414548,0,0,0,0};

struct frame_header{
    uint32_t magic;
    uint32_t size;
}frame_header = {0x59444F42,0};

FILE* outfile;
FILE* infile;

int main(int argc, char** argv){
    if(argc != 6){
        printf("Usage: lvfenc in_file width height framerate out_file.lvf\n");
        return 1;
    }

    int width = strtol(argv[2],NULL,10);
    int height = strtol(argv[3],NULL,10);
    int fps = strtol(argv[4],NULL,10);

    const unsigned int raw_frame_size = height*width*3;

    file_header.width = width;
    file_header.height = height;
    file_header.framerate = fps;

    qoi_desc desc = {width,height,3,0};

    if(argv[5][0]!='-')
        outfile = fopen(argv[5],"wb");
    else
        outfile = stdout;
    fwrite(&file_header,sizeof(file_header),1,outfile);

    if(argv[1][0]!='-')
        infile = fopen(argv[1],"rb");
    else
        infile = stdin;
    
    unsigned char* buff = malloc(raw_frame_size);
    while(fread(buff,1,raw_frame_size,infile)==raw_frame_size){
        unsigned char* qoi_data = qoi_encode(buff,&desc,(int*)&frame_header.size);
        frame_header.size-=14;
        fwrite(&frame_header,sizeof(frame_header),1,outfile);
        fwrite(qoi_data+14,1,frame_header.size,outfile);
        free(qoi_data);
    }
    free(buff);
    if(argv[1][0]!='-')
        fclose(infile);
    if(argv[5][0]!='-')
        fclose(outfile);
    return 0;
}