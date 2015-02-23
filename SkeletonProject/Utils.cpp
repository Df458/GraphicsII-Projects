#include "Utils.h"
#include <cstdio>

char* loadFileContents(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if(!file) {
        fprintf(stderr, "Failed to load %s: File not found.", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* filedata = new char[filesize];
    fread(filedata, 1, filesize, file);
    filedata[filesize - 1] = '\0';

    fclose(file);
    return filedata;
}
