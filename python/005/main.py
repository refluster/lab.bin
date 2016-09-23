import tensorflow as tf
import numpy as np
from random import shuffle
import read_data
 
def main():
    num_hidden = 20
    num_history = 4
    batch_size = 3

    ## placeholders ############################
    data = tf.placeholder(tf.float32, [None, num_history, 1])
    target = tf.placeholder(tf.float32, [None, 1])

    print("data = {0}").format(data.get_shape())
    print("target = {0}").format(target.get_shape())

    ## prediction network model ############################
    cell = tf.nn.rnn_cell.LSTMCell(num_hidden, state_is_tuple=True)
    val, state = tf.nn.dynamic_rnn(cell, data, dtype=tf.float32)
    print("val = {0}").format(val.get_shape())

    val = tf.transpose(val, [1, 0, 2])
    print("val = {0}").format(val.get_shape())

    last = tf.gather(val, int(val.get_shape()[0]) - 1)
    print("last = {0}").format(last.get_shape())

    weight = tf.Variable(tf.truncated_normal([num_hidden, int(target.get_shape()[1])]))
    bias = tf.Variable(tf.constant(0.1, shape=[target.get_shape()[1]]))
    prediction = tf.nn.softmax(tf.matmul(last, weight) + bias)

    print("weight = {0}").format(weight.get_shape())
    print("bias = {0}").format(bias.get_shape())

    ## checking tensor size ############################
    _a = tf.matmul(last, weight)
    print("last*w = {0}").format(_a.get_shape())
    _b = _a + bias
    print("last*w + b = {0}").format(_b.get_shape())

    ## calculate error ############################
    mse = tf.reduce_mean(tf.square(target - prediction))

    ## error minimization ############################
    optimizer = tf.train.AdamOptimizer()
    minimize = optimizer.minimize(mse)

    ## initialization ############################
    init_op = tf.initialize_all_variables()
    sess = tf.Session()
    sess.run(init_op)

    read_data.init(num_history, batch_size)

    for i in range(181/batch_size - num_history):
        inp, out = read_data.read_next()
        sess.run(minimize,{data: inp, target: out})

        print(sess.run(mse,{data: inp, target: out}))

    sess.close()

main()
