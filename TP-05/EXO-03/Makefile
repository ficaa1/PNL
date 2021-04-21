KERNELDIR_PNL ?= ~/PNL4/tmp

obj-m += hanging.o

all :
	make -C $(KERNELDIR_PNL) M=$$PWD modules

clean:
	make -C $(KERNELDIR_PNL) M=$$PWD clean
