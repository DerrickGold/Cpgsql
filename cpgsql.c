/*
 * An example using the Postgresql C API
 */
#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

//database host
#define HOST "localhost"
//user created with: createuser -s <username>
#define USER "derrick"
//database name created with: createdb <database name>
#define DB_NAME "testdatabase"


#define TABLE_NAME "Test"

#define MAKE_TABLE(table)                       \
  "CREATE TABLE "table"("                       \
  "id INTEGER PRIMARY KEY,"                     \
  "Name VARCHAR(20)"                            \
  ")"

#define INSERT_STMT(table, values)              \
  "INSERT INTO "table" VALUES("values")"

//Parameters used for connecting to db.
//Must be null terminated.
const char *connectKeys[] = {
  "user", "dbname", "host", NULL
};

//Values for the keys provided above, must be NULL terminated.
const char *connectVals[] = {
  USER, DB_NAME, HOST, NULL
};


//if any query errors out, cleanup and exit
void validate(PGconn *conn, PGresult *res) {
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    PQclear(res);
    PQfinish(conn);
    exit(1);
  }

  PQclear(res);
}

int main(int argc, char *argv[]) {

  //PGconn *conn = PQconnectdb("user=derrick dbname=testdatabase host=localhost");
  PGconn *conn = PQconnectdbParams(connectKeys, connectVals, 0);

  if (PQstatus(conn) == CONNECTION_BAD) {
    fprintf(stderr, "Conenction to database failed: %s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
  }

  
  int version = PQserverVersion(conn);
  printf("Server version: %d\n", version);

  //Make a table
  validate(conn, PQexec(conn, "DROP TABLE IF EXISTS "TABLE_NAME));
  validate(conn, PQexec(conn, MAKE_TABLE(TABLE_NAME)));

  //then insert some test values
  validate(conn, PQexec(conn, INSERT_STMT(TABLE_NAME, "1, 'Derrick'")));
  validate(conn, PQexec(conn, INSERT_STMT(TABLE_NAME, "2, 'Bob'")));
  validate(conn, PQexec(conn, INSERT_STMT(TABLE_NAME, "3, 'Jake'")));
  validate(conn, PQexec(conn, INSERT_STMT(TABLE_NAME, "4, 'Brynne'")));
  validate(conn, PQexec(conn, INSERT_STMT(TABLE_NAME, "5, 'Dylan'")));


  //now run a query
  PGresult *res = PQexec(conn,"SELECT * FROM "TABLE_NAME);
  if (PQresultStatus(res) != PGRES_TUPLES_OK)
    fprintf(stdout, "Query returned nothing\n");
  else {
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++)
      fprintf(stdout, "ID: %s, NAME: %s\n", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1));
    
  }
  
  PQclear(res);
  PQfinish(conn);
  return 0;
}
