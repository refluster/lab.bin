import csv
import numpy as np

def read_csv():
    f = open('data.csv', 'r')
    dataReader = csv.reader(f)
    
    data = []
    for d in dataReader:
        rssi = float(d[1])
        data.append([rssi])
    
    return data
            
def read():
    data = read_csv()
    return data
