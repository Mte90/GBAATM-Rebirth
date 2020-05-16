#include "mainwindow.h"
#include "class/cheatcodes.cpp"
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
  this->setWindowTitle("GBAATM-Rebirth - 1.0.0 Alpha");
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
  QFileInfo info(settings->value("files/input").toString());
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open background image"), info.dir().path(), tr("Image Files (*.bmp)"));
  if (!fileName.isEmpty()) {
    int goodbmp;
    goodbmp = bmp2short(fileName.toLocal8Bit().data(), menubgshort, 1);
    if (goodbmp == 1)
      customizetrainer.background = 1;
  }
}

void
MainWindow::loadFont()
{
  QFileInfo info(settings->value("files/input").toString());
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open font image"), info.dir().path(), tr("Image Files (*.bmp)"));
  if (!fileName.isEmpty()) {
    int goodbmp;
    goodbmp = bmp2short(fileName.toLocal8Bit().data(), menufontshort, 3);
    if (goodbmp == 1)
      customizetrainer.font = 1;
  }
}

void
MainWindow::loadSelectionBar()
{
  QFileInfo info(settings->value("files/input").toString());
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open background image"), info.dir().path(), tr("Image Files (*.bmp)"));
  if (!fileName.isEmpty()) {
    int goodbmp;
    goodbmp = bmp2short(fileName.toLocal8Bit().data(), menuselectshort, 2);
    if (goodbmp == 1)
      customizetrainer.selectionbar = 1;
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

void
MainWindow::removeIfExists(QString path)
{
  if (QFileInfo::exists(path)) {
    QFile file(path);
    file.remove();
  }
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
  ui->log->setPlainText("");
  this->appendLog(tr("DEADBEEF process in progress"));
  if (this->isOutputDefined()) {
    this->removeIfExists(ui->output_path->text());
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

bool
MainWindow::prepareKeys()
{
  if (ui->trainer_enable_keys->text().length() > 0) {
    traineractions.enablekey = ConvertKeys(ui->trainer_enable_keys->text().toLocal8Bit().data());
    if (traineractions.enablekey == 0x3ff) {
      traineractions.enablekey = 0xfe;
      ui->trainer_enable_keys->setText("L+R+A");
    }
    sprintf(traineractions.enablekeystr, "%s", ui->trainer_enable_keys->text().toLocal8Bit().data());
  } else {
    this->appendLog(tr("Enable keys is empty"));
    return false;
  }

  if (ui->trainer_disable_keys->text().length() > 0) {
    traineractions.disablekey = ConvertKeys(ui->trainer_disable_keys->text().toLocal8Bit().data());
    if (traineractions.disablekey == 0x3ff) {
      traineractions.disablekey = 0xfd;
      ui->trainer_disable_keys->setText("L+R+B");
    }
    sprintf(traineractions.disablekeystr, "%s", ui->trainer_disable_keys->text().toLocal8Bit().data());
  } else {
    this->appendLog(tr("Disable keys is empty"));
    return false;
  }
  return true;
}

void
MainWindow::patchGame()
{
  ui->log->setPlainText("");
  this->appendLog(tr("Game patching in progress"));
  if (this->isOutputDefined()) {
    this->appendLog(tr("Trainer enabled"));
    if (!this->prepareKeys()) {
      return;
    }

    Cheatcodes cheats;
    if (ui->cheats->toPlainText().length() > 0) {
      int cheat_type;
      if (ui->mode->currentText() == "Codebreaker/GS V3") { // cb/gssp
        cheat_type = 1;
      } else if (ui->mode->currentText() == "RAW (VBA)") { // raw
        cheat_type = 2;
      } else { // gs
        cheat_type = 3;
      }

      cheats.init(ui->cheats->toPlainText().toLocal8Bit().data(), ui->ram_block->currentText().toLocal8Bit().data(), cheat_type);

      this->appendLog(tr("Cheats added"));

      if (ui->menu_text->text().length() == 0) {
        ui->menu_text->setText(ui->romname->text());
      }

      settings->setValue("rom/menutitle", ui->menu_text->text().toUpper());
      cheats.titleGeneration(ui->menu_text->text());
    } else {
      this->appendLog(tr("Cheatcodes not imported"));
      return;
    }

    this->removeIfExists(ui->output_path->text());
    QString output = patchrom(ui->input_path->text().toLocal8Bit().data(), ui->output_path->text().toLocal8Bit().data(), cheats,
                              traineractions, ui->execute_every->text().toInt(), ui->vblank->isChecked(), customizetrainer);
    this->appendLog(output);
  } else {
    this->appendLog(tr("Output not defined"));
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}
