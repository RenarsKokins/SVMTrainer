#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include <QString>
#include <QWidget>
#include <opencv2/ml.hpp>
#include <opencv2/core.hpp>

class TrainingManager
{
public:
    TrainingManager();
    float testSVM();
    void trainSVM();
    void setClassCount(int);
    void loadSVM(QWidget *);
    void loadTestingData(QWidget *);
    void loadTrainingData(QWidget *);

private:
    cv::Mat testData;
    cv::Mat trainData;
    cv::Mat testLabels;
    cv::Mat trainLabels;
    cv::Ptr<cv::ml::SVM> svm;
};

#endif // TRAININGMANAGER_H
