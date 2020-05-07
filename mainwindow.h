#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <player.h>
#include <MatToQImage.h>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonLoad_clicked();

    void on_pushButtonPlay_clicked();

    void updatePlayerUI(QImage img);

    void updateDisplay(QImage img);

    QString getFormattedTime(int timeInSeconds);
    void on_horizontalSlider_sliderPressed();
    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_sliderMoved(int position);
    void updateStateButtonPlay(bool state);

private:
    Ui::MainWindow *ui;


    Player* myPlayer;

    QGraphicsScene scene;
    QTranslator translator;
    QGraphicsPixmapItem originalPixmap, processedPixmap;
    cv::Mat originalMat, processedMat;
    QImage originalImage, processedImage;


};
#endif // MAINWINDOW_H
