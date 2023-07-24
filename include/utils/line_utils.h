
#ifndef CPP_PROJECT_LINE_UTILS_H
#define CPP_PROJECT_LINE_UTILS_H

#include "constants.h"
#include "Line.h"
#include <vector>

class LineUtils {

private:


public:
    static std::vector<std::string> projectPointsOntoLine(Line* line, std::vector<int> x_coords);
    static std::vector<std::string> decodePoints(float point1_y, float point2_y, std::vector<int> x_coords);
    static std::vector<std::string> decodePointsString(std::string point1_y, std::string point2_y, std::vector<int> x_coords);
    static void printPoint(Point* point);
    static void printLine(Line* line);
};

#endif //CPP_PROJECT_LINE_UTILS_H
