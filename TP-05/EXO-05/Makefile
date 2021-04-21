KERNELDIR_PNL ?= ~/PNL4/tmp

obj-m += kcpustat.o

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR_PNL) M=$$PWD modules

clean:
	make -C $(KERNELDIR_PNL) M=$$PWD clean
