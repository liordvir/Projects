CC=/usr/local/bin/gcc
CXX=/usr/local/bin/g++
OBJS = Arithmetic.o Auxiliaries.o Exceptions.o Gcalc.o Graph.o main.o PythonInterface.o
LIB_OBJS = PythonInterface.o Exceptions.o Graph.o
GRAPH_FILES = "file_test_1 not enough vertices.gc" "file_test_2 not enough edges.gc" "file_test_3 double vertex.gc" \
	"file_test_4 double edge.gc" "file_test_5 edge of missing vertex one.gc" \
	"file_test_6 edge of missing vertex two.gc" "file_test_7 self-loop.gc" "file_test_8 illegal vertex name one.gc" \
	"file_test_9 illegal vertex name two.gc" \
	"file_test_10 illegal edge name.gc" "Illegal comma ,.gc" "LongerThanSpecified.gc"
ALL_FILES = design.pdf Makefile test_in.txt test_out.txt Graph.h Graph.cpp Gcalc.h Gcalc.cpp Arithmetic.h \
	Arithmetic.cpp Auxiliaries.h Auxiliaries.cpp Exceptions.h Exceptions.cpp graph.i main.cpp \
	PythonInterface.h PythonInterface.cpp
EXEC = gcalc
FILES_TO_CLEAN = $(OBJS) $(EXEC) $(LIB_OBJS) libgraph.a gcalc.zip
DEBUG = -g
CFLAGS = -DNDEBUG --pedantic-errors -Wall -Werror -std=c++11

$(EXEC) : $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $@

libgraph.a: $(LIB_OBJS)
	ar -rs $@ $^

Arithmetic.o: Arithmetic.cpp Arithmetic.h Graph.h Exceptions.h Auxiliaries.h
	$(CXX) -fPIC -c $(CFLAGS) $*.cpp
Auxiliaries.o: Auxiliaries.cpp Auxiliaries.h Graph.h Exceptions.h
	$(CXX) -fPIC -c $(CFLAGS) $*.cpp
Exceptions.o: Exceptions.cpp Exceptions.h
	$(CXX) -fPIC -c $(CFLAGS) $*.cpp
Gcalc.o: Gcalc.cpp Gcalc.h Graph.h Auxiliaries.h Exceptions.h Arithmetic.h
	$(CXX) -fPIC -c $(CFLAGS) $*.cpp
Graph.o: Graph.cpp Graph.h Exceptions.h
	$(CXX) -fPIC -c $(CFLAGS) $*.cpp
main.o: main.cpp Graph.h Gcalc.h
	$(CXX) -fPIC -c $(CFLAGS) $*.cpp
PythonInterface.o: PythonInterface.cpp PythonInterface.h Graph.h Exceptions.h
	$(CXX) -fPIC -c $(CFLAGS) $*.cpp

tar : $(ALL_FILES)
	zip $(EXEC) $(ALL_FILES) $(GRAPH_FILES)

clean :
	rm -f $(FILES_TO_CLEAN)