
#ifndef CPP_PROJECT_EPSILON_MAPPER_H
#define CPP_PROJECT_EPSILON_MAPPER_H

#include <vector>
#include <iostream>

class EpsilonMapper {

public:
    static std::vector<int> getEpsilonsVector(std::string dataset_name, std::string dataset_filename, int error_param);

private:
    static void checkErrors(std::string dataset_name, int error_param);
    static void throwInvalidFilenameError(std::string dataset_name, std::string dataset_filename);
};

#endif //CPP_PROJECT_EPSILON_MAPPER_H
