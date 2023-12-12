#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QGroupBox>
#include <iostream>

void MainWindow::initialize() {
    realtime = new Realtime;
    aspectRatioWidget = new AspectRatioWidget(this);
    aspectRatioWidget->setAspectWidget(realtime, 3.f/4.f);
    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(aspectRatioWidget, 1);
    this->setLayout(hLayout);

    // Create labels in sidebox
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    QLabel *camera_label = new QLabel(); // Camera label
    camera_label->setText("Camera");
    camera_label->setFont(font);
    QLabel *filters_label = new QLabel(); // Filters label
    filters_label->setText("Filters");
    filters_label->setFont(font);
    QLabel *param1_label = new QLabel(); // Parameter 1 label
    param1_label->setText("Parameter 1:");
    QLabel *param2_label = new QLabel(); // Parameter 2 label
    param2_label->setText("Parameter 2:");
    QLabel *near_label = new QLabel(); // Near plane label
    near_label->setText("Near Plane:");
    QLabel *far_label = new QLabel(); // Far plane label
    far_label->setText("Far Plane:");

    // Create checkbox for per-pixel filter
    filter1 = new QCheckBox();
    filter1->setText(QStringLiteral("filter 1"));
    filter1->setChecked(false);

    filter2 = new QCheckBox();
    filter2->setText(QStringLiteral("filter 2"));
    filter2->setChecked(false);

    filter3 = new QCheckBox();
    filter3->setText(QStringLiteral("filter 3"));
    filter3->setChecked(false);

    filter4 = new QCheckBox();
    filter4->setText(QStringLiteral("filter 4"));
    filter4->setChecked(false);

    // Create file uploader for scene file
    uploadFile0 = new QPushButton();
    uploadFile0->setText(QStringLiteral("Upload Scene File 0"));

    uploadFile1 = new QPushButton();
    uploadFile1->setText(QStringLiteral("Upload Scene File 1"));

    uploadFile2 = new QPushButton();
    uploadFile2->setText(QStringLiteral("Upload Scene File 2"));

    uploadFile3 = new QPushButton();
    uploadFile3->setText(QStringLiteral("Upload Scene File 3"));

    uploadFile4 = new QPushButton();
    uploadFile4->setText(QStringLiteral("Upload Scene File 4"));

    uploadFile5 = new QPushButton();
    uploadFile5->setText(QStringLiteral("Upload Scene File 5"));

    uploadFile6 = new QPushButton();
    uploadFile6->setText(QStringLiteral("Upload Scene File 6"));

    uploadFile7 = new QPushButton();
    uploadFile7->setText(QStringLiteral("Upload Scene File 7"));

    saveImage = new QPushButton();
    saveImage->setText(QStringLiteral("Save image"));

    // Creates the boxes containing the camera sliders and number boxes
    QGroupBox *nearLayout = new QGroupBox(); // horizonal near slider alignment
    QHBoxLayout *lnear = new QHBoxLayout();
    QGroupBox *farLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *lfar = new QHBoxLayout();

    // Create slider controls to control near/far planes
    nearSlider = new QSlider(Qt::Orientation::Horizontal); // Near plane slider
    nearSlider->setTickInterval(1);
    nearSlider->setMinimum(1);
    nearSlider->setMaximum(1000);
    nearSlider->setValue(10);

    nearBox = new QDoubleSpinBox();
    nearBox->setMinimum(0.01f);
    nearBox->setMaximum(10.f);
    nearBox->setSingleStep(0.1f);
    nearBox->setValue(0.1f);

    farSlider = new QSlider(Qt::Orientation::Horizontal); // Far plane slider
    farSlider->setTickInterval(1);
    farSlider->setMinimum(1000);
    farSlider->setMaximum(10000);
    farSlider->setValue(10000);

    farBox = new QDoubleSpinBox();
    farBox->setMinimum(10.f);
    farBox->setMaximum(100.f);
    farBox->setSingleStep(0.1f);
    farBox->setValue(100.f);

    // Adds the slider and number box to the parameter layouts
    lnear->addWidget(nearSlider);
    lnear->addWidget(nearBox);
    nearLayout->setLayout(lnear);

    lfar->addWidget(farSlider);
    lfar->addWidget(farBox);
    farLayout->setLayout(lfar);

    vLayout->addWidget(uploadFile0);
    vLayout->addWidget(uploadFile1);
    vLayout->addWidget(uploadFile2);
    vLayout->addWidget(uploadFile3);
    vLayout->addWidget(uploadFile4);
    vLayout->addWidget(uploadFile5);
    vLayout->addWidget(uploadFile6);
    vLayout->addWidget(uploadFile7);
    vLayout->addWidget(saveImage);
    vLayout->addWidget(camera_label);
    vLayout->addWidget(near_label);
    vLayout->addWidget(nearLayout);
    vLayout->addWidget(far_label);
    vLayout->addWidget(farLayout);
    vLayout->addWidget(filters_label);
    vLayout->addWidget(filter1);
    vLayout->addWidget(filter2);
    vLayout->addWidget(filter3);
    vLayout->addWidget(filter4);

    connectUIElements();

    // Set default values for near and far planes
    onValChangeNearBox(0.1f);
    onValChangeFarBox(100.f);
}

void MainWindow::finish() {
    realtime->finish();
    delete(realtime);
}

void MainWindow::connectUIElements() {
    connectFilter1();
    connectFilter2();
    connectFilter3();
    connectFilter4();
    connectUploadFile0();
    connectUploadFile1();
    connectUploadFile2();
    connectUploadFile3();
    connectUploadFile4();
    connectUploadFile5();
    connectUploadFile6();
    connectUploadFile7();
    connectSaveImage();
    connectNear();
    connectFar();
}

void MainWindow::connectFilter1() {
    connect(filter1, &QCheckBox::clicked, this, &MainWindow::onFilter1);
}

void MainWindow::connectFilter2() {
    connect(filter2, &QCheckBox::clicked, this, &MainWindow::onFilter2);
}

void MainWindow::connectFilter3() {
    connect(filter3, &QCheckBox::clicked, this, &MainWindow::onFilter3);
}

void MainWindow::connectFilter4() {
    connect(filter4, &QCheckBox::clicked, this, &MainWindow::onFilter4);
}



void MainWindow::connectUploadFile0() {
    connect(uploadFile0, &QPushButton::clicked, this, &MainWindow::onUploadFile0);
}

void MainWindow::connectUploadFile1() {
    connect(uploadFile1, &QPushButton::clicked, this, &MainWindow::onUploadFile1);
}

void MainWindow::connectUploadFile2() {
    connect(uploadFile2, &QPushButton::clicked, this, &MainWindow::onUploadFile2);
}

void MainWindow::connectUploadFile3() {
    connect(uploadFile3, &QPushButton::clicked, this, &MainWindow::onUploadFile3);
}

void MainWindow::connectUploadFile4() {
    connect(uploadFile4, &QPushButton::clicked, this, &MainWindow::onUploadFile4);
}

void MainWindow::connectUploadFile5() {
    connect(uploadFile5, &QPushButton::clicked, this, &MainWindow::onUploadFile5);
}

void MainWindow::connectUploadFile6() {
    connect(uploadFile6, &QPushButton::clicked, this, &MainWindow::onUploadFile6);
}

void MainWindow::connectUploadFile7() {
    connect(uploadFile7, &QPushButton::clicked, this, &MainWindow::onUploadFile7);
}

void MainWindow::connectSaveImage() {
    connect(saveImage, &QPushButton::clicked, this, &MainWindow::onSaveImage);
}

void MainWindow::connectNear() {
    connect(nearSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeNearSlider);
    connect(nearBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeNearBox);
}

void MainWindow::connectFar() {
    connect(farSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeFarSlider);
    connect(farBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeFarBox);
}

void MainWindow::onFilter1() {
    settings.filter1 = !settings.filter1;
    realtime->settingsChanged();
}

void MainWindow::onFilter2() {
    settings.filter2 = !settings.filter2;
    realtime->settingsChanged();
}

void MainWindow::onFilter3() {
    settings.filter3 = !settings.filter3;
    realtime->settingsChanged();
}

void MainWindow::onFilter4() {
    settings.filter4 = !settings.filter4;
    realtime->settingsChanged();
}

void MainWindow::onUploadFile0() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(0);

}

void MainWindow::onUploadFile1() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(1);

}

void MainWindow::onUploadFile2() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(2);

}

void MainWindow::onUploadFile3() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(3);

}

void MainWindow::onUploadFile4() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(4);

}

void MainWindow::onUploadFile5() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(5);

}

void MainWindow::onUploadFile6() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(6);

}

void MainWindow::onUploadFile7() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("..")
                                                              .append(QDir::separator())
                                                              .append("projects-final")
                                                              .append(QDir::separator())
                                                              .append("obj-files"), tr("Scene Files (*.obj)"));

    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    realtime->sceneChanged(7);

}

void MainWindow::onSaveImage() {
    if (settings.sceneFilePath.empty()) {
        std::cout << "No scene file loaded." << std::endl;
        return;
    }
    std::string sceneName = settings.sceneFilePath.substr(0, settings.sceneFilePath.find_last_of("."));
    sceneName = sceneName.substr(sceneName.find_last_of("/")+1);
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    QDir::currentPath()
                                                        .append(QDir::separator())
                                                        .append("student_outputs")
                                                        .append(QDir::separator())
//                                                        .append("lights-camera")
                                                        .append("action")
                                                        .append(QDir::separator())
                                                        .append("required")
                                                        .append(QDir::separator())
                                                        .append(sceneName), tr("Image Files (*.png)"));
    std::cout << "Saving image to: \"" << filePath.toStdString() << "\"." << std::endl;
    realtime->saveViewportImage(filePath.toStdString());
}

void MainWindow::onValChangeNearSlider(int newValue) {
    //nearSlider->setValue(newValue);
    nearBox->setValue(newValue/100.f);
    settings.nearPlane = nearBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeFarSlider(int newValue) {
    //farSlider->setValue(newValue);
    farBox->setValue(newValue/100.f);
    settings.farPlane = farBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeNearBox(double newValue) {
    nearSlider->setValue(int(newValue*100.f));
    //nearBox->setValue(newValue);
    settings.nearPlane = nearBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeFarBox(double newValue) {
    farSlider->setValue(int(newValue*100.f));
    //farBox->setValue(newValue);
    settings.farPlane = farBox->value();
    realtime->settingsChanged();
}
