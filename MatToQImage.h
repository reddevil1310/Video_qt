

#ifndef MATTOQIMAGE_H
#define MATTOQIMAGE_H

// Qt header files
#include <QtGui>
#include <QDebug>
// OpenCV header files
#include "opencv2/opencv.hpp"

using namespace cv;

QImage MatToQImage(const Mat&);
cv::Mat QImage2Mat(QImage const& src);

#endif // MATTOQIMAGE_H
