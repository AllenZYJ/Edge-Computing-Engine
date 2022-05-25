CXX = g++
BIN = bin
LIB = lib
LIB_NAME = autodiff
OBJS_GRADIENT_DESCENT = root/obj/node.o root/obj/graph.o examples/obj/gradient_descent.o
OBJS_ANN = root/obj/node.o root/obj/graph.o examples/obj/ann.o
OBJS_GRADIENT = root/obj/node.o root/obj/graph.o examples/obj/gradient.o
OBJS_SPEED = root/obj/node.o root/obj/graph.o examples/obj/speed.o
OBJS_SIMPLE = root/obj/node.o root/obj/graph.o examples/obj/simple.o

all : gradient_descent ann gradient speed simple

gradient_descent : $(BIN) root/include/vectmath.h root/include/mor.h root/include/dor.h root/include/por.h
	$(MAKE) -C examples obj obj/gradient_descent.o
	$(MAKE) -C root obj obj/node.o obj/graph.o
	$(CXX) -o $(BIN)/gradient_descent $(OBJS_GRADIENT_DESCENT) $(LIBS)

ann : $(BIN) root/include/vectmath.h root/include/mor.h root/include/dor.h root/include/por.h
	$(MAKE) -C examples obj obj/ann.o
	$(MAKE) -C root obj obj/node.o obj/graph.o
	$(CXX) -o $(BIN)/ann $(OBJS_ANN) $(LIBS)

gradient : $(BIN) root/include/vectmath.h root/include/mor.h root/include/dor.h root/include/por.h
	$(MAKE) -C examples obj obj/gradient.o
	$(MAKE) -C root obj obj/node.o obj/graph.o
	$(CXX) -o $(BIN)/gradient $(OBJS_GRADIENT) $(LIBS)

speed : $(BIN) root/include/vectmath.h root/include/mor.h root/include/dor.h root/include/por.h
	$(MAKE) -C examples obj obj/speed.o
	$(MAKE) -C root obj obj/node.o obj/graph.o
	$(CXX) -o $(BIN)/speed $(OBJS_SPEED) $(LIBS)

simple : $(BIN) root/include/mor.h root/include/dor.h root/include/por.h
	$(MAKE) -C examples obj obj/simple.o
	$(MAKE) -C root obj obj/node.o obj/graph.o
	$(CXX) -o $(BIN)/simple $(OBJS_SIMPLE) $(LIBS)

$(BIN) :
	if [ ! -d $(BIN) ]; then mkdir $(BIN); fi

$(LIB) :
	if [ ! -d $(LIB) ]; then mkdir $(LIB); fi

clean :
	$(MAKE) -C root clean
	$(MAKE) -C examples clean
	if [ -d $(BIN) ]; then rm $(BIN) -r; fi

install : $(LIB)
	ar rcs $(LIB)/lib$(LIB_NAME).a root/obj/graph.o root/obj/node.o
	if [ ! -d /usr/local/include/$(LIB_NAME) ]; then mkdir /usr/local/include/$(LIB_NAME); fi
	cp $(LIB)/lib$(LIB_NAME).a /usr/local/lib
	cp root/include/*.h /usr/local/include/$(LIB_NAME)

.PHONY : all
.PHONY : gradient_descent
.PHONY : ann
.PHONY : gradient
.PHONY : speed
.PHONY : simple
.PHONY : clean
.PHONY : install
