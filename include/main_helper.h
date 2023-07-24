
#ifndef CPP_PROJECT_MAIN_HELPER_H
#define CPP_PROJECT_MAIN_HELPER_H

#include <string>
#include "path.h"
#include <vector>
#include "dataset.h"
#include "constants.h"
#include "csv_reader.h"
#include "csv_writer.h"
#include "bit_stream_reader.h"
#include "bit_stream_writer.h"

class MainHelper {

public:
    static void checkAction(std::string action);
    static bool parseErrorMode(std::string error_mode);
    static void checkEpsilonsVectorLength(std::vector<int> epsilons_vector, std::string full_input_path);
    static std::vector<int> getEpsilonsVector(int error_param, std::string full_input_path);

    static Dataset* code(std::string coder_name,
                         std::string full_input_path, std::string full_output_path,
                         int window_size,
                         std::vector<int> epsilons_vector);
    static void decode(std::string full_input_path, std::string full_output_path);

private:
    static CSVReader* getCSVReader(std::string full_path);
    static CSVWriter* getCSVWriter(std::string full_path);
    static BitStreamReader* getBitStreamReader(std::string full_path);
    static BitStreamWriter* getBitStreamWriter(std::string full_path);
    static std::vector<int> removeNegativeNumbers(std::vector<int> epsilons_vector);
};

#endif //CPP_PROJECT_MAIN_HELPER_H
