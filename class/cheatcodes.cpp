#include "class/cheatcodes.h"
#include "core/trainermenu.h"
#include "functions.h"
#include <QString>

void
Cheatcodes::init(char* cheatcodes, char* selectram, int cheat_type)
{
  unsigned int* menuint = (unsigned int*)malloc(0x1000);
  memset(menuint, 0, 0x1000);
  unsigned int* cheatint = (unsigned int*)malloc(0x8000);
  memset(cheatint, 0, 0x8000);
  int cheatintlength = 0;
  int cheatselectram = hextoint(selectram);
  unsigned int* temptrainermenuint;
  temptrainermenuint = (unsigned int*)malloc(*trainermenuint + 4);
  memcpy(temptrainermenuint, trainermenuint, *trainermenuint + 4);

  if (testcht(cheatcodes, QString("[gameinfo]").toLocal8Bit().data()) == 1) {
    importcht(cheatcodes);
  }

  char* formatted_cheatcodes = formatcheats(cheatcodes);

  if (cheat_type == 1) {
    cheatintlength = convertcb(formatted_cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
  } else {
    cheatintlength = convertraw(formatted_cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
  }
  return;
}

unsigned int*
Cheatcodes::getTrainerMenuInt()
{
  return temptrainermenuint;
}

int
Cheatcodes::getCheatLength()
{
  return cheatintlength;
}

int
Cheatcodes::getSelectRam()
{
  return cheatselectram;
}

unsigned int*
Cheatcodes::getMenuInt()
{
  return menuint;
}

unsigned int*
Cheatcodes::getCheatInt()
{
  return cheatint;
}
