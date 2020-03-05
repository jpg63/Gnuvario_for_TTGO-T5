#ifndef HGT_READER_H
#define HGT_READER_H

#include <sdcardHAL.h>

#define FILE_EXTENSION ".HGT"
#define NO_FILE_FOR_POS -1
#define NB_PT_PER_ROW 1201
#define NB_POS_DEC 6
#define NB_SEC_PER_DEG 3600
#define NB_SEC_PAR_TILE 3

class HGTReader
{
public :
  HGTReader(const String& aglDir);
  int getGroundLevel(const String& latitude, const String& longitude);
  int getGroundLevel(float latitude, float longitude);

private:
  String getFileNameForPosition(const String& latitude, const String& longitude);
  String getFileNameForPosition(float latitude, float longitude);
  bool openFile(const String& fileName);
  int loadGroundLevel(const String& latitude, const String& longitude);
  int loadGroundLevel(float latitude, float longitude);
  int getTileHeight(int x, int y);
  static String rightPad(const String& src, int size, char c);

  String aglDir;
  String currentFileName = "";
  SdFile currentFile;

};

#endif //HGT_READER_H