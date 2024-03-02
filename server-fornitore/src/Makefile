

#IDIR=../hdr
IDIR=.,../../shared-server
ODIR=../obj
BDIR=../bin


CC=g++
# use this when connecting to Postgresql (library lpq)
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm 

CFLAGS=-std=c++20  -g -ggdb -fpermissive -L/usr/local/lib -Wall -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm -lhiredis

#CFLAGS=-std=c++20 -g -ggdb -fpermissive -L/usr/local/lib -Wall -I. -I../con2redis/src -I../con2db -I/usr/include/postgresql -L../con2redis/bin -L../con2db/bin -lpq -lm -lhiredis


# use this when no connection to Postgresql (library lpq) is needed
#CFLAGS=-std=c++11 -I. -I$(IDIR) -lm 

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

con2db_OBJ=$(wildcard /home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/*.o)
con2redis_OBJ=$(wildcard /home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2redis/obj/*.o)


all:	con2redis $(BDIR)/main con2db $(BDIR)/main

con2redis:
	$(MAKE) -C /home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2redis/src

con2db:
	$(MAKE) -C /home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(con2db_OBJ) $(con2redis_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core 
