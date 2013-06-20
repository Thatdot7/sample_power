import sqlite3

conn = sqlite3.connect('test.db')

# Forming row_counters table
#
# This will keep track of the entry it will next write to in the
# given table
conn.execute('''CREATE TABLE IF NOT EXISTS row_counters (
	table_name TEXT PRIMARY KEY,
	counter INTEGER);''')

conn.close()
