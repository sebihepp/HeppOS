
.PHONY: all clean

all:
	cd kloader && $(MAKE) all
	cd kernel && $(MAKE) all

clean:
	rm -f HeppOS.iso
	rm -f HeppOS.img
	cd kernel && $(MAKE) clean
	cd kloader && $(MAKE) clean
