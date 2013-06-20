import sqlite3

conn = sqlite3.connect('test.db')

# Forming row_counters table
#
# This will keep track of the entry it will next write to in the
# given table
conn.execute("CREATE TABLE IF NOT EXISTS row_counters ( \
	table_name TEXT PRIMARY KEY,\
	counter INTEGER);")

conn.execute("INSERT INTO row_counters VALUES('hour_record', 20);")
conn.commit()

# Forming hour_counters table
#
# This will keep track of energy used in hourly blocks
conn.execute("CREATE TABLE IF NOT EXISTS hour_record (\
	rowID INTEGER,\
	time TIMESTAMP,\
	energy REAL);")

for i in range(60):
	conn.execute("INSERT INTO hour_record VALUES (\
		" + str(i+1) + ", NULL, NULL );")

conn.commit()
conn.close()
