#! /bin/bash

sql_file=log.sql
cd log
rm -f utp

sql(){
    echo $@ >> $sql_file
}

for node in $(seq 0 10)
do 
    if [ $node -lt 10 ]; then 
	n="0$node" 
    else
	n=$node
    fi
    if [ -f n${n}_events.csv ] ; then 
	echo "  " > $sql_file
	sql "CREATE TABLE def (EventID INTEGER, EventName VARCHAR(20));" 
	sql "CREATE TABLE type_def (TypeID INTEGER, TypeName VARCHAR(20));"

	s1="CREATE TABLE events (thread INTEGER,event INTEGER,"
	s2="                     obj1 INTEGER,obj2 INTEGER,obj3 INTEGER,obj4 INTEGER,obj5 INTEGER,"
	s3="                     o1_type INTEGER, o2_type INTEGER,o3_type INTEGER,o4_type INTEGER,o5_type INTEGER, "
	s4="                     time INTEGER,info TEXT);"

	sql $s1$s2$s3$s4
	sql .mode csv >> $sql_file
	sql .separator "," >> $sql_file
	sql .import n00_def.csv def >> $sql_file
	sql .import n00_typedef.csv type_def >> $sql_file
	sql .import n${n}_events.csv events 
	sql .print "n${n}_events.csv imported, Record count:"
	sql "select count(*) from events;"
	db=utp_n$n
	cat $sql_file | sqlite3 $db
	
    fi
done 




cd ..
