import tensorflow as tf
import numpy as np
import math
import read_data
 
def main():
    num_hidden = 20
    num_history = 12
    batch_size = 3

    ## placeholders ############################
    data = tf.placeholder(tf.float32, shape=(num_history, 1))
    target = tf.placeholder(tf.float32, shape=(1))

    print("data = {0}").format(data.get_shape())
    print("target = {0}").format(target.get_shape())

    ## prediction network model ############################
    cell = tf.nn.rnn_cell.LSTMCell(num_hidden, state_is_tuple=True)
    val, state = tf.nn.rnn(cell, [data], dtype=tf.float32)
    print("val = {0}").format(val[0].get_shape())

    val = tf.transpose(val, [1, 0, 2])
    print("val = {0}").format(val.get_shape())

    last = tf.gather(val, int(val.get_shape()[0]) - 1)
    print("last = {0}").format(last.get_shape())

    weight = tf.Variable(tf.truncated_normal([num_hidden, int(target.get_shape()[0])]))
    bias = tf.Variable(tf.constant(0.1, shape=[target.get_shape()[0]]))
    prediction = tf.matmul(last, weight) + bias

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

    for n in range(50):
        read_data.init('data.csv', num_history)
        for i in range(181/batch_size - num_history):
            for j in range(batch_size):
                inp, out = read_data.read_next()
                sess.run(minimize,{data: inp, target: out})
            print(sess.run(prediction,{data: inp, target: out}))
        print("==============================")

    sess.close()

main()
