#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>
#include "core/trainermenu.h"
#include "core/GBAATMres.h"
#include "core/romfuncs.cpp"
#include "core/cheats.cpp"
#include "core/cheatcodes.cpp"
//#include "core/convertbmps.cpp"

char romname[25];
ENABLEDISABLESTRUCT myedstruct;
SLOMOSTRUCT myslomostruct;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("GBAATM-Rebirth - 0.0.1 Alpha");
    settings = new QSettings("Mte90", "GBAATM");
    connect(ui->input_btn, &QPushButton::pressed, this, &MainWindow::openRom);
    connect(ui->output_btn, &QPushButton::pressed, this, &MainWindow::saveRom);
    connect(ui->opencht, &QPushButton::pressed, this, &MainWindow::openCheat);
    connect(ui->deadbeef, &QPushButton::pressed, this, &MainWindow::deadbeef);
    connect(ui->patchgame, &QPushButton::pressed, this, &MainWindow::patchGame);

    if(settings->value("files/input").toString() != "") {
        ui->input_path->setText(settings->value("files/input").toString());
        this->readRom();
    }
    if(settings->value("files/output").toString() != "") {
        ui->output_path->setText(settings->value("files/output").toString());
    }
    if(settings->value("rom/menutitle").toString() != "") {
        ui->menu_text->setText(settings->value("rom/menutitle").toString());
    }
}

void MainWindow::openRom() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open GBA rom"), settings->value("files/input").toString(), tr("Image Files (*.gba *.rom *.bin)"));
    if (!fileName.isEmpty()) {
        ui->input_path->setText(fileName);
        settings->setValue("files/input", fileName);
    }
    this->readRom();
}

void MainWindow::saveRom() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save GBA rom"), settings->value("files/output").toString(), tr("Image Files (*.gba *.rom *.bin)"));
    if (!fileName.isEmpty()) {
        ui->output_path->setText(fileName);
        settings->setValue("files/output", fileName);
    }
}

void MainWindow::openCheat() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open cheat file"), settings->value("files/cheat").toString(), tr("CHT Files (*.cht);;Text Files (*.txt);;Assembly Code File (*.asm)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QString content = QString::fromUtf8(file.readAll());
        ui->cheats->setPlainText(content);
        settings->setValue("files/cheat", fileName);
        this->appendLog(tr("Cheat loaded"));
        parsecheat(fileName.toLocal8Bit().data());
        this->appendLog(tr("Cheat parsed"));
    }
}

void MainWindow::readRom(){
    QString fileName = ui->input_path->text();
    if(QFileInfo::exists(fileName)) {
        getromname(fileName.toLocal8Bit().data(), romname);
        ui->romname->setText(romname);
        this->appendLog(tr("Valid rom found"));
    } else {
        ui->romname->setText(tr("Input file not found"));
    }
}

void MainWindow::appendLog(QString text){
    ui->log->appendPlainText(text);
}

bool MainWindow::isOutputDefined(){
    QString fileName = ui->output_path->text();
    if(!fileName.isEmpty() && !QFileInfo::exists(fileName)) {
        return false;
    }
    return true;
}

bool MainWindow::deadbeef(){
    this->appendLog(tr("DEADBEEF process in progress"));
    if( this->isOutputDefined() ) {
        char* output = ui->output_path->text().toLocal8Bit().data();
        char* input = ui->input_path->text().toLocal8Bit().data();

        QString status = deadbeefrom(input,output);
        this->appendLog(QString(output));
        if (status.compare("-1") == 0) {
            this->appendLog(tr("Something went wrong"));
            return false;
        }

        this->appendLog(status);
        this->appendLog(tr("DEADBEEF function added successfully"));
        this->appendLog(tr("Run the game in an emulator and look at 0x3000000-0x300007FFF for DEADBEEFs"));
        this->appendLog(tr("0x3007F00-0x3007FFF is a good place to start"));
        this->appendLog(tr("Then enter the RAM to use address where at least 2 DEADBEEFs are in a row"));
    }

    return true;
}

void MainWindow::patchGame(){
    QString str;
    this->appendLog(tr("Game patching in progress"));
    if( this->isOutputDefined() ) {
        temptrainermenuint=(unsigned int *)malloc(*trainermenuint+4);
        memcpy(temptrainermenuint,trainermenuint,*trainermenuint+4);
        QString menu_text = ui->menu_text->text().toLower();
        if (menu_text.length() > 0) {
            settings->setValue("rom/menutitle", menu_text);
            char * trainermenuchar=(char *)temptrainermenuint+1;
            char* menutitle;

            QStringList lines = menu_text.split("/", QString::SkipEmptyParts);
            int trainerlines = lines.count();

            for (int thistrainerline=0; thistrainerline<lines.count(); thistrainerline++) {
                menutitle = lines[thistrainerline].toLocal8Bit().data();
                if ( strlen(menutitle) > 26) {
                    menutitle[26] = 0;
                }
                *(trainermenuchar+*temptrainermenuint-92+(thistrainerline*30))=(char)((240-(strlen(menutitle)*9))/2);
                *(trainermenuchar+*temptrainermenuint-91+(thistrainerline*30))=(char)((42-(trainerlines*14))/2)+14*thistrainerline;
                memcpy(trainermenuchar+*temptrainermenuint-90+(thistrainerline*30),menutitle,strlen(menutitle));
            }
            myedstruct.wantenable = 1;
        }

        unsigned int * menuint=(unsigned int *)malloc(0x1000);
        memset(menuint,0,0x1000);
        int cheatintlength=0;
        int cheatselectram=new_hextoint(ui->ram_block->currentText());
        int freemramaddrs[]={0x03007FA0,0x3007FC0,0x03007FD0,0x03007FE0};
        if (!(((cheatselectram>=0x2000000) && (cheatselectram<=0x203fff0)) || ((cheatselectram>=0x3000000) && (cheatselectram<=0x3007ff0)))) {
            cheatselectram=freemramaddrs[0];
            this->appendLog(QString("Invalid RAM address -- Using 0x%X instead").arg(cheatselectram));
//            ui->ram_block->setItemText(cheatselectram);
        }
        unsigned int * cheatint=(unsigned int *)malloc(0x8000);
        memset(cheatint,0,0x8000);
        char * cheatcodes = ui->cheats->toPlainText().toLocal8Bit().data();
        cheatcodes=(char *)malloc(MAXCODELEN*sizeof(char));
        memset(cheatcodes,0,MAXCODELEN*sizeof(char));

        if (strlen(cheatcodes)>0) {
            if (testcht(cheatcodes,"[gameinfo]")==1) {
                importcht(cheatcodes);
            }

            formatcheats(cheatcodes);

            int patchtype=ui->mode->currentIndex();

            if (patchtype==0) { //cb/gssp
                cheatintlength=convertcb(cheatcodes,cheatint,1,cheatselectram+4,menuint);
            }
            if (patchtype==1) { //raw
                cheatintlength=convertraw(cheatcodes,cheatint,1,cheatselectram+4,menuint);
            }
        }
        myedstruct.enablekey=ConvertKeys(ui->trainer_enable_keys->text().toLocal8Bit().data());
        if (myedstruct.enablekey==0x3ff) {
            myedstruct.enablekey=0xfe;
            //sprintf(tempchar,"L+R+A");
        }
        sprintf(myedstruct.enablekeystr,ui->trainer_enable_keys->text().toLocal8Bit().data());

        myedstruct.disablekey=ConvertKeys(ui->trainer_disable_keys->text().toLocal8Bit().data());
        if (myedstruct.disablekey==0x3ff) {
            myedstruct.disablekey=0xfd;
//            sprintf(tempchar,"L+R+B");
        }
        sprintf(myedstruct.disablekeystr,ui->trainer_disable_keys->text().toLocal8Bit().data());
        myslomostruct.wantslomo = ui->execute_every->text().toInt();
        myslomostruct.slowdownkey=ConvertKeys(ui->slowmotion_slow_keys->text().toLocal8Bit().data());
        if (myslomostruct.slowdownkey==0x3ff) {
            myslomostruct.slowdownkey=0xbf;
//            sprintf(tempchar,"L+R+UP");
        }
        sprintf(myslomostruct.slowdownkeystr,ui->slowmotion_slow_keys->text().toLocal8Bit().data());

        myslomostruct.speedupkey=ConvertKeys(ui->slowmotion_up_keys->text().toLocal8Bit().data());
        if (myslomostruct.speedupkey==0x3ff) {
            myslomostruct.speedupkey=0x7f;
//            sprintf(tempchar,"L+R+DOWN");
        }
        sprintf(myslomostruct.speedupkeystr,ui->slowmotion_up_keys->text().toLocal8Bit().data());

        char mypath[500];
        new_getpathfromfilename(mypath,ui->output_path->text().toLocal8Bit().data());
        patchrom(ui->input_path->text().toLocal8Bit().data(),ui->output_path->text().toLocal8Bit().data(),cheatint,cheatintlength,cheatselectram,myslomostruct,myedstruct,ui->execute_every->text().toInt(),mypath,1,menuint,cheatselectram+4, ui->vblank->isChecked());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
