#ifndef CHEATCODES
#define CHEATCODES

class Cheatcodes
{
  char* cheatcodes;
  unsigned int* temptrainermenuint;
  unsigned int* menuint;
  unsigned int* cheatint;
  int cheatselectram;
  int cheatintlength;

public:
  void init(char* cheatcodes, char* selectram, int cheat_type);
  unsigned int* getTempTrainerMenuInt();
  int getCheatLength();
  int getSelectRam();
  unsigned int* getMenuInt();
  unsigned int* getCheatInt();
};
#endif // CHEATCODES
