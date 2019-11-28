CXX = g++
BIN = bin
LIB = lib
OBJS_MAIN = root/obj/dual.o root/obj/gradient.o root/obj/main.o

all : main

main : $(BIN)
	$(MAKE) -C root obj obj/dual.o obj/gradient.o obj/main.o
	$(CXX) -o $(BIN)/main $(OBJS_MAIN) $(LIBS)

$(BIN) :
	if [ ! -d $(BIN) ]; then mkdir $(BIN); fi

$(LIB) :
	if [ ! -d $(LIB) ]; then mkdir $(LIB); fi

clean :
	$(MAKE) -C root clean
	if [ -d $(BIN) ]; then rm $(BIN) -r; fi

install : $(LIB)
	ar rcs $(LIB)/libgradient.a root/obj/dual.o root/obj/gradient.o
	sudo mv $(LIB)/libgradient.a /usr/local/lib/mylibs
	sudo ln -sf /usr/local/lib/mylibs/libgradient.a /usr/local/lib/libgradient.a
	sudo cp root/include/gradient.h /usr/local/include/mylibs

.PHONY : all
.PHONY : install
.PHONY : main
.PHONY : clean
