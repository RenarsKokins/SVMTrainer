#ifndef LABELMANAGER_H
#define LABELMANAGER_H

#include <QWidget>
#include <QVector>
#include <QPixmap>

class LabelManager
{
public:
    LabelManager();
    QPixmap getImage();                         // Returns an image from a path at index
    int getImageCount();
    void labelImage(int);                       // Label the current image at index
    int getCurrentIndex();
    bool isPathSelected();
    bool areImagesLoaded();
    QString getCurrentPath();                   // Returns path to current image at index
    bool increaseImageIndex();                  // Increases index by 1, returns false if all images labeled
    void loadImagePaths(QWidget *);
    void createNewLabelsFile(QWidget *);
    void openExistingLabelsFile(QWidget *);

private:
    QString path;
    int index = 0;
    int lastRows = 0;               // Used for checking if all images are the same size
    int lastCols = 0;               // Used for checking if all images are the same size
    QString lastImagePath = ".";
    QVector<QString> imagePaths;
};

#endif // LABELMANAGER_H
