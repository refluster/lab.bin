import csv
import re
from datetime import date

data = []
def read_temperature_csv():
    f = open('temp_2012_short.csv', 'r')
    dataReader = csv.reader(f)
    re_date = re.compile('(\d+)/(\d+)/(\d+) (\d+):(\d+)')
    
    for i in xrange(11):
        dataReader.next()

    for d in dataReader:
        r = re_date.match(d[0]) # 2012/4/1 17:30
        year = int(r.group(1))
        month = int(r.group(2))
        day = int(r.group(3))
        hour = int(r.group(4))
        minute = int(r.group(5))
        weekday = date(year, month, day).weekday()
        clockIndex = hour*2 + (minute == 30)
        
        temperature = float(d[1])
        
        record = [year, month, day, weekday, clockIndex, temperature]
        data.append(record)
            
read_temperature_csv()

def read_light_aircon_14f_csv():
    f = open('pow_light_aircon_14f_short.csv', 'r')
    dataReader = csv.reader(f)
    
    for i in xrange(11):
        dataReader.next()

    for i, d in enumerate(dataReader):
        for c in range(1, 10):
            data[i].append(d[c])

read_light_aircon_14f_csv()
    
for r in data:
    print r

