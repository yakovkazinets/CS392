CC = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors

all: dl_demo my_dl.so my_dl2.so
dl_demo: dl_demo.c 
		$(CC) $(CFLAGS) -o dl_demo dl_demo.c -ldl
my_dl.so: my_dl.c 
		$(CC) $(CFLAGS) -shared -fPIC -o my_dl.so my_dl.c
		#fPIC is position independent code?
my_dl2.so: my_dl2.c 
		$(CC) $(CFLAGS) -shared -fPIC -o my_dl2.so my_dl2.c
clean:
		rm -f *.so dl_demo dl_demo.exe 
		#shared object we can link to dynamically whereas regular we cannot!
