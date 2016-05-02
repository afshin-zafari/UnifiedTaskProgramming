import sqlite3,sys,os
if len(sys.argv) <2:
   print "usage: ", sys.argv[0] , "<db file name>"
   sys.exit()
dbname=sys.argv[1]
if not os.path.isfile(dbname):
   sys.exit()
db = sqlite3.connect(dbname)


data_def="CREATE TABLE IF NOT EXISTS data AS "\
   "SELECT * "\
   "FROM events "\
   "WHERE event IN (11,13,16,17)"
db.execute(data_def)

cores="CREATE TABLE IF NOT EXISTS events_%02d AS "\
   "SELECT * "\
   "FROM events "\
   "WHERE thread = ? "\
   "AND event NOT IN (11,13,16,17)"

for c in range(17):
   print cores %c
   db.execute(cores%c,(c,))


check="SELECT thread,COUNT(*) FROM events GROUP BY thread;"
rows=db.execute(check)
for r in rows:
   print r 

for c in range(17):
   view = "CREATE VIEW IF NOT EXISTS vw_dur_%02d AS "\
      "SELECT e1.thread,e1.obj1,e1.time AS start,e2.time AS finish, (e2.time - e1.time) dur "\
      "FROM   events_%02d e1 , events_%02d e2 "\
      "WHERE  e1.obj1  = e2.obj1 "\
      "AND    e1.event = 14 "\
      "AND    e2.event = 15 " %(c,c,c)
   db.execute(view)
   db.execute("CREATE TABLE IF NOT EXISTS idle_%02d ("\
              "thread INTEGER, "\
              "task   INTEGER, "\
              "start  INTEGER, "\
              "finish INTEGER, "\
              "dur    INTEGER, "\
              "idle   INTEGER, "\
              "slice  INTEGER)"%c)
   cursor = db.execute('SELECT * FROM vw_dur_%02d ORDER BY START'%c)
   rows=cursor.fetchall()
   for r in range(len(rows)-1):
      print rows[r]
      print rows[r+1]
      print rows[r+1][2]-rows[r][3]
      db.execute('INSERT INTO idle_%02d VALUES (?,?,?,?,?,?,?)'%c,(rows[r][0],rows[r][1],rows[r][2],rows[r][3],rows[r][4],rows[r+1][2]-rows[r][3],rows[r][2]//1000))


db.execute('CREATE TABLE IF NOT EXISTS busy ( core INTEGER, slice INTEGER, idle INTEGER, busy INTEGER, idle_percent NUMBER(2.2))')
for c in range(17):
   cmd='INSERT INTO busy SELECT %d,slice,SUM(idle),SUM(dur),SUM(idle)*1.0 / ( SUM(idle)+SUM(dur) )*100 FROM idle_%02d WHERE idle <10000 GROUP BY slice;'%(c,c)
   print cmd
   db.execute(cmd)

rows=db.execute('SELECT slice,SUM(idle),SUM(busy),AVG(idle_percent) FROM busy GROUP BY slice;')
db.commit()
rec=list()
import pylab as plt
for r in rows:
   print r
   rec.append(r)
x=[r[0] for r in rec if r[3] is not None ]
y=[(100-r[3]) for r in rec if r[3] is not None ]
plt.figure()
plt.plot(x,y)
plt.gca().set_ylim([0,100])
plt.savefig(dbname+'_idle.pdf')


db.close()
