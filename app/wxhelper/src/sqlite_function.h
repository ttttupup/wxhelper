#ifndef WXHELPER_SQLITE_FUNCTION_H_
#define WXHELPER_SQLITE_FUNCTION_H_
#include <cstdint>
#include "offset.h"
namespace sqlite3 {
namespace offset = wechat::offset;

#define SQLITE_OK 0 /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR 1       /* Generic error */
#define SQLITE_INTERNAL 2    /* Internal logic error in SQLite */
#define SQLITE_PERM 3        /* Access permission denied */
#define SQLITE_ABORT 4       /* Callback routine requested an abort */
#define SQLITE_BUSY 5        /* The database file is locked */
#define SQLITE_LOCKED 6      /* A table in the database is locked */
#define SQLITE_NOMEM 7       /* A malloc() failed */
#define SQLITE_READONLY 8    /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT 9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR 10      /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT 11    /* The database disk image is malformed */
#define SQLITE_NOTFOUND 12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL 13       /* Insertion failed because database is full */
#define SQLITE_CANTOPEN 14   /* Unable to open the database file */
#define SQLITE_PROTOCOL 15   /* Database lock protocol error */
#define SQLITE_EMPTY 16      /* Internal use only */
#define SQLITE_SCHEMA 17     /* The database schema changed */
#define SQLITE_TOOBIG 18     /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT 19 /* Abort due to constraint violation */
#define SQLITE_MISMATCH 20   /* Data type mismatch */
#define SQLITE_MISUSE 21     /* Library used incorrectly */
#define SQLITE_NOLFS 22      /* Uses OS features not supported on host */
#define SQLITE_AUTH 23       /* Authorization denied */
#define SQLITE_FORMAT 24     /* Not used */
#define SQLITE_RANGE 25      /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB 26     /* File opened that is not a database file */
#define SQLITE_NOTICE 27     /* Notifications from sqlite3_log() */
#define SQLITE_WARNING 28    /* Warnings from sqlite3_log() */
#define SQLITE_ROW 100       /* sqlite3_step() has another row ready */
#define SQLITE_DONE 101      /* sqlite3_step() has finished executing */
/* end-of-error-codes */

#define SQLITE_INTEGER 1
#define SQLITE_FLOAT 2
#define SQLITE_BLOB 4
#define SQLITE_NULL 5
#define SQLITE_TEXT 3

typedef int (*sqlite3_callback)(void *, int, char **, char **);

typedef int(__cdecl *sqlite3_exec)(uint64_t,         /* An open database */
                                   const char *sql,  /* SQL to be evaluated */
                                   sqlite3_callback, /* Callback function */
                                   void *,       /* 1st argument to callback */
                                   char **errmsg /* Error msg written here */
);

typedef int(__cdecl *sqlite3_prepare)(
    uint64_t db,        /* Database handle */
    const char *zSql,   /* SQL statement, UTF-8 encoded */
    int nByte,          /* Maximum length of zSql in bytes. */
    uint64_t **ppStmt,  /* OUT: Statement handle */
    const char **pzTail /* OUT: Pointer to unused portion of zSql */
);
typedef int(__cdecl *sqlite3_open)(const char *filename, uint64_t **ppDb);

typedef int(__cdecl *sqlite3_sleep)(int);
typedef int(__cdecl *sqlite3_errcode)(uint64_t *db);
typedef int(__cdecl *sqlite3_close)(uint64_t *);

typedef int(__cdecl *sqlite3_step)(uint64_t *);
typedef int(__cdecl *sqlite3_column_count)(uint64_t *pStmt);
typedef const char *(__cdecl *sqlite3_column_name)(uint64_t *, int N);
typedef int(__cdecl *sqlite3_column_type)(uint64_t *, int iCol);
typedef const void *(__cdecl *sqlite3_column_blob)(uint64_t *, int iCol);
typedef int(__cdecl *sqlite3_column_bytes)(uint64_t *, int iCol);
typedef int(__cdecl *sqlite3_finalize)(uint64_t *pStmt);

/***************************sqlite3  end*************************************/


struct SqliteFunction {
  SqliteFunction(uint64_t base_addr) {
    sqlite3_exec =
        reinterpret_cast<sqlite3::sqlite3_exec>(base_addr + offset::k_sqlite3_exec);
    sqlite3_prepare = reinterpret_cast<sqlite3::sqlite3_prepare>(
        base_addr + offset::k_sqlite3_prepare);
    sqlite3_open =
        reinterpret_cast<sqlite3::sqlite3_open>(base_addr + offset::k_sqlite3_open);

    sqlite3_step =
        reinterpret_cast<sqlite3::sqlite3_step>(base_addr + offset::k_sqlite3_step);
    sqlite3_column_count = reinterpret_cast<sqlite3::sqlite3_column_count>(
        base_addr + offset::k_sqlite3_column_count);
    sqlite3_column_name = reinterpret_cast<sqlite3::sqlite3_column_name>(
        base_addr + offset::k_sqlite3_column_name);
    sqlite3_column_type = reinterpret_cast<sqlite3::sqlite3_column_type>(
        base_addr + offset::k_sqlite3_column_type);
    sqlite3_column_blob = reinterpret_cast<sqlite3::sqlite3_column_blob>(
        base_addr + offset::k_sqlite3_column_blob);
    sqlite3_column_bytes = reinterpret_cast<sqlite3::sqlite3_column_bytes>(
        base_addr + offset::k_sqlite3_column_bytes);
    sqlite3_finalize = reinterpret_cast<sqlite3::sqlite3_finalize>(
        base_addr + offset::k_sqlite3_finalize);
  }
  sqlite3_exec sqlite3_exec;
  sqlite3_prepare sqlite3_prepare;
  sqlite3_open sqlite3_open;
  sqlite3_sleep sqlite3_sleep;
  sqlite3_errcode sqlite3_errcode;
  sqlite3_close sqlite3_close;
  sqlite3_step sqlite3_step;
  sqlite3_column_count sqlite3_column_count;
  sqlite3_column_name sqlite3_column_name;
  sqlite3_column_type sqlite3_column_type;
  sqlite3_column_blob sqlite3_column_blob;
  sqlite3_column_bytes sqlite3_column_bytes;
  sqlite3_finalize sqlite3_finalize;
};
}  // namespace sqlite3
#endif