import tensorflow as tf
from tensorflow.models.rnn import rnn, rnn_cell, seq2seq
import numpy as np

char_rdic = ['h', 'e', 'l', 'o'] # id -> char
char_dic = {w: i for i, w in enumerate(char_rdic)} # char -> id
sample = [char_dic[c] for c in "hello"] # to index
x_data = np.array([ [1, 0, 0, 0],
                    [0, 1, 0, 0],
                    [0, 0, 1, 0],
                    [0, 0, 1, 0]],
                    dtype='f')

char_vocab_size = len(char_dic)
rnn_size = 4
time_step_size = 4
batch_size = 1

rnn_cell = rnn_cell.BasicRNNCell(rnn_size)
state = tf.zeros([batch_size, rnn_cell.state_size])
X_split = tf.split(0, time_step_size, x_data)
outputs, state = seq2seq.rnn_decoder(X_split, state, rnn_cell)
print(state)
print(outputs)

logits = tf.reshape(tf.concat(1, outputs), [-1, rnn_size])
targets = tf.reshape(sample[1:], [-1])
weights = tf.ones([time_step_size * batch_size])

loss = seq2seq.sequence_loss_by_example([logits], [targets], [weights], rnn_size)
cost = tf.reduce_sum(loss) / batch_size
train_op = tf.train.RMSPropOptimizer(0.01, 0.9).minimize(cost)

with tf.Session() as sess:
    tf.initialize_all_variables().run()
    for i in range(100):
        sess.run(train_op)
        result = sess.run(tf.arg_max(logits, 1))
        print(result, [char_rdic[t] for t in result])
