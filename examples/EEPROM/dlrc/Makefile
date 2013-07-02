TOPDIR  := $(shell cd ..; cd ..; pwd)
include $(TOPDIR)/Rules.make

APP = ft232h_eeprom ft232h_replug ft232bm_eeprom ft232bm_replug read

all: $(APP) 

ft232h_eeprom: ft232h_eeprom.c	
	$(CC) $^ -o $@ $(CFLAGS)
	@cp $@ ft232h-tool/
ft232h_replug: ft232h_replug.c	
	$(CC) $^ -o $@ $(CFLAGS)
	@cp $@ ft232h-tool/

ft232bm_eeprom: ft232bm_eeprom.c	
	$(CC) $^ -o $@ $(CFLAGS)
	@cp $@ ft232bm-tool/
ft232bm_replug: ft232bm_replug.c	
	$(CC) $^ -o $@ $(CFLAGS)
	@cp $@ ft232bm-tool/

read: read.c	
	$(CC) $^ -o $@ $(CFLAGS)
	@cp $@ ft232h-tool/
	
clean:
	@rm -f *.o ; rm $(APP)
