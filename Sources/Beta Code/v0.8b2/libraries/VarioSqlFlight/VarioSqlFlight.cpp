#include <Arduino.h>

#include <HardwareConfig.h>
#include <DebugConfig.h>

#ifdef WIFI_DEBUG
#define ARDUINOTRACE_ENABLE 1
#else
#define ARDUINOTRACE_ENABLE 0
#endif

#define ARDUINOTRACE_SERIAL SerialPort
#include <ArduinoTrace.h>

#ifdef HAVE_SDCARD
#include <sdcardHAL.h>
#endif

#include "VarioSqlFlight.h"

VarioSqlFlight::VarioSqlFlight()
{

#ifdef SQL_DEBUG
    SerialPort.println("VarioSqlFlight");
#endif //SQL_DEBUG

    sqlite3 *db1;

    int rc;

#ifdef SQL_DEBUG
    SerialPort.println("sqlite3_initialize");
#endif //SQL_DEBUG

    sqlite3_initialize();

#ifdef SQL_DEBUG
    SerialPort.println("openDb");
    // if (SDHAL_SD.exists("/mdr512.db"))
    // {
    //     SerialPort.println("Le fichier existe");
    //     // SDHAL.begin();
    // }
#endif //SQL_DEBUG

    openDb("mdr512.db", &db1);
    openDb("/mdr512.db", &db1);
    openDb("/sdcard/mdr512.db", &db1);
    openDb("/sd/mdr512.db", &db1);
    openDb("/data/mdr512.db", &db1);
    openDb("/SD0/mdr512.db", &db1);
    openDb("/SD1/mdr512.db", &db1);
    openDb("/SD2/mdr512.db", &db1);
    openDb("/flash/mdr512.db", &db1);

    return;

    if (openDb("/sd/mdr512.db", &db1))
        return;

    rc = db_exec(db1, "SELECT * FROM flight WHERE pilot = 'Pascal'");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }

    sqlite3_close(db1);
}

int VarioSqlFlight::callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    Serial.printf("%s: ", (const char *)data);
    for (i = 0; i < argc; i++)
    {
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    Serial.printf("\n");
    return 0;
}

int VarioSqlFlight::openDb(const char *filename, sqlite3 **db)
{
    int rc = sqlite3_open(filename, db);
    if (rc)
    {
        Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    else
    {
        Serial.printf("Opened database successfully\n");
    }
    return rc;
}

int VarioSqlFlight::db_exec(sqlite3 *db, const char *sql)
{
    Serial.println(sql);
    long start = micros();
    int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        Serial.printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        Serial.printf("Operation done successfully\n");
    }
    Serial.print(F("Time taken:"));
    Serial.println(micros() - start);
    return rc;
}