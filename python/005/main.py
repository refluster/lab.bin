import read_data
import tensorflow as tf
import numpy as np
from tensorflow.models.rnn import rnn, rnn_cell, seq2seq


def main():
    data = read_data.read()
    print(data)

main()
