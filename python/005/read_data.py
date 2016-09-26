import csv
import numpy as np

ptr = 0
history_size = 4
batch_size = 3
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
            
def init(fname, _history_size, _batch_size):
    global ptr, history_size, batch_size

    ptr = 0
    history_size = _history_size
    batch_size = _batch_size
    read_csv(fname)

def read_next():
    global ptr, data

    rssi = data[0]
    distance = data[1]

    rssi_batch = []
    distance_batch = []
    for i in range(batch_size):
        rssi_batch.append(np.array(rssi[ptr + i: ptr + i + history_size]))
        distance_batch.append(distance[ptr + i + history_size - 1])

    ptr += batch_size

    return rssi_batch, distance_batch
