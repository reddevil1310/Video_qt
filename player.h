#ifndef PLAYER_H
#define PLAYER_H

#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>

using namespace std;

class Player : public QThread
{
    Q_OBJECT

private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    cv::VideoCapture *capture = nullptr;
    QImage img;
    cv::Mat RGBframe;
    cv::Mat frame;
    int frameRate;
    int frameWidth;
    int frameHeight;

    vector<cv::Point> polygonPoints;

signals:
    void processedImage(const QImage &image);
    void videoisplaying(bool state=0);

protected:
    void run();
    void nsleep(long long ms);

public:
    Player(QObject *parent = nullptr);
    ~Player();

    bool loadVideo(QString filename);
    bool isOpened();
    QImage getFrame();
    QImage image();
    int getFrameWidth();
    int getFrameHeight();

    void setCurrentFrame(int frameNumber);
    QImage getFirtFrame();

    double getFrameRate();
    double getCurrentFrame();
    double getNumberOfFrames();

    void Play();
    void Stop();
    bool isStopped();

    void setPolygonPoints(vector<cv::Point> pt);
    QStringList mimeTypeFilters;

    QImage getCurrentFrameVideo();
};

#endif // PLAYER_H
