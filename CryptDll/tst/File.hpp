#pragma once    //FILE* wrapper
//- takes care of closing on destroy
//- can be used with any functions expecting FILE*
#include <stdio.h>

class File{
private:
    FILE* _file;//file handle
public:
    File(const char* const filename=0, const char* const mode="rt")
        : _file(filename ? fopen(filename, mode) : 0)
    {}

    ~File(){
        if(_file)
            fclose(_file);
        _file = 0;
    }

    operator FILE*(){return _file;}//automatic conversion to FILE*

    File& operator()(const char* const filename, const char* const mode="rt"){
        this->~File();//close
        _file = fopen(filename, mode);
        return *this;
    }
};

#if 0//USAGE//================================================================================
     char buf[1024] = {0};
     File file("filename", "rt");
     if(!file)
         ;//ERR
     for(size_t len=0,n=0; n=fread(buf+len, 1, sizeof(buf), file); len+=n){
         buf[len+n] = 0;
         //...
     }
#endif