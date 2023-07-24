
#ifndef CPP_PROJECT_BIT_STREAM_UTILS_H
#define CPP_PROJECT_BIT_STREAM_UTILS_H

#include "bit_stream_reader.h"
#include <string>
#include "path.h"

class BitStreamUtils {

public:
    static BitStreamReader* open(Path path);
    // Compares two files, returns 0 if they match.
    // Otherwise it returns the index of the first different bit.
    static int compare(Path path1, Path path2);
    static int compareBytes(Path path1, Path path2);
    static int getSize(Path path);
    static void printBytes(Path path);
    static void removeFile(Path path);
};

#endif //CPP_PROJECT_BIT_STREAM_UTILS_H
