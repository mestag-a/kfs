##
## Makefile for Kernel in /home/mestag_a/tests/Kernel
## 
## Made by alexis mestag
## Login   <mestag_a@epitech.net>
## 
## Started on  Fri Oct  9 13:45:01 2015 alexis mestag
## Last update Fri Oct  9 16:55:17 2015 alexis mestag
##

NAME = kernel.elf

CFLAGS += -nostdinc
CFLAGS += -fno-builtin -m32
CFLAGS += -fno-stack-protector
CFLAGS += -g

LDFLAGS += -nostdlib
LDFLAGS += -m32 -Wl,--build-id=none

CSRC=	src/main.c
SSRC=	src/crt0.S
OBJ=	$(CSRC:.c=.o) $(SSRC:.S=.o)

%.o:	%.S
	$(CC) $(CFLAGS)   -c -o $@ $<

all:	$(NAME)

$(NAME):	$(OBJ)
		gcc -o $(NAME) $(OBJ) $(LDFLAGS)

QEMU_PARAMS =	-kernel $(NAME) -m 1G -monitor stdio

boot:	all
	qemu-system-i386 $(QEMU_PARAMS)

debug:	all
	qemu-system-i386 $(QEMU_PARAMS) -s -S 

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)
