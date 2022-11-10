#include <fstream>
#include <QDateTime>
#include <QFileDialog>
#include "trainingmanager.h"

TrainingManager::TrainingManager()
{
    svm = cv::ml::SVM::create();
}

void TrainingManager::loadTrainingData(QWidget *parent)
{
    std::ofstream fin;
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    QFileDialog::tr("Open training file"),
                                                    ".",
                                                    QFileDialog::tr("SVM train file(*.csv)"));

    if(fileName.isEmpty())
        return;

    fin.open(fileName.toStdString(), std::ios_base::app);
    if(!fin.is_open())
        return;
    fin.close();

    cv::Ptr<cv::ml::TrainData> tdata = cv::ml::TrainData::loadFromCSV(fileName.toStdString(),
           0, // lines to skip
           0, // 1st elem is the label
          -1); // only 1 response per line
    cv::Mat trainData = tdata->getTrainSamples();
    cv::Mat trainLabels = tdata->getTrainResponses();

    trainLabels.convertTo(trainLabels, CV_32S);

    this->trainLabels = trainLabels;
    this->trainData = trainData;
}

void TrainingManager::loadTestingData(QWidget *parent)
{
    std::ofstream fin;
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    QFileDialog::tr("Open testing file"),
                                                    ".",
                                                    QFileDialog::tr("SVM testing file(*.csv)"));

    if(fileName.isEmpty())
        return;

    fin.open(fileName.toStdString(), std::ios_base::app);
    if(!fin.is_open())
        return;
    fin.close();

    cv::Ptr<cv::ml::TrainData> tdata = cv::ml::TrainData::loadFromCSV(fileName.toStdString(),
           0, // lines to skip
           0, // 1st elem is the label
          -1); // only 1 response per line
    cv::Mat testData = tdata->getTrainSamples();
    cv::Mat testLabels = tdata->getTrainResponses();

    this->testData = testData;
    this->testLabels = testLabels;
}

void TrainingManager::trainSVM()
{
    svm->trainAuto(trainData, 0, trainLabels);
    svm->save(QDateTime::currentDateTime().toString().toStdString() + ".svm");
}

float TrainingManager::testSVM()
{
    cv::Mat testResults;
    svm->predict(testData, testResults);
    float accuracy = float(countNonZero(testResults == testLabels)) / testLabels.rows;
    return accuracy;
}

void TrainingManager::loadSVM(QWidget *parent)
{
    std::ofstream fin;
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    QFileDialog::tr("Open SVM file"),
                                                    ".",
                                                    QFileDialog::tr("SVM file(*.svm)"));

    if(fileName.isEmpty())
        return;

    fin.open(fileName.toStdString(), std::ios_base::app);
    if(!fin.is_open())
        return;
    fin.close();

    svm->load(fileName.toStdString());
}
