#include "player.h"
#include <chrono>
#include <thread>
#include <QThread>


Player::Player(QObject *parent)
    : QThread(parent)
{
    stop = true;

    // Sets the filters used in the file dialog
    mimeTypeFilters << "video/mp4" << "video/avi";
}

Player::~Player()
{
    mutex.lock();
    stop = true;
    condition.wakeOne();
    mutex.unlock();
    wait();
}

bool Player::loadVideo(QString filename)
{
    capture = new cv::VideoCapture(filename.toStdString());
    emit processedImage(getFirtFrame());
    if (capture->isOpened()) {
        frameRate   = int(capture->get(cv::CAP_PROP_FPS));
        frameWidth  = int(capture->get(cv::CAP_PROP_FRAME_WIDTH));
        frameHeight = int(capture->get(cv::CAP_PROP_FRAME_HEIGHT));
        return true;
    }
    else
    {
        return false;
    }

}

bool Player::isOpened()
{
    if (capture == nullptr) {
        return false;
    }

    return capture->isOpened();
}

QImage Player::getFrame()
{
    if (capture->read(frame)) {
        cv::Mat overlay;
        frame.copyTo(overlay);

        // Fill polygon
        if (polygonPoints.size() > 0) {
            // Draw all line segments
            const cv::Point *pts = (const cv::Point *) cv::Mat(polygonPoints).data;
            const int npts = cv::Mat(polygonPoints).rows;

            cv::fillPoly(overlay,
                         &pts,
                         &npts,
                         1,
                         cv::Scalar(0, 0, 128));

            cv::addWeighted(overlay,
                            0.4,
                            frame,
                            1 - 0.4,
                            0,
                            frame);
        }

        // Convert frame to QImage
        if (frame.channels() == 3) {
            cv::cvtColor(frame, RGBframe, cv::COLOR_BGR2RGB);

            img = QImage(static_cast<const unsigned char*>(RGBframe.data),
                         RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        } else {
            img = QImage(static_cast<const unsigned char*>(frame.data),
                         frame.cols,frame.rows,QImage::Format_Indexed8);
        }
    }
    else
    {
        emit videoisplaying(0);
        img = QImage(); // Null image
    }

    return img;
}

QImage Player::image()
{
    return img;
}

int Player::getFrameWidth()
{
    return frameWidth;
}

int Player::getFrameHeight()
{
    return frameHeight;
}

void Player::setCurrentFrame(int frameNumber)
{
    capture->set(cv::CAP_PROP_POS_FRAMES, frameNumber);
}

QImage Player::getFirtFrame()
{
        cv::Mat fr;
        QImage img1;
        if(capture->read(fr))
        {

            // Convert frame to QImage
            if (fr.channels() == 3)
            {
                cv::cvtColor(fr, RGBframe, cv::COLOR_BGR2RGB);

                img1 = QImage(static_cast<const unsigned char*>(RGBframe.data),
                             RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
            }
            else
            {
                img1 = QImage(static_cast<const unsigned char*>(fr.data),
                             fr.cols,fr.rows,QImage::Format_Indexed8);
            }
        }
        else
        {
            img1 = QImage(); // Null image
        }
        capture->set(cv::CAP_PROP_POS_FRAMES, 0);
        return img1;
}

double Player::getFrameRate()
{
    return frameRate;
}

double Player::getCurrentFrame()
{
    return capture->get(cv::CAP_PROP_POS_FRAMES);
}

double Player::getNumberOfFrames()
{
    return capture->get(cv::CAP_PROP_FRAME_COUNT);
}

void Player::Play()
{
    // If last frame, set to the first frame
    if (capture->get(cv::CAP_PROP_POS_FRAMES) >= capture->get(cv::CAP_PROP_FRAME_COUNT)) {
        capture->set(cv::CAP_PROP_POS_FRAMES, 0);
    }

    stop = false;
    start(TimeCriticalPriority);
    emit videoisplaying(1);
}

void Player::run()
{
    LONGLONG delay = (1000 / frameRate);

    while(!stop) {
        img = this->getFrame();

        if (img.isNull()) {
            stop = true;
        } else {
            emit processedImage(img);
            this->nsleep(delay);
        }
    }
}

void Player::Stop()
{
    stop = true;
    emit videoisplaying(0);
}

void Player::nsleep(long long ns)
{
    QThread::msleep(ns);
}


bool Player::isStopped()
{
    return this->stop;
}

void Player::setPolygonPoints(vector<cv::Point> pt)
{
    this->Stop();
    while(this->isRunning());

    polygonPoints = pt;
}
