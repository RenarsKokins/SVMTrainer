#include <QDebug>
#include <fstream>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "labelmanager.h"

LabelManager::LabelManager()
{

}

void LabelManager::createNewLabelsFile(QWidget *parent)
{
    std::ofstream fout;
    QString fileName = QFileDialog::getSaveFileName(parent,
                                                    QFileDialog::tr("Save label file"),
                                                    ".",
                                                    QFileDialog::tr("SVM train file(*.csv)"));
    if(fileName.isEmpty())
        return;

    if(!fileName.contains(".csv"))
        fileName.append(".csv");

    fout.open(fileName.toStdString());
    if(!fout.is_open())
        return;
    fout.close();
    path = fileName;
}

void LabelManager::openExistingLabelsFile(QWidget *parent)
{
    std::ifstream fin;
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    QFileDialog::tr("Open label file"),
                                                    ".",
                                                    QFileDialog::tr("SVM train file(*.csv)"));

    if(fileName.isEmpty())
        return;
    fin.open(fileName.toStdString());
    if(!fin.is_open())
        return;
    fin.close();

    path = fileName;
}

void LabelManager::loadImagePaths(QWidget *parent)
{
    std::ifstream fin;
    QStringList imagePaths = QFileDialog::getOpenFileNames(parent,
                                                        QFileDialog::tr("Open images for labeling"),
                                                        ".",
                                                        QFileDialog::tr("Images (*.tif *.tiff *.bmp *.jpg *.jpeg *.png *.raw)"));
    if(imagePaths.isEmpty())
        return;

    this->imagePaths.clear();
    index = 0;
    for(QString &imagePath : imagePaths)
    {
        fin.open(imagePath.toStdString());
        if(!fin.is_open())
        {
            qDebug() << "Image" << imagePath << "could not be open!";
            continue;
        }
        fin.close();
        this->imagePaths.append(imagePath);
    }
}

QPixmap LabelManager::getImage()
{
    cv::Mat img = cv::imread(imagePaths[index].toStdString(), cv::IMREAD_GRAYSCALE);
    if(img.empty())
    {
        qDebug() << "Image" << imagePaths[index] << "could not be loaded!";
        return QPixmap();
    }

    // Used for error detection (check if all images are the same size)
    if(lastRows == 0 || lastCols == 0)
    {
        lastRows = img.rows;
        lastCols = img.rows;
    }
    if((lastRows != img.rows) || (lastCols != img.cols))
    {
        qDebug() << "IMAGES NOT THE SAME SIZE! Path:" << imagePaths[index];
    }

    return QPixmap::fromImage(QImage(img.data,
                                     img.cols,
                                     img.rows,
                                     static_cast<int>(img.step),
                                     QImage::Format_Grayscale8));
}

void LabelManager::labelImage(int val)
{
    cv::Mat img = cv::imread(imagePaths[index].toStdString(), cv::IMREAD_GRAYSCALE);
    if(img.empty())
    {
        qDebug() << "Image" << imagePaths[index] << "could not be loaded for writing to train file!";
        return;
    }

    std::ofstream fout;
    fout.open(path.toStdString(), std::ios::app);
    if(!fout.is_open())
    {
        qDebug() << "Output(train) file could not be open!";
        return;
    }

    fout << val;     // Add label to training file
    for(int i = 0; i < img.size().area() * img.channels(); i++)
    {
        fout << "," << (unsigned short)img.data[i];
    }
    fout << std::endl;
    fout.close();
}

QString LabelManager::getCurrentPath()
{
    return imagePaths[index];
}

bool LabelManager::isPathSelected()
{
    return !path.isEmpty();
}

bool LabelManager::areImagesLoaded()
{
    return !imagePaths.empty();
}

int LabelManager::getCurrentIndex()
{
    return index;
}

int LabelManager::getImageCount()
{
    return imagePaths.count();
}

bool LabelManager::increaseImageIndex()
{
    if(index >= (getImageCount() - 1))
        return false;
    index++;
    return true;
}
