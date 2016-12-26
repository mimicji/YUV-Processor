CC = g++ 
PROC = yuv_processor
OPTIMIZE = -O2
WARNING = -w
SETTINGS = -std=c++11 -msse2 -mmmx -mavx2
CFLAGS = $(OPTIMIZE) $(WARNING) $(SETTINGS)

OBJECTS = main.o alpha_blend.o image_overlay.o rgb.o yuv.o yuv2rgb.o rgb2yuv.o

all: $(PROC)
    
yuv_processor: $(OBJECTS)
	$(CC) -o $(PROC) $(OBJECTS) $(CFLAGS)

main.o: process.h
	$(CC) -c main.cpp $(CFLAGS)
    
alpha_blend.o: process.h
	$(CC) -c alpha_blend.cpp $(CFLAGS)
    
image_overlay.o: process.h
	$(CC) -c image_overlay.cpp $(CFLAGS)

rgb.o: bmp.h rgb.h
	$(CC) -c rgb.cpp $(CFLAGS)
    
yuv.o: yuv.h
	$(CC) -c yuv.cpp $(CFLAGS)
    
yuv2rgb.o: process.h
	$(CC) -c yuv2rgb.cpp $(CFLAGS)
    
rgb2yuv.o: process.h
	$(CC) -c rgb2yuv.cpp $(CFLAGS)    
    
.PHONY: clean

clean:
	rm -rf yuv_processor $(OBJECTS)

    
