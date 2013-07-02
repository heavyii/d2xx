TOPDIR  := $(shell cd ..; cd ..; pwd)
include $(TOPDIR)/Rules.make

APP = write 

all: $(APP) 

write: main.c	
	$(CC) main.c -o $@ $(CFLAGS)


clean:
	@rm -f *.o ; rm $(APP)
