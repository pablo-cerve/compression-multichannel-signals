
#ifndef CPP_PROJECT_BIT_STREAM_WRITER_H
#define CPP_PROJECT_BIT_STREAM_WRITER_H

#include <stdio.h>
#include <string>
#include "path.h"

#define PRINT_BSW 0

class BitStreamWriter {

private:
    //
    // SOURCE: https://os.mbed.com/forum/helloworld/topic/2053/?page=1#comment-54746
    //
    union Float {
        float    m_float;
        uint8_t  m_bytes[sizeof(float)];
    };
    union Double {
        double   m_double;
        uint8_t  m_bytes[sizeof(double)];
    };
    union Integer {
        int   m_int;
        uint8_t  m_bytes[sizeof(int)];
    };

    // FILE * fp;
    unsigned char current, offset;
#if PRINT_BSW
    int current_byte;
#endif
    void construct(const char * file);
    void write();

public:
    FILE * fp;
    BitStreamWriter(Path path);

    void pushBit(unsigned int bit);
    void pushBits(unsigned int bit, int times);

    void pushInt(unsigned int x, int k);

    void pushFloat(float x);
    void pushDouble(double x);
    void pushInt(int x);

    int flushByte();

    void close();

    ~BitStreamWriter();

};

#endif //CPP_PROJECT_BIT_STREAM_WRITER_H
