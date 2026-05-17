// This program reads from a zaproo SQLite database (default: media.db) and
// lists game names in the format "SystemName-GameName.jpg".
// Make sure to update your zaproo media database before running this program to ensure it has the latest game information!
//
// Usage: gameLister.exe [path_to_database]
//
// Compile the code with: gcc gameLister.c -lsqlite3 -o gameLister.exe
// Insure to install the SQLite3 development libraries i.e. apt install sqlite3 libsqlite3-dev


#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

int main(int argc, char *argv[]) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *dbFile = (argc >= 2) ? argv[1] : "media.db";
    
    const char *sql = "SELECT s.Name, m.Path FROM Media m JOIN Systems s ON m.SystemDBID = s.DBID ORDER BY s.Name, m.Path;";
    int rc = sqlite3_open(dbFile, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Unable to open database 'media.db': %s\n", sqlite3_errmsg(db));
        if (db) {
            sqlite3_close(db);
        }
        return 1;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *systemName = sqlite3_column_text(stmt, 0);
        const unsigned char *path = sqlite3_column_text(stmt, 1);

        if (systemName && path) {
            char jpgPath[4096];
            const char *srcPath = (const char *)path;
            size_t len = strlen(srcPath);
            if (len >= sizeof(jpgPath)) {
                len = sizeof(jpgPath) - 1;
            }
            memcpy(jpgPath, srcPath, len);
            jpgPath[len] = '\0';

            char *lastSlash = strrchr(jpgPath, '/');
            char *lastBackslash = strrchr(jpgPath, '\\');
            char *lastSep = lastSlash;
            if (lastBackslash && (!lastSep || lastBackslash > lastSep)) {
                lastSep = lastBackslash;
            }

            char *filename = lastSep ? lastSep + 1 : jpgPath;
            char *dot = strrchr(filename, '.');
            if (dot) {
                *dot = '\0';
            }

            memmove(jpgPath, filename, strlen(filename) + 1);
            strcat(jpgPath, ".jpg");

            printf("%s-%s\n", systemName, jpgPath);
        }
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Query execution error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
