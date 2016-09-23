import tensorflow as tf
import numpy as np
from random import shuffle
import read_data
 
#NUM_EXAMPLES = 10000
#test_input = train_input[NUM_EXAMPLES:]
#test_output = train_output[NUM_EXAMPLES:] #everything beyond 10,000
#
#train_input = train_input[:NUM_EXAMPLES]
#train_output = train_output[:NUM_EXAMPLES] #till 10,000

def main():
    num_hidden = 20
    num_history = 24

    ## read data from csv ############################
    readdata = read_data.read()

    ## placeholders ############################
    data = tf.placeholder(tf.float32, [None, num_history, 1])
    target = tf.placeholder(tf.float32, [None, 1])

    print("data = {0}").format(data.get_shape())
    print("target = {0}").format(target.get_shape())

    ## network model ############################
    cell = tf.nn.rnn_cell.LSTMCell(num_hidden, state_is_tuple=True)
    val, state = tf.nn.dynamic_rnn(cell, data, dtype=tf.float32)
#    print("state = {0}").format(state.get_shape())
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

    # for check
    _a = tf.matmul(last, weight)
    print("last*w = {0}").format(_a.get_shape())
    _b = _a + bias
    print("last*w + b = {0}").format(_b.get_shape())

    mse = tf.reduce_mean(tf.square(target - prediction))

    optimizer = tf.train.AdamOptimizer()
    minimize = optimizer.minimize(mse)

    init_op = tf.initialize_all_variables()
    sess = tf.Session()
    sess.run(init_op)

    batch_size = 1000
    no_of_batches = int(len(train_input)/batch_size)
    epoch = 5000

    read_data.init(4, 3)

    ptr = num_history
    #for i in range(no_of_batches):
    for i in range(1):
        #inp, out = train_input[ptr:ptr+batch_size], train_output[ptr:ptr+batch_size]
        inp, out = read_data.read_next()
        ptr += batch_size
        sess.run(minimize,{data: inp, target: out})
#    incorrect = sess.run(error,{data: test_input, target: test_output})
#    print('Epoch {:2d} error {:3.1f}%'.format(i + 1, 100 * incorrect))
    sess.close()

main()

def test():
    read_data.init(4, 3)
    a, b = read_data.read_next()
    print(a)
    print(b)

#test()
