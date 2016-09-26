import csv
import numpy as np

ptr = 0
history_size = 4
data = None

def read_csv(fname):
    global data

    f = open(fname, 'r')
    dataReader = csv.reader(f)
    rssi = []
    distance = []
    for c in dataReader:
        rssi.append([float(c[1])])
        distance.append([float(c[2])])
    data = [np.array(rssi),np.array(distance)]
    return data
            
def init(fname, _history_size):
    global ptr, history_size

    ptr = 0
    history_size = _history_size
    read_csv(fname)

def read_next():
    global ptr, data

    rssi = data[0]
    distance = data[1]

    rssi = np.array(rssi[ptr: ptr + history_size])
    distance = distance[ptr + history_size - 1]
    ptr += 1

    return rssi, distance
