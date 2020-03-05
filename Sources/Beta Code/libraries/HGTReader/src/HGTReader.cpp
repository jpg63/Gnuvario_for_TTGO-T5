#include <sdcardHAL.h>
#include "HGTReader.h"

/**
  \brief Constructeur de classe.
  \param aglDir: répertoire contenant les fichiers HGT.
*/
HGTReader::HGTReader(const String& aglDir) : aglDir(aglDir), currentFileName("")
{}

/**
  \brief Calcule la hauteur du sol à la position donnée.
  \param latitude: latitude de la position au format Degrés Minutes Secondes.
  \param longitude: longitude de la position au format Degrés Minutes Secondes.
  \return une altitude en mètres.
*/
int HGTReader::getGroundLevel(const String& latitude, const String& longitude)
{
  String fileName = getFileNameForPosition(latitude, longitude);
  if (fileName != currentFileName) {
    if (currentFileName != "") {

      currentFile.close();
    }
    if (openFile(fileName)) {
      currentFileName = fileName;
    }
    else
    {
      currentFileName = "";
    }
  }
  if (currentFileName != "")
  {

    return NO_FILE_FOR_POS;
  }
  return loadGroundLevel(latitude, longitude);
}

/**
  \brief Calcule la hauteur du sol à la position donnée.
  \param latitude: latitude de la position au format Degrés Minutes Secondes.
  \param longitude: longitude de la position au format Degrés Minutes Secondes.
  \return une altitude en mètres.
*/
int HGTReader::getGroundLevel(float latitude, float longitude)
{
  Serial.printf("getGroundLevel(%f,%f)\n", latitude, longitude);
  String fileName = getFileNameForPosition(latitude, longitude);
  Serial.printf("Filename : %s\n", fileName.c_str());
  if (fileName != currentFileName) {
    if (currentFileName != "") {
      Serial.printf("Close %s\n", currentFileName.c_str());
      currentFile.close();
    }
    if (openFile(fileName)) {
      Serial.printf("Open %s\n", currentFileName.c_str());
      currentFileName = fileName;
    }
    else
    {
      Serial.printf("Echec ouverture\n");
      currentFileName = "";
    }
  }
  if (currentFileName == "")
  {
    Serial.printf("Pas de fichier\n");
    return NO_FILE_FOR_POS;
  }
  return loadGroundLevel(latitude, longitude);
}

/**
  \brief Génère le nom de fichier contenant la hauteur du sol à la position donnée.
  \param longitude: longitude de la position au format Degrés Minutes Secondes.
  \param latitude: latitude de la position au format Degrés Minutes Secondes.
  \return un nom de fichier.
*/
String HGTReader::getFileNameForPosition(const String& latitude, const String& longitude)
{
  int iIntLat = latitude.substring(0, latitude.indexOf(".")).toInt();
  String sLatDir = latitude.substring(latitude.length() - 1);
  if (sLatDir == "S") {
    iIntLat += 1;
  }
  int iIntLong = longitude.substring(0, longitude.indexOf(".")).toInt();
  String sLongDir = longitude.substring(longitude.length() - 1);
  if (sLongDir == "W") {
    iIntLong +=1;
  }
  char  r[12];
  sprintf(r, "%s%02d%s%03d%s", sLatDir.c_str(), iIntLat, sLongDir.c_str(), iIntLong, String(FILE_EXTENSION).c_str());
  Serial.println(r);
  return String(r);
}

/**
  \brief Génère le nom de fichier contenant la hauteur du sol à la position donnée.
  \param longitude: longitude de la position au format Degrés Minutes Secondes.
  \param latitude: latitude de la position au format Degrés Minutes Secondes.
  \return un nom de fichier.
*/
String HGTReader::getFileNameForPosition(float latitude, float longitude)
{
  int iLatDec = (int)floor(latitude);
  int iLonDec = (int)floor(longitude);
  char cLatDir = iLatDec > 0 ? 'N' : 'S';
  char cLonDir = iLonDec > 0 ? 'E' : 'W';
  char  r[30];
  sprintf(r, "%c%02d%c%03d%s", cLatDir, abs(iLatDec), cLonDir, abs(iLonDec), String(FILE_EXTENSION).c_str());  
  String tmp = String(r);
  Serial.println(tmp.c_str());
  return tmp;
}

/**
  \brief Ouvre un fichier dans le répertoire passé au constructeur de classe.
  \param file: le fichier à ouvrir.
  \return true si l'ouverture s'est bien passée.
*/
bool HGTReader::openFile(const String& fileName)
{
  String path = String(aglDir) + fileName;
  Serial.printf("Path : %s\n", path.c_str());  
  if (currentFile.open(path.c_str(), O_RDONLY))
  {
    Serial.printf("Open OK\n");
    return true;
  }
  else
  {
    Serial.printf("Open KO\n");
    return false;
  }
}

/**
  \brief Lit la hauteur du sol dans un fichier chargé.
  \param latitude: latitude de la position au format Degrés Minutes Secondes Direction.
  \param longitude: longitude de la position au format Degrés Minutes Secondes Direction.
  \return une altitude en mètres.
*/
int HGTReader::loadGroundLevel(const String& latitude, const String& longitude)
{
  float fLat = latitude.substring(0, latitude.indexOf(' ')).toFloat();
  float fLon = longitude.substring(0, longitude.indexOf(' ')).toFloat();
  if(latitude.substring(latitude.length() - 1) == "S" ) fLat = -fLat;
  if(longitude.substring(longitude.length() - 1) == "W" ) fLon = -fLon;
  return loadGroundLevel(fLat, fLon);
}

/**
  \brief Lit la hauteur du sol dans un fichier chargé.
  \param latitude: latitude de la position au format Degrés Minutes Secondes Direction.
  \param longitude: longitude de la position au format Degrés Minutes Secondes Direction.
  \return une altitude en mètres.
*/
int HGTReader::loadGroundLevel(float latitude, float longitude)
{
   Serial.printf("loadGroundLevel\n");
  
  int latDec = (int)floor(latitude);
  int lonDec = (int)floor(longitude);

  float secondsLat = (latitude-latDec) * NB_SEC_PER_DEG;
  float secondsLon = (longitude-lonDec) * NB_SEC_PER_DEG;

  int y = secondsLat/NB_SEC_PAR_TILE;
  int x = secondsLon/NB_SEC_PAR_TILE;

  int height[4];
  height[2] = getTileHeight(x, y);
  height[0] = getTileHeight(x, y+1);
  height[3] = getTileHeight(x+1, y);
  height[1] = getTileHeight(x+1, y+1);

  float dy = fmod(secondsLat , NB_SEC_PAR_TILE) / NB_SEC_PAR_TILE;
  float dx = fmod(secondsLon , NB_SEC_PAR_TILE) / NB_SEC_PAR_TILE;

  return  height[0] * dy * (1 - dx) +
          height[1] * dy * (dx) +
          height[2] * (1 - dy) * (1 - dx) +
          height[3] * (1 - dy) * dx;
}

int HGTReader::getTileHeight(int x, int y)
{
  int offsetX = 2 * ( x+ ( NB_PT_PER_ROW - y - 1) * NB_PT_PER_ROW);
  currentFile.seekSet(offsetX);
  //Serial.printf("offset = %d\n",offsetX);
  unsigned char b[2];
  currentFile.read(b, 2);
  int h = (b[0] << 8) + b[1];
  //Serial.printf("h = %d\n",h);
  return h;

}

/**
  \brief Fonction utilitaire : complète une chaine de caractères avec un caractère.
  \param src: la chaine à compléter.
  \param size: la taille de chaine à atteindre.
  \param c: le caractère pour compléter la chaine.
  \return une chaine de taille size (si src est moins longue que size)
*/
String HGTReader::rightPad(const String& src, int size, char c)
{
  String r = src;
  while (src.length() < size) {
    r = r + c;
  }
  return r;
}
