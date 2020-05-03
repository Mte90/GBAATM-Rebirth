#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  void openRom();
  void saveRom();
  void openCheat();
  bool deadbeef();
  void patchGame();
  bool isOutputDefined();
  void removeIfExists(QString path);
  void readRom();
  void appendLog(QString text);
  void loadBg();
  void loadFont();
  void loadSelectionBar();

private:
  Ui::MainWindow* ui;
  QSettings* settings;
  int wantbg = 0;
  int wantselect = 0;
  int wantfont = 0;
};
#endif // MAINWINDOW_H
