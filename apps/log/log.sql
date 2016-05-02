  
CREATE TABLE def (EventID INTEGER, EventName VARCHAR(20));
CREATE TABLE type_def (TypeID INTEGER, TypeName VARCHAR(20));
CREATE TABLE events (thread INTEGER,event INTEGER, obj1 INTEGER,obj2 INTEGER,obj3 INTEGER,obj4 INTEGER,obj5 INTEGER, o1_type INTEGER, o2_type INTEGER,o3_type INTEGER,o4_type INTEGER,o5_type INTEGER, time INTEGER,info TEXT);
.mode csv
.separator ,
.import n00_def.csv def
.import n00_typedef.csv type_def
.import n00_events.csv events
.print n00_events.csv imported, Record count:
select count(*) from events;
