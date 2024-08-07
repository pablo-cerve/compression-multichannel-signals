#include "datetime_utils.h"
#include "main_helper.h"
#include "tests.h"
#include "assert.h"

int main(int argc, char *argv[]){
    try {
        if (argc == 1) { throw std::invalid_argument("Missing arguments."); }

        std::string action = argv[1];
        MainHelper::checkAction(action); // check that action is "code", "decode" or "test"

        if (action == "test") {
            if (argc > 3) { throw std::invalid_argument("Too many arguments for test."); }

            std::string full_dataset_path;
            if (argc == 3){ full_dataset_path = argv[2]; }
            Tests::runAll(full_dataset_path);
            return 0;
        }

        // action == "code" || action == "decode"
        if (argc < 4) { throw std::invalid_argument("Missing arguments for " + action + "."); }

        std::string full_input_path = argv[2];
        std::string full_output_path = argv[3];

        if (action == "decode"){
            if (argc > 4) { throw std::invalid_argument("Too many arguments for decode."); }

            MainHelper::decode(full_input_path, full_output_path);
            return 0;
        }

        // action == "code"
        if (argc < 5) { throw std::invalid_argument("Missing arguments for code."); }

        std::string coder_name = argv[4];
        Constants::getCoderValue(coder_name); // check that coder_name is valid

        // default coder params
        int window_size = 1;
        std::vector<int> epsilons_vector;

        bool is_lossless = Constants::isLosslessCoder(coder_name);
        bool requires_window_size = Constants::requiresWindowSize(coder_name);

        if (is_lossless) {
            if (!requires_window_size){
                if (argc > 5) { throw std::invalid_argument("Too many arguments for code with " + coder_name + "."); }
                MainHelper::code(coder_name, full_input_path, full_output_path, window_size, epsilons_vector);
                return 0;
            }
            else if (argc < 6) {
                throw std::invalid_argument("Missing arguments for code with " + coder_name + ".");
            }
        }

        assert(requires_window_size);
        window_size = atoi(argv[5]);

        if (is_lossless){
            if (argc > 6) { throw std::invalid_argument("Too many arguments for code with " + coder_name + "."); }
            MainHelper::code(coder_name, full_input_path, full_output_path, window_size, epsilons_vector);
            return 0;
        }

        // !is_lossless && requires_window_size
        if (argc < 8) { throw std::invalid_argument("Missing arguments for code with " + coder_name + "."); }

        // check that error_mode is "error_mode=epsilon" or "error_mode=e"
        std::string error_mode = argv[6];
        bool epsilonErrorMode = MainHelper::parseErrorMode(error_mode);

        if (epsilonErrorMode) {
            // Note: epsilons_vector doesn't include the epsilon for timestamps column, since it's always 0
            for (int i = 7; i < argc; i++) { epsilons_vector.push_back(atoi(argv[i])); }
            MainHelper::checkEpsilonsVectorLength(epsilons_vector, full_input_path);
        }
        else {
            int error_param = atoi(argv[7]);
            epsilons_vector = MainHelper::getEpsilonsVector(error_param, full_input_path);
        }
        MainHelper::code(coder_name, full_input_path, full_output_path, window_size, epsilons_vector);
        return 0;
    }
    catch(std::exception& e) {
        std::cout << "ERROR: " + std::string(e.what()) << std::endl;
        return -1;
    }
}
