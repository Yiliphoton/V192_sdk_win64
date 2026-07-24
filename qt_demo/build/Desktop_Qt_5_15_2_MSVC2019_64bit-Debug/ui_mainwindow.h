/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *spinPort;
    QPushButton *btnStartOnline;
    QPushButton *btnStopOnline;
    QLabel *label_2;
    QDoubleSpinBox *spinMinDist;
    QLabel *label_3;
    QDoubleSpinBox *spinMaxDist;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOpenPcap;
    QPushButton *btnReadNextFrame;
    QPushButton *btnClosePcap;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBoxExport;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *chkAutoSave;
    QPushButton *btnSaveFrame;
    QPushButton *btnExportPCD;
    QPushButton *btnExportPLY;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *lblFrameId;
    QLabel *label_6;
    QLabel *lblFrameCount;
    QLabel *label_8;
    QLabel *lblPointCount;
    QLabel *label_10;
    QLabel *lblValidPoints;
    QLabel *label_12;
    QLabel *lblFPS;
    QLabel *label_14;
    QLabel *lblDualEcho;
    QLabel *label_16;
    QLabel *lblDistRange;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textPointDetails;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        spinPort = new QSpinBox(groupBox);
        spinPort->setObjectName(QString::fromUtf8("spinPort"));
        spinPort->setMinimum(1024);
        spinPort->setMaximum(65535);
        spinPort->setValue(8899);

        gridLayout->addWidget(spinPort, 0, 1, 1, 1);

        btnStartOnline = new QPushButton(groupBox);
        btnStartOnline->setObjectName(QString::fromUtf8("btnStartOnline"));

        gridLayout->addWidget(btnStartOnline, 0, 2, 1, 1);

        btnStopOnline = new QPushButton(groupBox);
        btnStopOnline->setObjectName(QString::fromUtf8("btnStopOnline"));

        gridLayout->addWidget(btnStopOnline, 0, 3, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spinMinDist = new QDoubleSpinBox(groupBox);
        spinMinDist->setObjectName(QString::fromUtf8("spinMinDist"));
        spinMinDist->setDecimals(1);
        spinMinDist->setMinimum(0.000000000000000);
        spinMinDist->setMaximum(300.000000000000000);
        spinMinDist->setValue(0.100000000000000);

        gridLayout->addWidget(spinMinDist, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 2, 1, 1);

        spinMaxDist = new QDoubleSpinBox(groupBox);
        spinMaxDist->setObjectName(QString::fromUtf8("spinMaxDist"));
        spinMaxDist->setDecimals(1);
        spinMaxDist->setMaximum(500.000000000000000);
        spinMaxDist->setValue(300.000000000000000);

        gridLayout->addWidget(spinMaxDist, 1, 3, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnOpenPcap = new QPushButton(groupBox_2);
        btnOpenPcap->setObjectName(QString::fromUtf8("btnOpenPcap"));

        horizontalLayout->addWidget(btnOpenPcap);

        btnReadNextFrame = new QPushButton(groupBox_2);
        btnReadNextFrame->setObjectName(QString::fromUtf8("btnReadNextFrame"));

        horizontalLayout->addWidget(btnReadNextFrame);

        btnClosePcap = new QPushButton(groupBox_2);
        btnClosePcap->setObjectName(QString::fromUtf8("btnClosePcap"));

        horizontalLayout->addWidget(btnClosePcap);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox_2);

        groupBoxExport = new QGroupBox(centralwidget);
        groupBoxExport->setObjectName(QString::fromUtf8("groupBoxExport"));
        horizontalLayout_2 = new QHBoxLayout(groupBoxExport);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        chkAutoSave = new QCheckBox(groupBoxExport);
        chkAutoSave->setObjectName(QString::fromUtf8("chkAutoSave"));

        horizontalLayout_2->addWidget(chkAutoSave);

        btnSaveFrame = new QPushButton(groupBoxExport);
        btnSaveFrame->setObjectName(QString::fromUtf8("btnSaveFrame"));

        horizontalLayout_2->addWidget(btnSaveFrame);

        btnExportPCD = new QPushButton(groupBoxExport);
        btnExportPCD->setObjectName(QString::fromUtf8("btnExportPCD"));

        horizontalLayout_2->addWidget(btnExportPCD);

        btnExportPLY = new QPushButton(groupBoxExport);
        btnExportPLY->setObjectName(QString::fromUtf8("btnExportPLY"));

        horizontalLayout_2->addWidget(btnExportPLY);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(groupBoxExport);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        lblFrameId = new QLabel(groupBox_3);
        lblFrameId->setObjectName(QString::fromUtf8("lblFrameId"));

        gridLayout_2->addWidget(lblFrameId, 0, 1, 1, 1);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 2, 1, 1);

        lblFrameCount = new QLabel(groupBox_3);
        lblFrameCount->setObjectName(QString::fromUtf8("lblFrameCount"));

        gridLayout_2->addWidget(lblFrameCount, 0, 3, 1, 1);

        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        lblPointCount = new QLabel(groupBox_3);
        lblPointCount->setObjectName(QString::fromUtf8("lblPointCount"));

        gridLayout_2->addWidget(lblPointCount, 1, 1, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 1, 2, 1, 1);

        lblValidPoints = new QLabel(groupBox_3);
        lblValidPoints->setObjectName(QString::fromUtf8("lblValidPoints"));

        gridLayout_2->addWidget(lblValidPoints, 1, 3, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 2, 0, 1, 1);

        lblFPS = new QLabel(groupBox_3);
        lblFPS->setObjectName(QString::fromUtf8("lblFPS"));

        gridLayout_2->addWidget(lblFPS, 2, 1, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 2, 2, 1, 1);

        lblDualEcho = new QLabel(groupBox_3);
        lblDualEcho->setObjectName(QString::fromUtf8("lblDualEcho"));

        gridLayout_2->addWidget(lblDualEcho, 2, 3, 1, 1);

        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 3, 0, 1, 1);

        lblDistRange = new QLabel(groupBox_3);
        lblDistRange->setObjectName(QString::fromUtf8("lblDistRange"));

        gridLayout_2->addWidget(lblDistRange, 3, 1, 1, 3);


        verticalLayout->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_2 = new QVBoxLayout(groupBox_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        textPointDetails = new QTextEdit(groupBox_4);
        textPointDetails->setObjectName(QString::fromUtf8("textPointDetails"));
        textPointDetails->setReadOnly(true);
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        textPointDetails->setFont(font);

        verticalLayout_2->addWidget(textPointDetails);


        verticalLayout->addWidget(groupBox_4);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "V192 \347\202\271\344\272\221\346\237\245\347\234\213\345\231\250", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "UDP\347\253\257\345\217\243:", nullptr));
        btnStartOnline->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\345\234\250\347\272\277\346\216\245\346\224\266", nullptr));
        btnStopOnline->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\346\216\245\346\224\266", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\260\217\350\267\235\347\246\273(m):", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\244\247\350\267\235\347\246\273(m):", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "PCAP\346\226\207\344\273\266", nullptr));
        btnOpenPcap->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200PCAP\346\226\207\344\273\266", nullptr));
        btnReadNextFrame->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\217\226\344\270\213\344\270\200\345\270\247", nullptr));
        btnClosePcap->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255PCAP", nullptr));
        groupBoxExport->setTitle(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\345\257\274\345\207\272", nullptr));
        chkAutoSave->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\344\277\235\345\255\230\345\210\260\346\227\266\351\227\264\346\210\263\346\226\207\344\273\266\345\244\271", nullptr));
        btnSaveFrame->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\345\275\223\345\211\215\345\270\247(TXT)", nullptr));
        btnExportPCD->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272PCD", nullptr));
        btnExportPLY->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272PLY", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\347\273\237\350\256\241\344\277\241\346\201\257", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\270\247ID:", nullptr));
        lblFrameId->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\346\200\273\345\270\247\346\225\260:", nullptr));
        lblFrameCount->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\347\202\271\346\225\260:", nullptr));
        lblPointCount->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\346\234\211\346\225\210\347\202\271\346\225\260:", nullptr));
        lblValidPoints->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\345\270\247\347\216\207(FPS):", nullptr));
        lblFPS->setText(QCoreApplication::translate("MainWindow", "0.00", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\345\217\214\345\233\236\346\263\242:", nullptr));
        lblDualEcho->setText(QCoreApplication::translate("MainWindow", "\345\220\246", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "\350\267\235\347\246\273\350\214\203\345\233\264:", nullptr));
        lblDistRange->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\347\202\271\350\257\246\346\203\205\357\274\210\345\211\21510\344\270\252\346\234\211\346\225\210\347\202\271\357\274\211", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
