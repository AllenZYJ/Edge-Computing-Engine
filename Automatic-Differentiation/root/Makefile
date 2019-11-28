CXX = g++
ODIR = obj
CXXFLAGS = -std=c++14
OBJS = $(ODIR)/gradient.o $(ODIR)/dual.o $(ODIR)/main.o 

all : $(ODIR) $(OBJS) 

$(ODIR)/gradient.o : src/gradient.cpp include/gradient.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/dual.o : src/dual.cpp include/dual.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/main.o : src/main.cpp include/dual.h include/gradient.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR) : 
	if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi

clean : 
	if [ -d $(ODIR) ]; then rm $(ODIR) -r; fi

.PHONY : all
.PHONY : clean

