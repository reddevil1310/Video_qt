#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myPlayer = new Player();
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),this, SLOT(updatePlayerUI(QImage)));
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),this, SLOT(updateDisplay(QImage)));
    QObject::connect(myPlayer, SIGNAL(videoisplaying(bool)),this, SLOT(updateStateButtonPlay(bool)));

    ui->graphicsView->setScene(&scene);
    scene.addItem(&originalPixmap);
    scene.addItem(&processedPixmap);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    ui->pushButtonPlay->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete myPlayer;

}

void MainWindow::updateDisplay(QImage img)
{
//    originalImage = MatToQImage(originalMat);//QImage(originalMat.data, originalMat.cols, originalMat.rows, originalMat.step, QImage::Format_RGB888);
//    originalPixmap.setPixmap(QPixmap::fromImage(originalImage));//.rgbSwapped()

//    processedImage = MatToQImage(processedMat);//QImage(processedMat.data, processedMat.cols, processedMat.rows, processedMat.step, QImage::Format_RGB888);
//    processedPixmap.setPixmap(QPixmap::fromImage(processedImage));//.rgbSwapped()

    if (!img.isNull())
    {
     ui->graphicsView->setAlignment(Qt::AlignCenter);
     originalPixmap.setPixmap(QPixmap::fromImage(img).scaled(ui->graphicsView->size(),Qt::KeepAspectRatio, Qt::FastTransformation));

     ui->horizontalSlider->setValue(myPlayer->getCurrentFrame());
     ui->label_2->setText( getFormattedTime( (int)myPlayer->getCurrentFrame()/(int)myPlayer->getFrameRate()) );
    }

}


void MainWindow::on_pushButtonLoad_clicked()
{
    myPlayer->Stop();
    QString filename = QFileDialog::getOpenFileName(this,
                                              tr("Open Video"), ".",
                                              tr("Video Files (*.avi *.mpg *.mp4)"));
        QFileInfo name = filename;
        if (!filename.isEmpty())
        {
            if (!myPlayer->loadVideo(filename))
            {
                QMessageBox msgBox;
                msgBox.setText("The selected video could not be opened!");
                msgBox.exec();
            }
            else
            {
                       this->setWindowTitle(name.fileName());
                       ui->pushButtonPlay->setEnabled(true);
                       ui->horizontalSlider->setEnabled(true);
                       ui->horizontalSlider->setMaximum(myPlayer->getNumberOfFrames());
                       ui->label_3->setText( getFormattedTime( (int)myPlayer->getNumberOfFrames()/(int)myPlayer->getFrameRate()) );
            }
        }
}

void MainWindow::on_pushButtonPlay_clicked()
{
    if (myPlayer->isStopped())
    {
        myPlayer->Play();
        //ui->pushButtonPlay->setText(tr("Stop"));
    }else
    {
        myPlayer->Stop();
        //ui->pushButtonPlay->setText(tr("Play"));
    }
}
void MainWindow::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio, Qt::FastTransformation));

        ui->horizontalSlider->setValue(myPlayer->getCurrentFrame());
        ui->label_2->setText( getFormattedTime( (int)myPlayer->getCurrentFrame()/(int)myPlayer->getFrameRate()) );
    }
}

QString MainWindow::getFormattedTime(int timeInSeconds){
    int seconds = (int) (timeInSeconds) % 60 ;
    int minutes = (int) ((timeInSeconds / 60) % 60);
    int hours   = (int) ((timeInSeconds / (60*60)) % 24);
    QTime t(hours, minutes, seconds);
    if (hours == 0 )
        return t.toString("mm:ss");
    else
        return t.toString("h:mm:ss");
}
void MainWindow::on_horizontalSlider_sliderPressed()
{
    myPlayer->Stop();
}
void MainWindow::on_horizontalSlider_sliderReleased()
{
    myPlayer->Stop();
    QObject::disconnect(myPlayer, SIGNAL(processedImage(QImage)),this, SLOT(updatePlayerUI(QImage)));
    QObject::disconnect(myPlayer, SIGNAL(processedImage(QImage)),this, SLOT(updateDisplay(QImage)));
    updatePlayerUI(myPlayer->getFrame());
    updateDisplay(myPlayer->getFrame());
    myPlayer->setCurrentFrame(myPlayer->getCurrentFrame()-1);
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),this, SLOT(updatePlayerUI(QImage)));
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),this, SLOT(updateDisplay(QImage)));
}
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    myPlayer->setCurrentFrame(position);
    ui->label_2->setText( getFormattedTime( position/(int)myPlayer->getFrameRate()) );
}

void MainWindow::updateStateButtonPlay(bool state)
{
    if(state==1)
    {
        ui->pushButtonPlay->setText(tr("Stop"));
    }
    else
    {
        ui->pushButtonPlay->setText(tr("Play"));
    }
}

