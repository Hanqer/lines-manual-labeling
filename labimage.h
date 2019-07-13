#ifndef LABIMAGE_H
#define LABIMAGE_H
#include <opencv.hpp>
struct mLine{
    int x1, y1, x2, y2;
    mLine() {}
    mLine(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

struct LabImage{
    cv::Mat image_show;
    int num_semantic_lines;
    std::string name;
    std::string abs_path;

    bool needDraw;
    cv::Point pre_point;
    std::vector<mLine> lines;

    LabImage() :  num_semantic_lines(0), needDraw(false){}
};

#endif // LABIMAGE_H
