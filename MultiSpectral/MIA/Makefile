CXX=g++
CC=gcc

SRCDIR := src
BUILDIR := build
TARGET := bin/MIA

FILES:= main.cpp ArrayComs.c CamSettings.cpp Proc.cpp Constants.c Thread.cpp
#FILES:= main.cpp  CamSettings.cpp Proc.cpp
SOURCES:= $(FILES:%.cpp=$(SRCDIR)/%.cpp)
SOURCES:= $(SOURCES:%.c=$(SRCDIR)/%.c)
OBJECTS:= $(SOURCES:$(SRCDIR)%=$(BUILDIR)%.o)

LIBS= -L lib 
LIBS+= -lopencv_core -lopencv_highgui -lopencv_imgproc 
LIBS+= -lv4l2 
LIBS+= -lpthread
INC := -I include

#CFLAGS= -Wall -Werror -O2
CFLAGS= -O2

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS)  $^ -o $(TARGET) $(LIBS)

$(BUILDIR)/%.o: $(SRCDIR)/%
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm build/*.o
	rm bin/MIA
