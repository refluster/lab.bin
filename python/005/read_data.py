import csv
import numpy as np

ptr = 0
history_size = 4
batch_size = 3
data = []

def read_csv():
#    for fname in ['data.csv', 'data2.csv']:
    for fname in ['data.csv']:
        f = open(fname, 'r')
        dataReader = csv.reader(f)
        rssi = []
        distance = []
        for c in dataReader:
            rssi.append([float(c[1])])
            distance.append(float(c[2]))
        data.append([np.array(rssi),np.array(distance)])
    return data
            
def read():
    data = read_csv()
    return data

def init(_history_size, _batch_size):
    global ptr, history_size, batch_size

    ptr = 0
    history_size = _history_size
    batch_size = _batch_size
    read()

def read_next():
    global ptr

    d = data[0] # fix to the first csv file
    rssi = d[0]
    distance = d[1]

    rssi_batch = []
    distance_batch = []
    for i in range(batch_size):
        rssi_batch.append(np.array(rssi[ptr + i: ptr + i + history_size]))
        distance_batch.append(distance[ptr + i + history_size - 1])

    ptr += batch_size

    return rssi_batch, distance_batch
