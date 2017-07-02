OBJ=monome.o griffin.o
.SUFFIXES: .pd_linux

CFLAGS= -fPIC -g

TARGET=sliders.so sliders sliders.pd_linux

LIBS=-llo -lsporth -lsoundpipe -lsndfile -lm -lpthread -ldl

default: $(TARGET) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

sliders: $(OBJ) sliders.c
	$(CC) $(OBJ) $(CFLAGS) -DSTANDALONE -DBUILD_MAIN sliders.c -o $@ $(LIBS)

sliders.so: $(OBJ) sliders.c
	$(CC) -shared $(OBJ) $(CFLAGS) -DBUILD_SPORTH $(LIBS) sliders.c -o $@ 

sliders.pd_linux: pd.c $(OBJ) 
	$(CC) $*.c $(CFLAGS) $(OBJ) -DSTANDALONE -DPD -O2 -shared $(LIBS) \
		-lc -lm -o $*.pd_linux
	#strip --strip-unneeded $*.pd_linux

install: sliders.so
	mkdir -p /usr/local/share/sporth/plugins
	install sliders.so /usr/local/share/sporth/plugins

clean:
	rm -rf sliders sliders.so $(OBJ) *.pd_linux

