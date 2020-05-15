#ifndef VARIO_SQL_FLIGHT_H
#define VARIO_SQL_FLIGHT_H

#include <HardwareConfig.h>

#ifdef HAVE_SDCARD
#include <sdcardHAL.h>
#endif

#include <sqlite3.h>

class VarioSqlFlight
{

private:
    String dbPath;
    int openDb(const char *filename, sqlite3 **db);
    char *zErrMsg = 0;
    int db_exec(sqlite3 *db, const char *sql);
    const char *data = "Callback function called";
    static int callback(void *data, int argc, char **argv, char **azColName);

public:
    VarioSqlFlight();
};
#endif