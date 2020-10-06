TARGET = yuv420to444

C = gcc
CPP = g++


CFLAGS = -O0 -g -Wall -std=c++11 -dGNU
CFLAGS += \
	-I/usr/include

LDFLAGS = -lpthread

SRCS = yuv420to444.c \

all:
	$(C) -v -o $(TARGET) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o *~ $(TARGET)
