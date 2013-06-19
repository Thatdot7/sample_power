import sqlite3
import sys
from crontab import CronTab

def setup():
        cron = CronTab()
	cron.remove_all("")
	cron.write()
        job = cron.new(command='python /home/pi/sample_power/database_command.py hourly')
        job.minute.on(0)
        job = cron.new(command='python /home/pi/sample_power/database_command.py daily')
        job.hour.on(0)
        job.minute.on(0)
        job = cron.new(command='python /home/pi/sample_power/database_command.py monthly')
        job.dom.on(1)
        job.hour.on(0)
        job.minute.on(0)
        cron.write()
        
	conn = sqlite3.connect('test.db')

	conn.execute("PRAGMA journal_mode = WAL;")

	# Forming row_counters table
	#
	# This will keep track of the entry it will next write to in the
	# given table
	print "Creating and filling table 'row_counters'"
	conn.execute("DROP TABLE IF EXISTS row_counters;")
	conn.execute("CREATE TABLE IF NOT EXISTS row_counters ( \
		table_name TEXT PRIMARY KEY,\
		counter INTEGER);")

	conn.execute("INSERT INTO row_counters VALUES ('real_time_record', 1)")
	conn.execute("INSERT INTO row_counters VALUES ('last_24_hours', 1)")
	conn.execute("INSERT INTO row_counters VALUES ('last_31_days', 1)")

	# Forming real time record
	#
	# This will keep track of energy used and real power every second
	print "Creating 'real_time_record' and filling it with 10000 dummy rows"
	conn.execute("DROP TABLE IF EXISTS real_time_record;")
	conn.execute("CREATE TABLE real_time_record (\
		rowID INTEGER,\
		time TIMESTAMP,\
		real_power REAL,\
		energy REAL);")

	for i in range(10000):
		conn.execute("INSERT INTO real_time_record VALUES (\
			" + str(i+1) + ", datetime(CURRENT_TIMESTAMP),\
                         0, 0 );")

	# Forming record of last 24 hours
	#
	# This will keep track of energy used in last 24 hours
	# and updates in hourly blocks
	print "Creating 'last_24_hours' and filling it with 24 dummy rows"
	conn.execute("DROP TABLE IF EXISTS last_24_hours;")
	conn.execute("CREATE TABLE IF NOT EXISTS last_24_hours (\
		rowID INTEGER,\
		time TIMESTAMP,\
		energy REAL);")

	for i in range(24):
		conn.execute("INSERT INTO last_24_hours VALUES (\
			" + str(i+1) + ", datetime(CURRENT_TIMESTAMP),\
                        0);")


        # Forming record of last 31 days
	#
	# This will keep track of energy used in the last 31 days
	# and updates in daily blocks
	print "Creating 'last_31_days' and filling it with 31 dummy rows"
	conn.execute("DROP TABLE IF EXISTS last_31_days;")
	conn.execute("CREATE TABLE IF NOT EXISTS last_31_days (\
		rowID INTEGER,\
		time TIMESTAMP,\
		energy REAL);")

	for i in range(31):
		conn.execute("INSERT INTO last_31_days VALUES (\
			" + str(i+1) + ", datetime(CURRENT_TIMESTAMP\
                        ), 0);")

        # Monthly record of energy usage
        #
        # This record will be continuously growning
        conn.execute("DROP TABLE IF EXISTS monthly_record;")
        conn.execute("CREATE TABLE IF NOT EXISTS monthly_record (\
                time TIMESTAMP,\
                energy REAL);")
        
	conn.commit()
	conn.close()

def hourly():

        conn = sqlite3.connect('test.db')

        cur = conn.cursor()
        cur.execute("SELECT counter FROM row_counters WHERE (\
                table_name = 'last_24_hours');")

        counter = cur.fetchone()[0]

        cur.execute("SELECT SUM(energy) FROM real_time_record WHERE (time > datetime(CURRENT_TIMESTAMP, '-1 hour'));")

        energy_total = cur.fetchone()[0]

        conn.execute("UPDATE last_24_hours SET energy = " + str(energy_total) + ", \
                time = datetime(CURRENT_TIMESTAMP) \
                WHERE (rowID = " + str(counter) + ");")
        
        if counter >= 24:
                conn.execute("UPDATE row_counters SET counter = 1 \
                        WHERE (table_name = 'last_24_hours');")
        else:
                conn.execute("UPDATE row_counters SET counter = " + str(counter + 1) + " \
                        WHERE (table_name = 'last_24_hours');")
        
        conn.commit()
        conn.close()

def daily():

        conn = sqlite3.connect('test.db')

        cur = conn.cursor()
        cur.execute("SELECT counter FROM row_counters WHERE (\
                table_name = 'last_31_days');")

        counter = cur.fetchone()[0]

        cur.execute("SELECT SUM(energy) FROM last_24_hours;")

        energy_total = cur.fetchone()[0]

        conn.execute("UPDATE last_31_days SET energy = " + str(energy_total) + ", \
                time = datetime(CURRENT_TIMESTAMP) \
                WHERE (rowID = " + str(counter) + ");")
        
        if counter >= 31:
                conn.execute("UPDATE row_counters SET counter = 1 \
                        WHERE (table_name = 'last_31_days');")
        else:
                conn.execute("UPDATE row_counters SET counter = " + str(counter + 1) + " \
                        WHERE (table_name = 'last_31_days');")
        
        conn.commit()
        conn.close()

def monthly():
        conn = sqlite3.connect('test.db')
        cur = conn.cursor()
        cur.execute("SELECT SUM(energy) FROM last_31_days WHERE (time > datetime(CURRENT_TIMESTAMP, '-1 month'));")

        energy_total = cur.fetchone()[0]

        conn.execute("INSERT INTO montly_record VALUES (datetime(CURRENT_TIMESTAMP);")
        conn.commit()
        conn.close()

if __name__ == "__main__":
	if sys.argv[1] == "setup":
		setup()
        elif sys.argv[1] == "hourly":
                hourly()
        elif sys.argv[1] == "daily":
                daily()
        elif sys.argv[1] == "monthly":
                monthly()
