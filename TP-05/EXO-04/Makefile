KERNELDIR_PNL ?= ~/PNL4/tmp

obj-m += prdebug.o

CFLAGS_prdebug.o := -DDEBUG

all :
	make -C $(KERNELDIR_PNL) M=$$PWD modules

clean:
	make -C $(KERNELDIR_PNL) M=$$PWD clean
