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
  bool prepareKeys();

private:
  Ui::MainWindow* ui;
  QSettings* settings;
};
#endif // MAINWINDOW_H
