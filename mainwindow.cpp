#include "mainwindow.h"
#include "core/cheatcodes-converters.cpp"
#include "core/cheatcodes.cpp"
#include "core/convertbmps.cpp"
#include "core/romfuncs.cpp"
#include "functions.h"
#include "ui_mainwindow.h"
#include "variables.h"

char romname[25];

MainWindow::MainWindow(QWidget* parent)
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
  connect(ui->loadbg, &QPushButton::pressed, this, &MainWindow::loadBg);
  connect(ui->loadfont, &QPushButton::pressed, this, &MainWindow::loadFont);
  connect(ui->loadselection, &QPushButton::pressed, this, &MainWindow::loadSelectionBar);

  if (settings->value("files/input").toString() != "") {
    ui->input_path->setText(settings->value("files/input").toString());
    this->readRom();
  }
  if (settings->value("files/output").toString() != "") {
    ui->output_path->setText(settings->value("files/output").toString());
  }
  if (settings->value("rom/menutitle").toString() != "") {
    ui->menu_text->setText(settings->value("rom/menutitle").toString());
  }
}

void
MainWindow::openRom()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open GBA rom"), settings->value("files/input").toString(),
                                                  tr("Image Files (*.gba *.rom *.bin)"));
  if (!fileName.isEmpty()) {
    ui->input_path->setText(fileName);
    settings->setValue("files/input", fileName);
  }
  this->readRom();
}

void
MainWindow::loadBg()
{
  QString fileName =
    QFileDialog::getOpenFileName(this, tr("Open background image"), settings->value("files/input").toString(), tr("Image Files (*.bmp)"));
  if (!fileName.isEmpty()) {
    int goodbmp;
    goodbmp = bmp2short(fileName.toLocal8Bit().data(), menubgshort, 1);
    if (goodbmp == 1)
      wantbg = 1;
  }
}

void
MainWindow::loadFont()
{
  QString fileName =
    QFileDialog::getOpenFileName(this, tr("Open font image"), settings->value("files/input").toString(), tr("Image Files (*.bmp)"));
  if (!fileName.isEmpty()) {
    int goodbmp;
    goodbmp = bmp2short(fileName.toLocal8Bit().data(), menufontshort, 3);
    if (goodbmp == 1)
      wantfont = 1;
  }
}

void
MainWindow::loadSelectionBar()
{
  QString fileName =
    QFileDialog::getOpenFileName(this, tr("Open background image"), settings->value("files/input").toString(), tr("Image Files (*.bmp)"));
  if (!fileName.isEmpty()) {
    int goodbmp;
    goodbmp = bmp2short(fileName.toLocal8Bit().data(), menuselectshort, 2);
    if (goodbmp == 1)
      wantselect = 1;
  }
}

void
MainWindow::saveRom()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save GBA rom"), settings->value("files/output").toString(),
                                                  tr("Image Files (*.gba *.rom *.bin)"));
  if (!fileName.isEmpty()) {
    ui->output_path->setText(fileName);
    settings->setValue("files/output", fileName);
  }
}

void
MainWindow::openCheat()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open cheat file"), settings->value("files/cheat").toString(),
                                                  tr("CHT Files (*.cht);;Text Files (*.txt);;Assembly Code File (*.asm)"));
  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      this->appendLog(tr("Cheat has some issues on reading"));
      return;
    }
    QString content = QString::fromUtf8(file.readAll());
    settings->setValue("files/cheat", fileName);
    this->appendLog(tr("Cheat loaded"));

    ui->cheats->setPlainText(content);
  }
}

void
MainWindow::readRom()
{
  QString fileName = ui->input_path->text();
  if (QFileInfo::exists(fileName)) {
    getromname(fileName.toLocal8Bit().data(), romname);
    ui->romname->setText(romname);
    this->appendLog(tr("Valid rom found"));
  } else {
    ui->romname->setText(tr("Input file not found"));
  }
}

void
MainWindow::appendLog(QString text)
{
  ui->log->appendPlainText(text);
}

bool
MainWindow::isOutputDefined()
{
  QString fileName = ui->output_path->text();
  if (fileName.isEmpty()) {
    return false;
  }
  return true;
}

bool
MainWindow::deadbeef()
{
  this->appendLog(tr("DEADBEEF process in progress"));
  if (this->isOutputDefined()) {
    char* output = ui->output_path->text().toLocal8Bit().data();
    char* input = ui->input_path->text().toLocal8Bit().data();

    QString status = deadbeefrom(input, output);
    this->appendLog(QString(output));
    if (status.compare("-1") == 0) {
      this->appendLog(tr("Something went wrong"));
      return false;
    }

    this->appendLog(status);
    this->appendLog(tr("DEADBEEF function added successfully"));
    this->appendLog(tr("Run the game in an emulator and look at "
                       "0x3000000-0x300007FFF for DEADBEEFs"));
    this->appendLog(tr("0x3007F00-0x3007FFF is a good place to start"));
    this->appendLog(tr("Then enter the RAM to use address where at least 2 "
                       "DEADBEEFs are in a row"));
  } else {
    this->appendLog(tr("Output not defined"));
  }

  return true;
}

void
MainWindow::patchGame()
{
  QString str;
  myslomostruct.wantslomo = 0;
  myedstruct.wantenable = 0;
  this->appendLog(tr("Game patching in progress"));
  if (this->isOutputDefined()) {
    QString menu_text = ui->menu_text->text().toUpper();
    if (ui->enable_trainer->isChecked()) {
      myedstruct.wantenable = 1;
      this->appendLog(tr("Trainer enabled"));
    }

    myedstruct.enablekey = ConvertKeys(ui->trainer_enable_keys->text().toLocal8Bit().data());
    if (myedstruct.enablekey == 0x3ff) {
      myedstruct.enablekey = 0xfe;
      ui->trainer_enable_keys->setText("L+R+A");
    }
    sprintf(myedstruct.enablekeystr, "%s", ui->trainer_enable_keys->text().toLocal8Bit().data());

    myedstruct.disablekey = ConvertKeys(ui->trainer_disable_keys->text().toLocal8Bit().data());
    if (myedstruct.disablekey == 0x3ff) {
      myedstruct.disablekey = 0xfd;
      ui->trainer_disable_keys->setText("L+R+B");
    }
    sprintf(myedstruct.disablekeystr, "%s", ui->trainer_disable_keys->text().toLocal8Bit().data());

    unsigned int* menuint = (unsigned int*)malloc(0x1000);
    memset(menuint, 0, 0x1000);
    unsigned int* cheatint = (unsigned int*)malloc(0x8000);
    memset(cheatint, 0, 0x8000);
    int cheatintlength = 0;
    int cheatselectram = hextoint(ui->ram_block->currentText().toLocal8Bit().data());

    if (ui->cheats->toPlainText().length() > 0) {
      char* cheatcodes = ui->cheats->toPlainText().toLocal8Bit().data();
      if (testcht(cheatcodes, QString("[gameinfo]").toLocal8Bit().data()) == 1) {
        importcht(cheatcodes);
      }

      char* formatted_cheatcodes = formatcheats(cheatcodes);
      if (ui->mode->currentText() == "Codebreaker/GS V3") { // cb/gssp
        cheatintlength = convertcb(formatted_cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
      } else { // raw
        cheatintlength = convertraw(formatted_cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
      }

      this->appendLog(tr("Cheats added"));
    } else {
      free(menuint);
      free(cheatint);
    }

    unsigned int* temptrainermenuint;
    temptrainermenuint = (unsigned int*)malloc(*trainermenuint + 4);
    memcpy(temptrainermenuint, trainermenuint, *trainermenuint + 4);
    // TODO: if there is a title create the trainer menu
    if (menu_text.length() > 0) {
      settings->setValue("rom/menutitle", menu_text);
      char* trainermenuchar = (char*)temptrainermenuint + 1;
      char* menutitle;

      QStringList lines = menu_text.split("/", QString::SkipEmptyParts);
      int trainerlines = lines.count();

      for (int thistrainerline = 0; thistrainerline < lines.count(); thistrainerline++) {
        menutitle = lines[thistrainerline].toLocal8Bit().data();
        if (strlen(menutitle) > 26) {
          menutitle[26] = 0;
        }
        *(trainermenuchar + *temptrainermenuint - 92 + (thistrainerline * 30)) = (char)((240 - (strlen(menutitle) * 9)) / 2);
        *(trainermenuchar + *temptrainermenuint - 91 + (thistrainerline * 30)) =
          (char)((42 - (trainerlines * 14)) / 2) + 14 * thistrainerline;
        memcpy(trainermenuchar + *temptrainermenuint - 90 + (thistrainerline * 30), menutitle, strlen(menutitle));
      }
      this->appendLog(tr("Menu title added"));
    }

    if (ui->enable_slowmotion->isChecked()) {
      myslomostruct.wantslomo = 1;
      myslomostruct.slowdownkey = ConvertKeys(ui->slowmotion_slow_keys->text().toLocal8Bit().data());
      if (myslomostruct.slowdownkey == 0x3ff) {
        myslomostruct.slowdownkey = 0xbf;
        ui->slowmotion_slow_keys->setText("L+R+UP");
      }
      sprintf(myslomostruct.slowdownkeystr, "%s", ui->slowmotion_slow_keys->text().toLocal8Bit().data());

      myslomostruct.speedupkey = ConvertKeys(ui->slowmotion_up_keys->text().toLocal8Bit().data());
      if (myslomostruct.speedupkey == 0x3ff) {
        myslomostruct.speedupkey = 0x7f;
        ui->slowmotion_up_keys->setText("L+R+DOWN");
      }
      sprintf(myslomostruct.speedupkeystr, "%s", ui->slowmotion_up_keys->text().toLocal8Bit().data());

      this->appendLog(tr("Slowmotion enabled"));
    }

    if (myslomostruct.wantslomo == 0 && myedstruct.wantenable == 0) {
      this->appendLog(tr("You didn't select any patches!"));
      free(temptrainermenuint);
      return;
    }

    patchrom(ui->input_path->text().toLocal8Bit().data(), ui->output_path->text().toLocal8Bit().data(), cheatint, cheatintlength,
             cheatselectram, myslomostruct, myedstruct, ui->execute_every->text().toInt(), (menu_text.length() > 0), menuint,
             cheatselectram + 4, ui->vblank->isChecked(), temptrainermenuint, wantbg, wantfont, wantselect);
    this->appendLog(tr("Game patched"));
  } else {
    this->appendLog(tr("Output not defined"));
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}
