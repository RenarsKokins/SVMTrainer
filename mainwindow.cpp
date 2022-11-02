#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labelManager = new LabelManager();
    trainingManager = new TrainingManager();

    // Label manager
    connect(ui->addLabelButton, SIGNAL(released()), this, SLOT(addLabel()));
    connect(ui->bulkAddButton, SIGNAL(released()), this, SLOT(addBulkLabel()));
    connect(ui->loadImagesButton, SIGNAL(released()), this, SLOT(loadImages()));
    connect(ui->startLabelingButton, SIGNAL(released()), this, SLOT(startLabeling()));
    connect(ui->newLabelsButton, SIGNAL(released()), this, SLOT(createNewLabelsFile()));
    connect(ui->appendToLabelsButton, SIGNAL(released()), this, SLOT(openExistingLabelsFile()));

    // Training manager
    connect(ui->trainSvmButton, SIGNAL(released()), this, SLOT(trainSVM()));
    connect(ui->loadTrainingDataButton, SIGNAL(released()), this, SLOT(loadTrainingData()));
    connect(ui->loadSvmButton, SIGNAL(released()), this, SLOT(loadSVM()));
    connect(ui->loadTestDataButton, SIGNAL(released()), this, SLOT(loadTestData()));
    connect(ui->testSvmButton, SIGNAL(released()), this, SLOT(testSVM()));
}

void MainWindow::createNewLabelsFile()
{
    labelManager->createNewLabelsFile(this);
}

void MainWindow::openExistingLabelsFile()
{
    labelManager->openExistingLabelsFile(this);
}

void MainWindow::loadImages()
{
    if(!labelManager->isPathSelected())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("You must select/create file for storing labels (above buttons)!"),
                             QMessageBox::Ok);
        return;
    }
    labelManager->loadImagePaths(this);
}

void MainWindow::startLabeling()
{
    if(!labelManager->areImagesLoaded())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("You must select images for labeling!"),
                             QMessageBox::Ok);
        return;
    }
    // Get a bitmap to use for label
    ui->imageLabel->setPixmap(labelManager->getImage().scaled(ui->imageLabel->height(),
                                                              ui->imageLabel->height(),
                                                              Qt::KeepAspectRatioByExpanding,
                                                              Qt::FastTransformation));
    updateImagePathLabel();
    updateImageSizeLabel();
    updateImagesLabeledLabel();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete labelManager;
    delete trainingManager;
}

void MainWindow::updateImageSizeLabel()
{
    QString text;
    text.append(QString::number(labelManager->getImage().width()));
    text.append("x");
    text.append(QString::number(labelManager->getImage().height()));
    ui->imageSizeLabel->setText(text);
}

void MainWindow::updateImagesLabeledLabel()
{
    QString text;
    text.append(QString::number(labelManager->getCurrentIndex() + 1));
    text.append(" out of ");
    text.append(QString::number(labelManager->getImageCount()));
    ui->imagesLabeledLabel->setText(text);
}

void MainWindow::updateImagePathLabel()
{
    ui->imagePathLabel->setText(labelManager->getCurrentPath());
}

bool MainWindow::addLabel()
{
    if(!labelManager->areImagesLoaded())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("You must select images for labeling!"),
                             QMessageBox::Ok);
        return false;
    }
    labelManager->labelImage(ui->labelImageValueSpinbox->value());
    if(!labelManager->increaseImageIndex())
    {
        QMessageBox::information(this,
                                 tr("Info"),
                                 tr("All images labeled! Now you can train SVM!"),
                                 QMessageBox::Ok);
        return false;
    }

    ui->imageLabel->setPixmap(labelManager->getImage().scaled(ui->imageLabel->height(),
                                                              ui->imageLabel->height(),
                                                              Qt::KeepAspectRatioByExpanding,
                                                              Qt::FastTransformation));
    updateImagePathLabel();
    updateImageSizeLabel();
    updateImagesLabeledLabel();
    return true;
}

void MainWindow::addBulkLabel()
{
    for(int i = labelManager->getCurrentIndex(); i < labelManager->getImageCount(); i++)
    {
        // Stop loop if we encounter any issues, or all images have been labeled
        if(!addLabel())
            break;
    }
}

void MainWindow::loadTrainingData()
{
    trainingManager->loadTrainingData(this);
}

void MainWindow::trainSVM()
{
    trainingManager->trainSVM();
    QMessageBox::information(this,
                             tr("Info"),
                             tr("SVM has been trained! Now you can test SVM!"),
                             QMessageBox::Ok);
}

void MainWindow::loadSVM()
{
    trainingManager->loadSVM(this);
}

void MainWindow::loadTestData()
{
    trainingManager->loadTestingData(this);
}

void MainWindow::testSVM()
{
    ui->testResultsLabel->setText(QString::number(trainingManager->testSVM() * 100) + "% correct!");
}
