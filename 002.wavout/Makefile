OPT = -L/usr/local/Cellar/libsndfile/1.0.25/lib/ \
	-I/usr/local/Cellar/libsndfile/1.0.25/include/ \
	-I/usr/local/Cellar/portaudio/19.20140130/include/ \
	-L/usr/local/Cellar/portaudio/19.20140130/lib/

all:
	g++ -lportaudio -lsndfile $(OPT) wavReader.cc portaudio.cc combFilter.cc allPassFilter.cc

combFilter:
	g++ combFilter.cc -DTEST -o combFilter

allPassFilter:
	g++ allPassFilter.cc -DTEST -o allPassFilter


