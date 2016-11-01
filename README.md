# Cpgsql
A simple example using the Postgresql C API

## Creating a database

Assuming Postgresql is installed on the system, run (replacing the text in <> with your own values):

    initdb <database name> -E utf8
    postgres -D <database name>
    createuser -s <your user>
    createdb <database name>

Once the database is made, update the conenction details in cpgsql.c to match the values provided in the database setup above.

## Building

Type `make` from the project root directory. `libpq` will need to be installed.