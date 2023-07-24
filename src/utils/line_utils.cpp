
#include "line_utils.h"
#include "conversor.h"
#include <assert.h>
#include <iostream>

std::vector<std::string> LineUtils::projectPointsOntoLine(Line* line, std::vector<int> x_coords){
    int window_size = x_coords.size();
    std::vector<std::string> res(window_size);
    for (int i=0; i < window_size; i++){
        int x_coord = x_coords.at(i);
        double value = line->getValue(x_coord);
        res[i] = Conversor::doubleToIntToString(value);
    }
    return res;
}

std::vector<std::string> LineUtils::decodePoints(float point1_y, float point2_y, std::vector<int> x_coords){
    int first_x_coord = x_coords.front();
    int last_x_coord = x_coords.back();
#if CHECKS
    assert(first_x_coord == 0);
    assert(last_x_coord > first_x_coord);
#endif
    Point p1 = Point(point1_y, first_x_coord);
    Point p2 = Point(point2_y, last_x_coord);
    Line* line = new Line(&p1, &p2);
    return projectPointsOntoLine(line, x_coords);
}

std::vector<std::string> LineUtils::decodePointsString(std::string point1_y, std::string point2_y, std::vector<int> x_coords){
    int point1_y_int = Conversor::stringToInt(point1_y);
    int point2_y_int = Conversor::stringToInt(point2_y);
    return decodePoints(point1_y_int, point2_y_int, x_coords);
}

void LineUtils::printPoint(Point* point){
    std::cout << "(x, y) = (" << point->x << ", " << point->y << ")" << std::endl;
}

void LineUtils::printLine(Line* line){
    std::cout << "(slope, intercept) = (" << line->getSlope() << ", " << line->getIntercept() << ")" << std::endl;
}
