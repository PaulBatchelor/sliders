OBJ=monome.o griffin.o

CFLAGS= -fPIC -g

TARGET=sliders.so sliders

LIBS=-llo -lsporth -lsoundpipe -lsndfile -lm -lpthread -ldl

default: $(TARGET) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

sliders: $(OBJ) sliders.c
	$(CC) $(OBJ) $(CFLAGS) -DSTANDALONE sliders.c -o $@ $(LIBS)

sliders.so: $(OBJ) sliders.c
	$(CC) -shared $(OBJ) $(CFLAGS) $(LIBS) sliders.c -o $@ 

install: sliders.so
	mkdir -p /usr/local/share/sporth/plugins
	install sliders.so /usr/local/share/sporth/plugins

clean:
	rm -rf sliders sliders.so $(OBJ)

