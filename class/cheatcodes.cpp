#include "class/cheatcodes.h"
#include "core/trainermenu.h"
#include "functions.h"
#include <QString>
#include <QTextStream>

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

  if (this->test(cheatcodes, QString("[gameinfo]").toLocal8Bit().data()) == 1 || this->test(cheatcodes, QString("[GameInfo]").toLocal8Bit().data()) == 1) {
    cheatcodes = this->import(cheatcodes);
  }

  cheatcodes = this->format(cheatcodes);

  if (cheat_type == 1) {
    cheatintlength = convertcb(cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
  } else if (cheat_type == 2) {
    cheatintlength = convertraw(cheatcodes, cheatint, 1, cheatselectram + 4, menuint);
  } else if (cheat_type == 2) {
    cheatintlength = convertgs(cheatcodes);
  }
}

void
Cheatcodes::titleGeneration(QString title)
{
  char* trainermenuchar = (char*)temptrainermenuint + 1;
  char* menutitle;

  QStringList lines = title.toUpper().split("/", QString::SkipEmptyParts);
  int trainerlines = lines.count();

  for (int thistrainerline = 0; thistrainerline < lines.count(); thistrainerline++) {
    menutitle = lines[thistrainerline].toLocal8Bit().data();
    if (strlen(menutitle) > 26) {
      menutitle[26] = 0;
    }
    *(trainermenuchar + *temptrainermenuint - 92 + (thistrainerline * 30)) = (char)((240 - (strlen(menutitle) * 9)) / 2);
    *(trainermenuchar + *temptrainermenuint - 91 + (thistrainerline * 30)) = (char)((42 - (trainerlines * 14)) / 2) + 14 * thistrainerline;
    memcpy(trainermenuchar + *temptrainermenuint - 90 + (thistrainerline * 30), menutitle, strlen(menutitle));
  }
}

char*
Cheatcodes::format(char* cheatcodechar)
{
  char goodcodechar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', ' ' };
  char lastchar[] = { 0, 0 };
  char thischar[] = { 0, 0 };
  int labellast = 0;
  char* templine = (char*)malloc(500);
  memset(templine, 0, 500);
  char* tempchar = (char*)malloc(strlen(cheatcodechar) * 5);
  memset(tempchar, 0, strlen(cheatcodechar) * 5);
  int howmanylines = 0;
  strcat(cheatcodechar, "\r\n");
  for (unsigned int strptr = 0; strptr < strlen(cheatcodechar); strptr++) {
    thischar[0] = *(cheatcodechar + strptr);

    if (thischar[0] == '\t')
      thischar[0] = '\n';
    if ((thischar[0] == ':') || (thischar[0] == '-')) {
      thischar[0] = ' ';
    }

    if (((thischar[0] < 0x20) || (thischar[0] > 0x7e)) && (thischar[0] != 0xa) && (thischar[0] != 0x9)) {
      lastchar[0] = thischar[0];
    }
    if ((thischar[0] >= 0x61) && (thischar[0] <= 0x7a)) {
      thischar[0] -= 0x20;
    }

    if (lastchar[0] != thischar[0]) {
      if ((thischar[0] != 0x9) && (thischar[0] != 0xd)) {
        strcat(templine, QString(thischar).remove("\r").remove("\n").toLocal8Bit().data());
        lastchar[0] = thischar[0];
      }

      if (thischar[0] == 0xa) {
        if (templine[0] != '/') {
          howmanylines++;
          int goodchars = 0;
          for (unsigned int charptr = 0; charptr < strlen(templine); charptr++) {
            for (unsigned int goodcharptr = 0; goodcharptr < strlen(goodcodechar); goodcharptr++) {
              if (templine[charptr] == goodcodechar[goodcharptr]) {
                goodchars++;
              }
            }
          }

          if ((goodchars >= 10) && ((strlen(templine) - goodchars) < 5)) {
            if (goodchars < 20) {
              char tempcpychar[2];
              memset(tempcpychar, 0, 2);
              char* newtempline = (char*)malloc(strlen(templine) + 20);
              memset(newtempline, 0, strlen(templine) + 7);
              int tempcpyptr = 0;
              while ((tempcpychar[0] != ' ') && (tempcpyptr < (int)strlen(templine))) {
                tempcpychar[0] = *(templine + tempcpyptr);
                if (tempcpychar[0] != ' ') {
                  strcat(newtempline, tempcpychar);
                }
                tempcpyptr++;
              }

              if (tempcpyptr == (int)strlen(templine)) {
                tempcpyptr = strlen(templine) - 4;
                *(newtempline + strlen(templine) - 4) = 0;
              }
              strcat(newtempline, " ");
              memset(newtempline + strlen(newtempline), 0, strlen(templine) + 20 - strlen(newtempline));
              memset(newtempline + tempcpyptr + 1, '0', 8 - (strlen(templine) - tempcpyptr));
              sprintf(newtempline + strlen(newtempline), "%s", templine + tempcpyptr);
              labellast = 0;
              sprintf(templine, "%s\n", newtempline);

              if (tempcpyptr == (int)strlen(templine)) {
                QTextStream(stdout) << QString("Bad code detected: %1").arg(templine);
                templine[0] = 0;
              }

              free(newtempline);
            }
            strcat(tempchar, templine);
          } else { // label or bad code
            if (strlen(templine) > 1) {
              if ((labellast == 0) && (howmanylines > 1)) {
                strcat(tempchar, "}\n");
              }
              sprintf(tempchar + strlen(tempchar), "[%s]\n{\n", templine);
              labellast = 1;
            }
          }
        }
        memset(templine, 0, 500);
      }

    } else {
      lastchar[0] = thischar[0];
      if ((thischar[0] != 0x20) && (thischar[0] != 0xa)) {
        strcat(templine, thischar);
      }
    }
  }

  // Add missing closing parenthesis
  if (QString(tempchar).count(QLatin1Char('{')) > QString(tempchar).count(QLatin1Char('}'))) {
    strcat(tempchar, "}\n");
  }

  return tempchar;
}

int
Cheatcodes::test(char* cheatcodechar, char* srchstr)
{
  char* tempchar = (char*)malloc(strlen(cheatcodechar) + 10);
  char* tempsrch = (char*)malloc(strlen(srchstr) + 10);
  memset(tempchar, 0, strlen(cheatcodechar) + 10);
  memset(tempsrch, 0, strlen(srchstr) + 10);
  char thischar;
  for (int charptr = 0; charptr < (int)strlen(cheatcodechar); charptr++) {
    thischar = *(cheatcodechar + charptr);
    if ((thischar >= 0x61) && (thischar <= 0x7a))
      thischar -= 0x20;
    *(tempchar + charptr) = thischar;
  }

  for (int charptr = 0; charptr < (int)strlen(srchstr); charptr++) {
    thischar = *(srchstr + charptr);
    if ((thischar >= 0x61) && (thischar <= 0x7a))
      thischar -= 0x20;
    *(tempsrch + charptr) = thischar;
  }
  int match = 0;
  if (strstr(tempchar, tempsrch))
    match = 1;
  free(tempsrch);
  free(tempchar);
  return match;
}

char*
Cheatcodes::import(char* cheatcodechar)
{
  QTextStream(stdout) << "CHT format detected -- importing cheats";

  char* tempchar = (char*)malloc(MAXCODELEN);
  memset(tempchar, 0, MAXCODELEN);

  char* endcht = strstr(cheatcodechar, "GameInfo");
  if (endcht)
    memset(endcht, 0, 8);

  int myptr = 0;

  QString cheatline;
  char* lastdesc = (char*)malloc(MAXCHTLINE);
  char* lastlabel = NULL;
  while (myptr < (int)strlen(cheatcodechar)) {
    getnextchtline(cheatcodechar, &myptr, cheatline.toLocal8Bit().data());

    if (test(cheatline.toLocal8Bit().data(), (char*)"OFF=") == 1)
      continue;
    if (cheatline.length() > 0) {
      cheatline.replace("[", (char*)"");
      cheatline.replace("]", (char*)"");
      cheatline.replace(",\r\n", (char*)"\r\n");
      cheatline.replace(";\r\n", (char*)"\r\n");
      cheatline.replace("ON=\r\n", (char*)"");
      cheatline.replace("ON=", (char*)"");
      cheatline.replace("0=", (char*)"");
      cheatline.replace("MAX=", (char*)"");
      if (cheatline == '=')
        cheatline.replace("=", (char*)"");
      cheatline.replace(";", (char*)"\r\n");
      cheatline.replace("TEXT=", (char*)"");
      cheatline.replace("Text=", (char*)"");
      if (cheatline.length() > 1)
        sprintf(tempchar + strlen(tempchar), "%s\r\n", cheatline.toLocal8Bit().data());
    }
  }

  memset(cheatcodechar, 0, MAXCODELEN);
  memcpy(cheatcodechar, tempchar, strlen(tempchar));
  memset(tempchar, 0, MAXCODELEN);

  myptr = 0;

  while (myptr < (int)strlen(cheatcodechar)) {
    getnextcheatline(cheatcodechar, &myptr, cheatline.toLocal8Bit().data());
    if (cheatline.length() == 0)
      continue;
    if (cheatline == '=')
      continue;
    char* multimodchar = strstr(cheatline.toLocal8Bit().data(), "=");
    if (multimodchar) {
      if (lastlabel != NULL) {
        *lastlabel = 0;
        lastlabel = NULL;
      }

      *multimodchar = 0;
      sprintf(tempchar + strlen(tempchar), "%s - %s\r\n", lastdesc, cheatline.toLocal8Bit().data());
      for (int copyptr = 0; copyptr < (int)strlen(multimodchar + 1); copyptr++) {
        cheatline[copyptr] = *(multimodchar + 1 + copyptr);
      }
      cheatline.insert(strlen(multimodchar + 1), 0);
    }

    int cheat = testchtline(cheatline.toLocal8Bit().data());

    if (cheat == 1) {
      cheatline.replace(":", (char*)",");
      char* addrchar = strstr(cheatline.toLocal8Bit().data(), ",");
      *addrchar = 0;
      addrchar++;
      int addressint = hextoint(cheatline.toLocal8Bit().data());
      if (addressint == 0x400130) {
        sprintf(tempchar + strlen(tempchar), "D0000020 %04X\r\n", hextoint(addrchar) ^ 0xff);
        continue;
      }
      if ((addressint == 0) || (addressint > 0x41FFFFF)) {
        sprintf(tempchar + strlen(tempchar), "//%08X:%s ;pointer code????\r\n", addressint, addrchar);
        continue;
      }

      if (addressint <= 0x3FFFF)
        addressint |= 0x2000000;
      else if ((addressint & 0xf000000) == 0)
        addressint = (addressint & 0xffff) | 0x3000000;

      char* morecommas = strstr(addrchar, ",");
      if (!morecommas) {

        switch (strlen(addrchar)) {
          case 1:
          case 2:
            sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint, hextoint(addrchar));
            break;
          case 3:
          case 4:
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, hextoint(addrchar));
            break;
          default:
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, hextoint(addrchar) & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 2, (hextoint(addrchar) >> 16) & 0xffff);
        }

      } else {
        unsigned int values[1024];
        memset(&values, 0, sizeof(int) * 1024);
        int commacount = countcommas(addrchar);
        char* tempaddrchar = addrchar;
        char* newaddrchar;
        int size = 1;
        for (int commaptr = 0; commaptr < commacount + 1; commaptr++) {
          newaddrchar = strstr(tempaddrchar, ",");
          if (newaddrchar)
            *newaddrchar = 0;
          values[commaptr] = hextoint(tempaddrchar);
          if ((values[commaptr] > 0xff) && (size < 2))
            size = 2;
          if (values[commaptr] > 0xffff)
            size = 4;
          tempaddrchar = newaddrchar + 1;
        }

        if (size == 1) {
          switch (commacount) {
            case 1:
              sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint, values[1], values[0]);
              break;
            case 2:
              if ((addressint % 2) == 0) { // even address to start
                sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint, values[1], values[0]);
                sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint + 2, values[2]);
              } else {
                sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint, values[0]);
                sprintf(tempchar + strlen(tempchar), "3%X %02X%02X\r\n", addressint + 1, values[2], values[1]);
              }
              break;
            case 3:
              sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint, values[1], values[0]);
              sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint + 2, values[3], values[2]);
              break;
            default:
              int commahalf = ((int)((commacount + 1) / 2));
              sprintf(tempchar + strlen(tempchar), "5%X %04X\r\n", addressint, commahalf);
              int commaptr = 0;
              while (commaptr < commahalf) {
                int thisval[] = { 0, 0, 0 };
                if (commaptr < commahalf)
                  thisval[0] = ((values[commaptr * 2 + 1]) & 0xff) | ((values[commaptr * 2] & 0xff) << 8);
                if (commaptr + 1 < commahalf)
                  thisval[1] = ((values[commaptr * 2 + 3]) & 0xff) | ((values[commaptr * 2 + 2] & 0xff) << 8);
                if (commaptr + 2 < commahalf)
                  thisval[2] = ((values[commaptr * 2 + 5]) & 0xff) | ((values[commaptr * 2 + 4] & 0xff) << 8);
                sprintf(tempchar + strlen(tempchar), "%04X%04X %04X\r\n", thisval[0], thisval[1], thisval[2]);
                commaptr += 3;
              }
              if ((commacount + 1) % 2 != 0)
                sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint + commahalf * 2, values[commacount]);
          }
        }
        if (size == 2) {
          if (commacount == 1) {
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, values[0]);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 2, values[1]);
            continue;
          }

          if (commacount > 1) {
            sprintf(tempchar + strlen(tempchar), "5%X %04X\r\n", addressint, commacount + 1);
            for (int commactr = 0; commactr < commacount + 1; commactr += 3) {
              int thisval[] = { 0, 0, 0 };
              if (commactr < commacount + 1)
                thisval[0] = ((values[commactr] >> 8) & 0xff) | ((values[commactr] & 0xff) << 8);
              if (commactr + 1 < commacount + 1)
                thisval[1] = ((values[commactr + 1] >> 8) & 0xff) | ((values[commactr + 1] & 0xff) << 8);
              if (commactr + 2 < commacount + 1)
                thisval[2] = ((values[commactr + 2] >> 8) & 0xff) | ((values[commactr + 2] & 0xff) << 8);
              sprintf(tempchar + strlen(tempchar), "%04X%04X %04X\r\n", thisval[0], thisval[1], thisval[2]);
            }
          }
        }

        if (size == 4) {
          if (commacount == 1) {
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, values[0] & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 2, (values[0] >> 16) & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 4, values[1] & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 6, (values[1] >> 16) & 0xffff);
            continue;
          }

          if (commacount > 1) {
            sprintf(tempchar + strlen(tempchar), "5%X %04X\r\n", addressint, (commacount + 1) * 2);
            for (int commactr = 0; commactr < commacount + 1; commactr++) {
              int thisval = ((values[commactr] & 0xff) << 24) | (((values[commactr] >> 8) & 0xff) << 16) |
                            ((values[commactr] & 0xff0000) >> 8) | ((values[commactr] >> 24) & 0xff);
              if ((commactr % 3) == 0)
                sprintf(tempchar + strlen(tempchar), "%08X ", thisval);
              if ((commactr % 3) == 1)
                sprintf(tempchar + strlen(tempchar), "%04X\r\n%04X", (thisval >> 16) & 0xffff, thisval & 0xffff);
              if ((commactr % 3) == 2)
                sprintf(tempchar + strlen(tempchar), "%04X %04X\r\n", (thisval >> 16) & 0xffff, thisval & 0xffff);
            }
            if (((commacount + 1) % 3) == 1)
              sprintf(tempchar + strlen(tempchar), "0000\r\n");
            if (((commacount + 1) % 3) == 2)
              sprintf(tempchar + strlen(tempchar), "0000 0000\r\n");
          }
        }
      }
    } else {
      lastlabel = tempchar + strlen(tempchar);
      sprintf(tempchar + strlen(tempchar), "%s\r\n", cheatline.toLocal8Bit().data());
      memset(lastdesc, 0, MAXCHTLINE);
      sprintf(lastdesc, "%s", cheatline.toLocal8Bit().data());
    }
  }
  memset(cheatcodechar, 0, MAXCODELEN);
  memcpy(cheatcodechar, tempchar, MAXCODELEN);

  free(tempchar);

  return cheatcodechar;
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
