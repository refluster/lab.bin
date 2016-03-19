import tensorflow as tf

def x2_plus_b(x, b):
    _x = tf.constant(x)
    _b = tf.constant(b)
    result = tf.square(_x)
    result = tf.add(result, _b)
    return result

def monitor_calculation(x, b):
    title = "b = {0}".format(b)
    c = x2_plus_b(float(x), float(b))
    s = tf.scalar_summary(title, c)
    m = tf.merge_summary([s])  # if you are using some summaries, merge them
    return m

with tf.Session() as sess:
    writer = tf.train.SummaryWriter("log", graph_def=sess.graph_def)    
    xaxis = range(-10, 12)

    for b in range(3):
        for x in xaxis:
            summary_str = sess.run(monitor_calculation(x, b))
            writer.add_summary(summary_str, x)

            
