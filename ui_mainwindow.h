/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QWidget *formLayoutWidget;
    QGridLayout *gridLayout_2;
    QLineEdit *output_path;
    QPushButton *output_btn;
    QLabel *label;
    QPushButton *input_btn;
    QLineEdit *input_path;
    QLabel *label_3;
    QLabel *romname;
    QLabel *checksum;
    QLabel *warning;
    QWidget *tab_2;
    QPlainTextEdit *cheats;
    QPushButton *opencht;
    QComboBox *mode;
    QWidget *tab_3;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_5;
    QPushButton *loadselection;
    QPushButton *loadfont;
    QPushButton *loadbg;
    QLineEdit *menu_text;
    QLabel *label_2;
    QLabel *label_8;
    QLabel *label_10;
    QCheckBox *vblank;
    QSpinBox *execute_every;
    QComboBox *ram_block;
    QGroupBox *groupBox;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_4;
    QLabel *label_7;
    QLineEdit *trainer_enable_keys;
    QLabel *label_6;
    QLineEdit *trainer_disable_keys;
    QWidget *tab_5;
    QTextBrowser *textBrowser;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *patchgame;
    QPushButton *deadbeef;
    QPlainTextEdit *log;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(661, 803);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(661, 803));
        MainWindow->setMaximumSize(QSize(661, 803));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(9, 9, 641, 791));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(verticalLayoutWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMaximumSize(QSize(16777215, 360));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        formLayoutWidget = new QWidget(tab_4);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 611, 76));
        gridLayout_2 = new QGridLayout(formLayoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        output_path = new QLineEdit(formLayoutWidget);
        output_path->setObjectName(QString::fromUtf8("output_path"));

        gridLayout_2->addWidget(output_path, 1, 1, 1, 1);

        output_btn = new QPushButton(formLayoutWidget);
        output_btn->setObjectName(QString::fromUtf8("output_btn"));

        gridLayout_2->addWidget(output_btn, 1, 2, 1, 1);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        input_btn = new QPushButton(formLayoutWidget);
        input_btn->setObjectName(QString::fromUtf8("input_btn"));

        gridLayout_2->addWidget(input_btn, 0, 2, 1, 1);

        input_path = new QLineEdit(formLayoutWidget);
        input_path->setObjectName(QString::fromUtf8("input_path"));

        gridLayout_2->addWidget(input_path, 0, 1, 1, 1);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        romname = new QLabel(tab_4);
        romname->setObjectName(QString::fromUtf8("romname"));
        romname->setGeometry(QRect(10, 100, 611, 31));
        checksum = new QLabel(tab_4);
        checksum->setObjectName(QString::fromUtf8("checksum"));
        checksum->setGeometry(QRect(10, 130, 611, 31));
        warning = new QLabel(tab_4);
        warning->setObjectName(QString::fromUtf8("warning"));
        warning->setGeometry(QRect(10, 220, 611, 91));
        warning->setMouseTracking(true);
        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        cheats = new QPlainTextEdit(tab_2);
        cheats->setObjectName(QString::fromUtf8("cheats"));
        cheats->setGeometry(QRect(10, 10, 611, 261));
        opencht = new QPushButton(tab_2);
        opencht->setObjectName(QString::fromUtf8("opencht"));
        opencht->setGeometry(QRect(10, 280, 101, 34));
        mode = new QComboBox(tab_2);
        mode->addItem(QString());
        mode->addItem(QString());
        mode->addItem(QString());
        mode->setObjectName(QString::fromUtf8("mode"));
        mode->setGeometry(QRect(120, 280, 181, 32));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayoutWidget_4 = new QWidget(tab_3);
        gridLayoutWidget_4->setObjectName(QString::fromUtf8("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(10, 50, 611, 41));
        gridLayout_5 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        loadselection = new QPushButton(gridLayoutWidget_4);
        loadselection->setObjectName(QString::fromUtf8("loadselection"));

        gridLayout_5->addWidget(loadselection, 0, 1, 1, 1);

        loadfont = new QPushButton(gridLayoutWidget_4);
        loadfont->setObjectName(QString::fromUtf8("loadfont"));

        gridLayout_5->addWidget(loadfont, 0, 2, 1, 1);

        loadbg = new QPushButton(gridLayoutWidget_4);
        loadbg->setObjectName(QString::fromUtf8("loadbg"));

        gridLayout_5->addWidget(loadbg, 0, 0, 1, 1);

        menu_text = new QLineEdit(tab_3);
        menu_text->setObjectName(QString::fromUtf8("menu_text"));
        menu_text->setGeometry(QRect(280, 10, 339, 32));
        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 191, 31));
        label_8 = new QLabel(tab_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 110, 101, 31));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(360, 110, 81, 31));
        vblank = new QCheckBox(tab_3);
        vblank->setObjectName(QString::fromUtf8("vblank"));
        vblank->setGeometry(QRect(190, 110, 121, 31));
        vblank->setChecked(true);
        execute_every = new QSpinBox(tab_3);
        execute_every->setObjectName(QString::fromUtf8("execute_every"));
        execute_every->setGeometry(QRect(100, 110, 52, 32));
        execute_every->setMinimum(1);
        execute_every->setMaximum(256);
        ram_block = new QComboBox(tab_3);
        ram_block->addItem(QString());
        ram_block->addItem(QString());
        ram_block->addItem(QString());
        ram_block->addItem(QString());
        ram_block->setObjectName(QString::fromUtf8("ram_block"));
        ram_block->setGeometry(QRect(450, 110, 171, 32));
        groupBox = new QGroupBox(tab_3);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 220, 631, 101));
        gridLayoutWidget_3 = new QWidget(groupBox);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(9, 30, 611, 61));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(gridLayoutWidget_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 0, 0, 1, 1);

        trainer_enable_keys = new QLineEdit(gridLayoutWidget_3);
        trainer_enable_keys->setObjectName(QString::fromUtf8("trainer_enable_keys"));

        gridLayout_4->addWidget(trainer_enable_keys, 1, 0, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 0, 1, 1, 1);

        trainer_disable_keys = new QLineEdit(gridLayoutWidget_3);
        trainer_disable_keys->setObjectName(QString::fromUtf8("trainer_disable_keys"));

        gridLayout_4->addWidget(trainer_disable_keys, 1, 1, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        textBrowser = new QTextBrowser(tab_5);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 10, 611, 301));
        tabWidget->addTab(tab_5, QString());

        verticalLayout->addWidget(tabWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setHorizontalSpacing(6);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        patchgame = new QPushButton(verticalLayoutWidget);
        patchgame->setObjectName(QString::fromUtf8("patchgame"));

        gridLayout->addWidget(patchgame, 0, 2, 1, 1);

        deadbeef = new QPushButton(verticalLayoutWidget);
        deadbeef->setObjectName(QString::fromUtf8("deadbeef"));

        gridLayout->addWidget(deadbeef, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        log = new QPlainTextEdit(verticalLayoutWidget);
        log->setObjectName(QString::fromUtf8("log"));
        log->setMaximumSize(QSize(16777215, 350));
        log->setReadOnly(true);

        verticalLayout->addWidget(log);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GBAATM-Rebirth", nullptr));
        output_btn->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Output", nullptr));
        input_btn->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Input", nullptr));
        romname->setText(QString());
        checksum->setText(QString());
        warning->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>On GBATemp's threads (check <span style=\" text-decoration: underline;\">Info</span> tab) for this app and the original one <span style=\" font-weight:600;\">there are tons of reports</span><br/>about some cheat codes that don't work or crashes the app.</p><p>The <span style=\" text-decoration: underline;\">only</span> solution is to try others or that includes the decryption key.</p><p><span style=\" font-weight:600; text-decoration: underline;\">Or that someone fix the code</span>.</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Files", nullptr));
        opencht->setText(QCoreApplication::translate("MainWindow", "Open CHT file", nullptr));
        mode->setItemText(0, QCoreApplication::translate("MainWindow", "Codebreaker/GS V3", nullptr));
        mode->setItemText(1, QCoreApplication::translate("MainWindow", "RAW (VBA)", nullptr));
        mode->setItemText(2, QCoreApplication::translate("MainWindow", "GS/AR v1/2 (Experimental)", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "CheatCodes", nullptr));
        loadselection->setText(QCoreApplication::translate("MainWindow", "Load selection bar - 240x14", nullptr));
        loadfont->setText(QCoreApplication::translate("MainWindow", "Load font texture - 9x12", nullptr));
        loadbg->setText(QCoreApplication::translate("MainWindow", "Load BG - 240x160", nullptr));
        menu_text->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "Menu Text (use / to split lines):", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Execute every:", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "RAM to use:", nullptr));
        vblank->setText(QCoreApplication::translate("MainWindow", "VBLANK Check", nullptr));
        ram_block->setItemText(0, QCoreApplication::translate("MainWindow", "0x3007FA0", nullptr));
        ram_block->setItemText(1, QCoreApplication::translate("MainWindow", "0x3007FC0", nullptr));
        ram_block->setItemText(2, QCoreApplication::translate("MainWindow", "0x3007FD0", nullptr));
        ram_block->setItemText(3, QCoreApplication::translate("MainWindow", "0x3007FE0", nullptr));

        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Trainer", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Enable Keys:", nullptr));
        trainer_enable_keys->setText(QCoreApplication::translate("MainWindow", "L+R+A", nullptr));
        trainer_enable_keys->setPlaceholderText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "Disable Keys:", nullptr));
        trainer_disable_keys->setText(QCoreApplication::translate("MainWindow", "L+R+B", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Parameters", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Oxygen'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Software based on GBAATM 1.9.9.7.0.0.1, made by Cracker.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">On 2020 a new version after the release of the source code was possible. This new version is built with QT, include a refactoring of th"
                        "e original code that was ported to new operative systems with a new UI and &quot;maybe&quot; new features.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Code released, open for contributions: </span><a href=\"https://github.com/Mte90/GBAATM-Rebirth\"><span style=\" font-size:10pt; text-decoration: underline; color:#2980b9;\">https://github.com/Mte90/GBAATM-Rebirth</span></a></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt; text-decoration: underline; color:#2980b9;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\""
                        ">Original app Thread: <a href=\"https://github.com/Mte90/GBAATM-Rebirth\"><span style=\" font-size:10pt; text-decoration: underline; color:#2980b9;\">https://gbatemp.net/threads/gba-auto-trainer-maker-gbaatm.99334</span></a></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">New app thread: <a href=\"https://github.com/Mte90/GBAATM-Rebirth\"><span style=\" font-size:10pt; text-decoration: underline; color:#2980b9;\">https://gbatemp.net/threads/gbaatm-rebirth-gba-auto-trainer-maker-a-new-hope.564321</span></a></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Info", nullptr));
        patchgame->setText(QCoreApplication::translate("MainWindow", "Patch Game!", nullptr));
        deadbeef->setText(QCoreApplication::translate("MainWindow", "DEADBEEF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
