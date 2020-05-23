NAME := mydev_gpio

obj-m := $(NAME).o
#ccflags-y := -DIS_DEV_MISC

VERSION := $(shell uname -r)
LOCAL := $(shell pwd)

all:
	make -C /lib/modules/$(VERSION)/build M=$(LOCAL) modules

lib:
	gcc -fPIC -c -o lib$(NAME).o lib$(NAME).c
	gcc -shared -Wl,-soname,lib$(NAME).so.1 -o lib$(NAME).so.1.0.0 lib$(NAME).o -lc
	ln -s lib$(NAME).so.1.0.0 lib$(NAME).so
	ln -s lib$(NAME).so.1.0.0 lib$(NAME).so.1
    
test:
	gcc -o test_$(NAME) test_$(NAME).c

libtest:
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)
	gcc -o test_lib$(NAME) test_lib$(NAME).c -L. -l$(NAME)


libtestclean:
	rm -f test_lib$(NAME)

testclean:
	rm -f test_$(NAME)

libclean:
	rm -f lib$(NAME).o
	rm -f lib$(NAME).so*

clean:
	rm -f $(NAME).o
	rm -f $(NAME).ko
	rm -f $(NAME).mod.*
	rm -f .$(NAME).ko.cmd
	rm -f .$(NAME).mod.o.cmd
	rm -f .$(NAME).o.cmd
	rm -fr .tmp_versions
	rm -f modules.order
	rm -f Module.symvers

