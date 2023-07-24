
#include "csv_utils.h"

#include <fstream>
#include <iostream>
#include <vector>
#include "csv_reader.h"
#include "csv_writer.h"
#include "assert.h"
#include "vector_utils.h"
#include "string_utils.h"

void CSVUtils::CompareCSVLossless(Path path1, Path path2){
    CSVReader* csv_reader1 = new CSVReader(path1);
    CSVReader* csv_reader2 = new CSVReader(path2);
    while (csv_reader1->continue_reading) {
        std::vector<std::string> row1 = csv_reader1->readLineCSV();
        std::vector<std::string> row2 = csv_reader2->readLineCSV();
        if (row1 != row2){
            std::cout << "current_line " << csv_reader1->current_line_count << std::endl;
            std::cout << "line1 = " << StringUtils::join(row1, ".") << std::endl;
            std::cout << "line2 = " << StringUtils::join(row2, ".") << std::endl;
            std::cout << "line1.size() = " << row1.size() << std::endl;
            std::cout << "line2.size() = " << row2.size() << std::endl;
            break;
        }
    }
    delete csv_reader1;
    delete csv_reader2;
}
