
#include "tests_line.h"
#include "assert.h"
#include <iostream>

void TestsLine::runAll(){
    std::cout << "  getValueTest()" << std::endl;      getValueTest();
    std::cout << "  getYDistanceToDot()" << std::endl; getYDistanceToDot();
}

void TestsLine::getValueTest(){
    Line* line1 = new Line(new Point(430, 0), new Point(428, 2));
    assert(line1-> getValue(1) == 429);

    int num = 103620;
    Line* line2 = new Line(new Point(430, num), new Point(428, num + 60*2));
    assert(line2-> getValue(num + 60) == 429);
}

void TestsLine::getYDistanceToDot(){
    Line* line1 = new Line(new Point(5, 0), new Point(3, 2));
    assert(line1->getYDistanceToDot(new Point(3, 1)) == 1);
    assert(line1->getYDistanceToDot(new Point(4, 1)) == 0);
    assert(line1->getYDistanceToDot(new Point(5, 1)) == 1);

    Line* line2 = new Line(new Point(5, 0), new Point(4, 2));
    assert(line2->getYDistanceToDot(new Point(3, 1)) == 1.5);
    assert(line2->getYDistanceToDot(new Point(4, 1)) == 0.5);
    assert(line2->getYDistanceToDot(new Point(5, 1)) == 0.5);
    assert(line2->getYDistanceToDot(new Point(6, 1)) == 1.5);
}
