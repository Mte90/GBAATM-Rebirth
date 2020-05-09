#include "class/cheatcodes.h"
#include "core/trainermenu.h"
#include "functions.h"
#include <QString>

void
Cheatcodes::init(char* cheatcodes, char* selectram, int cheat_type)
{
  menuint = (unsigned int*)malloc(0x1000);
  memset(menuint, 0, 0x1000);
  cheatint = (unsigned int*)malloc(0x8000);
  memset(cheatint, 0, 0x8000);
  cheatselectram = hextoint(selectram);
  temptrainermenuint = (unsigned int*)malloc(*trainermenuint + 4);
  memcpy(temptrainermenuint, trainermenuint, *trainermenuint + 4);

  if (testcht(cheatcodes, QString("[gameinfo]").toLocal8Bit().data()) == 1) {
    importcht(cheatcodes);
  }

  char* formatted_cheatcodes = formatcheats(cheatcodes);

  if (cheat_type == 1) {
    cheatintlength = convertcb(formatted_cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
  } else if (cheat_type == 2) {
    cheatintlength = convertraw(formatted_cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
  }
}

unsigned int*
Cheatcodes::getTempTrainerMenuInt()
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
