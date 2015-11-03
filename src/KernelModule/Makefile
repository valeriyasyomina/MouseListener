obj-m := usbmouse.o 


KDIR  := /lib/modules/$(shell uname -r)/build


PWD   := $(shell pwd)


default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


clean:
	rm *.o	*.ko *.order *.symvers *.mod.c
