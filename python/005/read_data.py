import csv
import numpy as np

def read_csv():
    data = []
    for fname in ['data.csv', 'data2.csv']:
        f = open(fname, 'r')
        dataReader = csv.reader(f)
        d = []
        for c in dataReader:
            rssi = float(c[1])
            d.append([rssi])
        data.append(np.array(d))
    return data
            
def read():
    data = read_csv()
    return data
