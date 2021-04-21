KERNELDIR_PNL ?= ~/PNL4/tmp

obj-m += taskmonitor.o

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR_PNL) M=$$PWD modules

clean:
	make -C $(KERNELDIR_PNL) M=$$PWD clean
	
	

hellotest: hellotest.o
	gcc $^ -o $@

%.o: %.c
	gcc -c $< -o $@
