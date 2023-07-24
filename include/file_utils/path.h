
#ifndef CPP_PROJECT_PATH_H
#define CPP_PROJECT_PATH_H

#include <string>
#include <iostream>

class Path{

public:
    std::string file_path;
    std::string file_filename;
    std::string full_path;

    Path(){
        file_path = "";
        file_filename = "";
        full_path = file_path + '/' + file_filename;
    }

    Path(std::string file_path_, std::string file_filename_){
        file_path = file_path_;
        file_filename = file_filename_;
        full_path = file_path + '/' + file_filename;
    }

    Path(std::string full_path_){
        try {
            // SOURCE: http://www.cplusplus.com/reference/string/string/find_last_of/
            std::size_t found = full_path_.find_last_of("/\\");
            file_path = full_path_.substr(0, found);
            file_filename = full_path_.substr(found + 1);
            full_path = file_path + '/' + file_filename;
        }
        catch(...){
            throw std::invalid_argument("Invalid path: \"" + full_path_ + "\".");
        }
    }
};

#endif //CPP_PROJECT_PATH_H
