OBJ=sliders.o monome.o griffin.o

#CFLAGS= -DSTANDALONE
CFLAGS= -fPIC  -g

TARGET=sliders.so

LIBS=-llo -lsporth -lsoundpipe -lsndfile -lm -lpthread 

default: $(TARGET) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

sliders: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LIBS) -o $@ 

sliders.so: $(OBJ)
	$(CC) -shared $(OBJ) $(CFLAGS) $(LIBS)  -o $@ 


clean:
	rm -rf sliders sliders.so $(OBJ)

