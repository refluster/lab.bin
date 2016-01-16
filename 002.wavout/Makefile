OPT = -L/usr/local/Cellar/libsndfile/1.0.25/lib/ \
	-I/usr/local/Cellar/libsndfile/1.0.25/include/ \
	-I/usr/local/Cellar/portaudio/19.20140130/include/ \
	-L/usr/local/Cellar/portaudio/19.20140130/lib/

all:
	gcc -lportaudio -lsndfile sndfile.c portaudio.c $(OPT)

sndfile:
	gcc -lsndfile sndfile.c $(OPT)

