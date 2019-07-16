#!/bin/bash
# given column-name
# found the tables which has such column from a given db
# pretty print csv: | column -t -s,
columns=(
    col1
    col2
)
db=db

if [ ! -f $db ]; then
    echo $db is not a valid database
    exit -1
fi

tables=$(sqlite3 $db ".tables" | sed "s|\([a-zA-Z_]*\) *\([a-zA-Z_]*\)|\1\n\2|" | sed "/^$/d")

# loop: each table of db
for table in ${tables[@]}; do
    # get schema of table
    schema=$(sqlite3 $db ".schema $table")
    # loop: each column of `columns`
    for column in ${columns[@]}; do
        # check
        echo $schema | grep -q $column
        # if found, then print
        if [ $? -eq 0 ]; then
            echo "Found,$table,$column"
        fi
    done
done