
#include "text_utils.h"

#include <fstream>
#include <iostream>
#include <vector>


int TextUtils::lineCount(std::string path, std::string filename){
    std::string full_path = path + '/' + filename;
    int line_count = 0;
    std::string line;
    std::ifstream file(full_path);
    while (std::getline(file, line)) ++line_count;
    file.close();
    return line_count;
}
