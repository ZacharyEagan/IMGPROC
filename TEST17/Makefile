CC=g++
CFLAGS=-I

LIBS=-lopencv_core -lopencv_highgui -lopencv_imgproc -lv4l2 -lpthread


MIA: main.cpp Serial.o Settings.o Proc.o
	g++ main.cpp Serial.o Settings.o $(LIBS) -o MIA

Serial.o:
	$(CC) -c Serial.c
Settings.o:
	$(CC) -c Settings.cpp
Proc.o:
	$(CC) -c Proc.cpp $(LIBS)

clean:
	rm *.o
