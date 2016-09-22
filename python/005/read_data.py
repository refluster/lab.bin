import csv
import numpy as np

def read_csv():
    data = []
#    for fname in ['data.csv', 'data2.csv']:
    for fname in ['data.csv']:
        f = open(fname, 'r')
        dataReader = csv.reader(f)
        rssi = []
        distance = []
        for c in dataReader:
            rssi.append([float(c[1])])
            distance.append([float(c[2])])
        data.append([np.array(rssi),np.array(distance)])
    return data
            
def read():
    data = read_csv()
    return data
