#include "Utils.h"
#include <cstdio>

char* loadFileContents(const char* filepath) {
    FILE* file = fopen((getPath() + filepath).c_str(), "rb");
    if(!file) {
        fprintf(stderr, "Failed to load %s: File not found.", (getPath() + filepath).c_str());
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

std::string getPath(){
	char buf[1024];
    GetModuleFileName(NULL, buf, 1024);
    std::string::size_type pos = std::string(buf).find_last_of("\\/");
    return std::string(buf).substr(0, pos + 1);
}
