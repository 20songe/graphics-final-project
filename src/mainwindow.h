#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "realtime.h"
#include "utils/aspectratiowidget/aspectratiowidget.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    void initialize();
    void finish();

private:
    void connectUIElements();
    void connectNear();
    void connectFar();
    void connectFilter1();
    void connectFilter2();
    void connectFilter3();
    void connectFilter4();
    void connectUploadFile0();
    void connectUploadFile1();
    void connectUploadFile2();
    void connectUploadFile3();
    void connectUploadFile4();
    void connectUploadFile5();
    void connectUploadFile6();
    void connectUploadFile7();
    void connectSaveImage();

    Realtime *realtime;
    AspectRatioWidget *aspectRatioWidget;
    QCheckBox *filter1;
    QCheckBox *filter2;
    QCheckBox *filter3;
    QCheckBox *filter4;
    QPushButton *uploadFile0;
    QPushButton *uploadFile1;
    QPushButton *uploadFile2;
    QPushButton *uploadFile3;
    QPushButton *uploadFile4;
    QPushButton *uploadFile5;
    QPushButton *uploadFile6;
    QPushButton *uploadFile7;
    QPushButton *saveImage;
    QSlider *nearSlider;
    QSlider *farSlider;
    QDoubleSpinBox *nearBox;
    QDoubleSpinBox *farBox;

private slots:
    void onFilter1();
    void onFilter2();
    void onFilter3();
    void onFilter4();
    void onUploadFile0();
    void onUploadFile1();
    void onUploadFile2();
    void onUploadFile3();
    void onUploadFile4();
    void onUploadFile5();
    void onUploadFile6();
    void onUploadFile7();
    void onSaveImage();
    void onValChangeNearSlider(int newValue);
    void onValChangeFarSlider(int newValue);
    void onValChangeNearBox(double newValue);
    void onValChangeFarBox(double newValue);

};
