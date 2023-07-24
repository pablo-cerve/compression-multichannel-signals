
#include "bit_stream_utils.h"

#include <iostream>
#include <stdio.h>

BitStreamReader* BitStreamUtils::open(Path path){
    try {
        return new BitStreamReader(path);
    }
    catch( const std::runtime_error& e ){
        std::cout << e.what() << std::endl;
        exit(-1);
    }
}

int BitStreamUtils::compare(Path path1, Path path2){
    BitStreamReader* reader1 = open(path1);
    BitStreamReader* reader2 = open(path2);

    int cont=1; // first different bit

    while (!reader1->reachedEOF()){
        if (reader2->reachedEOF()) return cont;
        if (reader1->getBit()!=reader2->getBit()) return cont;
        cont++;
    }
    if (!reader2->reachedEOF()) return cont;

    return 0;
}

int BitStreamUtils::compareBytes(Path path1, Path path2){
    BitStreamReader* reader1 = open(path1);
    BitStreamReader* reader2 = open(path2);

    int cont=1; // first different bit
    bool diff = false;

    while (!reader1->reachedEOF()){
        if (reader2->reachedEOF()) { diff = true; break; }
        if (reader1->getInt(8)!=reader2->getInt(8)) { diff = true; break; }
        cont++;
    }
    if (!diff && !reader2->reachedEOF()) { diff = true; }

    delete reader1;
    delete reader2;

    cont = (diff ? cont : 0);
    return cont;
}

int BitStreamUtils::getSize(Path path){
    int size = 0;
    BitStreamReader* reader = open(path);
    while (!reader->reachedEOF()){
        reader->getInt(8);
        size++;
    }
    delete reader;
    return size;
}

void BitStreamUtils::printBytes(Path path){
    std::cout << "printBytes " << path.full_path << std::endl;
    BitStreamReader* reader = open(path);
    int cont = 1;
    int byte;
    while (!reader->reachedEOF()){
        byte = reader->getInt(8);
        std::cout << cont << " - " << byte << std::endl;
        cont++;
    }
    delete reader;
}

void BitStreamUtils::removeFile(Path path){
    const char * file_path = path.full_path.c_str();
    remove(file_path);
}
