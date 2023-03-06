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
}file_header;

struct frame_header{
    uint32_t magic;
    uint32_t size;
}frame_header;

FILE* infile;
FILE* outfile;

void writeHeader(unsigned char *bytes, uint32_t width, uint32_t height);

int main(int argc, char** argv){
    if(argc != 3){
        printf("Usage: lvfdec in_file out_file.lvf\n");
        return 1;
    }

    if(argv[1][0]!='-')
        infile = fopen(argv[1],"rb");
    else
        infile = stdin;

    fread(&file_header,sizeof(file_header),1,infile);
    if(file_header.magic!=0x44414548){
        printf("File not recognised\n");
        return 1;
    }

    if(argv[2][0]!='-')
        outfile = fopen(argv[5],"wb");
    else
        outfile = stdout;

    qoi_desc desc = {0,0,0,0};

    while(1){
        fread(&frame_header,sizeof(frame_header),1,infile);
        if(frame_header.magic!=0x59444F42){
            printf("Could not read frame\n");
            return 1;
        }
        unsigned char* buff = malloc(frame_header.size+14);
        writeHeader(buff,file_header.width,file_header.height);
        fread(buff+14,1,frame_header.size,infile);
        unsigned char* rgb_data = qoi_decode(buff,frame_header.size+14,&desc,3);
        free(buff);
        int size = file_header.height*file_header.width*3;
        fwrite(rgb_data,size,1,outfile);
        free(rgb_data);
        if(feof(infile))
            break;
    }

    if(argv[1][0]!='-')
        fclose(infile);
    if(argv[2][0]!='-')
        fclose(outfile);
    else
        printf("Resolution: %dx%d framerate:%d fps\n",file_header.width,file_header.height,file_header.framerate);
    return 0;
}

void writeHeader(unsigned char *bytes, uint32_t width, uint32_t height) {
    int p=0;
    bytes[p++] = 0x71;
    bytes[p++] = 0x6F;
    bytes[p++] = 0x69;
    bytes[p++] = 0x66;
	bytes[p++] = (0xff000000 & width) >> 24;
	bytes[p++] = (0x00ff0000 & width) >> 16;
	bytes[p++] = (0x0000ff00 & width) >> 8;
	bytes[p++] = (0x000000ff & width);
	bytes[p++] = (0xff000000 & height) >> 24;
	bytes[p++] = (0x00ff0000 & height) >> 16;
	bytes[p++] = (0x0000ff00 & height) >> 8;
	bytes[p++] = (0x000000ff & height);
	bytes[p++] = 3;
	bytes[p++] = 0;
}