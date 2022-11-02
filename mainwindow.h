#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "labelmanager.h"
#include "trainingmanager.h"

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
    bool addLabel();                    // Label the image specified value
    void loadImages();                  // Load images to label
    void addBulkLabel();                // Label all images specified value
    void startLabeling();               // Start labeling process
    void createNewLabelsFile();         // Create new file for label data
    void openExistingLabelsFile();      // Append to an existing label data file

    void testSVM();                     // Test SVM
    void loadSVM();                     // Load SVM for testing
    void trainSVM();                    // Start training process
    void loadTestData();                // Load testing data for SVM
    void loadTrainingData();            // Load training data for SVM

private:
    Ui::MainWindow *ui;
    LabelManager *labelManager;
    TrainingManager *trainingManager;

    void updateImagePathLabel();
    void updateImageSizeLabel();
    void updateImagesLabeledLabel();
};
#endif // MAINWINDOW_H
