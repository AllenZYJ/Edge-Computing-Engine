CXX = g++
ODIR = obj
CXXFLAGS = -std=c++11 -O3
OBJS = $(ODIR)/graph.o $(ODIR)/node.o 

all : $(ODIR) $(OBJS) 

$(ODIR)/graph.o : src/graph.cpp include/graph.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/node.o : src/node.cpp include/node.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR) : 
	if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi

clean : 
	if [ -d $(ODIR) ]; then rm $(ODIR) -r; fi

.PHONY : all
.PHONY : clean

