CXX = g++
ODIR = obj
CXXFLAGS = -std=c++11 -O3
OBJS = $(ODIR)/gradient_descent.o $(ODIR)/ann.o $(ODIR)/gradient.o $(ODIR)/speed.o $(ODIR)/simple.o 

all : $(ODIR) $(OBJS) 

$(ODIR)/gradient_descent.o : src/gradient_descent.cpp ../root/include/vectmath.h ../root/include/node.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/ann.o : src/ann.cpp ../root/include/vectmath.h ../root/include/node.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/gradient.o : src/gradient.cpp ../root/include/vectmath.h ../root/include/node.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/speed.o : src/speed.cpp ../root/include/vectmath.h ../root/include/node.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/simple.o : src/simple.cpp ../root/include/node.h 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR) : 
	if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi

clean : 
	if [ -d $(ODIR) ]; then rm $(ODIR) -r; fi

.PHONY : all
.PHONY : clean

