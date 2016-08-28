import read_data
import tensorflow as tf
from tensorflow.models.rnn import rnn, rnn_cell, seq2seq

num_of_input_nodes = 1
num_of_hidden_nodes = 48
num_of_output_nodes = 1
length_of_sequences = 48
forget_bias = 0.8
learning_rate = 0.01
num_of_training_epochs = 300
size_of_mini_batch = 48
num_of_prediction_epochs = 1


def get_loss(output, superviser):
    with tf.name_scope('loss') as scope:
        loss = tf.reduce_mean(tf.square(output - superviser))
        tf.scalar_summary('loss', loss)
        return loss

'''
def create_model(x):
    rnn_size = 48
    rnn_cell = rnn_cell.BasicRNNCell(rnn_size)
    state = tf.zeros([batch_size, rnn_cell.state_size])
    x_split = tf.split(0, 365, x)
    output, state = seq2seq.rnn_decoder(x_split, state, rnn_cell)
'''

def inference(indata, istate):
    with tf.name_scope("inference") as scope:
        weight1_var = tf.Variable(tf.truncated_normal([num_of_input_nodes,
                                                       num_of_hidden_nodes],
                                                      stddev=0.1), name="weight1")
        weight2_var = tf.Variable(tf.truncated_normal([num_of_hidden_nodes,
                                                       num_of_output_nodes],
                                                      stddev=0.1), name="weight2")
        bias1_var   = tf.Variable(tf.truncated_normal([num_of_hidden_nodes],
                                                      stddev=0.1), name="bias1")
        bias2_var   = tf.Variable(tf.truncated_normal([num_of_output_nodes],
                                                      stddev=0.1), name="bias2")

        in1 = tf.transpose(indata, [1, 0, 2])
        in2 = tf.reshape(in1, [-1, num_of_input_nodes])
        in3 = tf.matmul(in2, weight1_var) + bias1_var
        in4 = tf.split(0, length_of_sequences, in3)

        cell = rnn_cell.BasicLSTMCell(num_of_hidden_nodes, forget_bias=forget_bias)
        rnn_output, states_op = rnn.rnn(cell, in4, initial_state=istate)
        output_op = tf.matmul(rnn_output[-1], weight2_var) + bias2_var

        w1_hist = tf.histogram_summary("weights1", weight1_var )
        w2_hist = tf.histogram_summary("weights2", weight2_var )
        b1_hist = tf.histogram_summary("biases1", bias1_var)
        b2_hist = tf.histogram_summary("biases2", bias2_var)
        output_hist = tf.histogram_summary("output",  output_op )
        results = [weight1_var, weight2_var, bias1_var,  bias2_var]
        return output_op, states_op, results

def calc_accuracy(output_op, prints=False):
    inputs, ts = make_prediction(num_of_prediction_epochs)
    pred_dict = {
        indata: inputs,
        supervisor: ts,
        istate: tf.zero([num_of_prediction_epochs, num_of_hidden_nodes * 2]),
    }
    output= sess.run([output_op], feed_dict=pred_dict)

    def print_result (p, q):
        print("output: %f, correct: %d" % (p , q))
    if prints:
        [print_result(p, q)  for p, q in zip(output[0], ts)]

        opt = abs(output - ts)[0]
        total = sum([1 if x[0] < 0.05 else 0 for x in opt])
    print("accuracy %f" % (total/float(len(ts))))
    return output

def training(loss):
    with tf.name_scope("training") as scope:
        train_step = optimizer.minimize(loss)
        return train_step

def main():
    data = read_data.read()
    #for d in data:
    #    print d
    #create_model(data)

    optimizer = tf.train.GradientDescentOptimizer(learning_rate=learning_rate)

    with tf.Graph().as_default():
        input_ph = tf.placeholder(tf.float32, [None, length_of_sequences, num_of_input_nodes],
                               name="input")
        supervisor_ph = tf.placeholder(tf.float32, [None, length_of_sequences,
                                                 num_of_output_nodes], name="superviser")
        istate_ph = tf.placeholder(tf.float32, [None, num_of_hidden_nodes * 2],
                                name="istate")

        output, states, datas = inference(input_ph, istate_ph)
        loss = get_loss(output, supervisor_ph)
        train_step = training(loss)

        summary = tf.merge_all_summaries()
        init = tf.initialize_all_variables()

        with tf.Session() as sess:
            saver = tf.train.Saver()
            summary_writer = tf.train.SummaryWriter('/tmp/tensorflow_log', graph=sess.graph)
            sess.run(init)

            for epoch in range(num_of_training_epochs - 1):
                inputs = data[epoch]
                supervisors = data[epoch + 1]
                train_dict = {
                    input_ph: inputs,
                    supervisor_ph: supervisors,
                    istate_ph: tf.zeros([size_of_mini_batch, num_of_hidden_nodes * 2])
                }
                sess.run(train_step, feed_dict=train_dict)

                if (epoch) % 100 == 0:
                    summary_str, train_loss = sess.run([summary, loss], feed_dict=train_dict)
                    print("train#%d, train loss: %e" % (epoch, train_loss))
                    summary_writer.add_summary(summary_str, epoch)
#                    if (epoch ) % 500 == 0:
#                        calc_accuracy(output)

main()

exit

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
