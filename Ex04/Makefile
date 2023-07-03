static_lib: div.c mult.c sub.c sum.c
	gcc -c div.c mult.c sub.c sum.c
	ar cr libfunc.a *.o

static_run: static_lib main.c libfunc.a
	gcc main.c -L. -lfunc

dynamic_lib: div.c mult.c sub.c sum.c
	gcc -c -fPIC div.c mult.c sub.c sum.c
	gcc -shared -o libfunc.so *.o

dynamic_run: dynamic_lib main.c libfunc.so
	gcc main.c -L. -lfunc

late_binding: dynamic_lib bind_main.c
	gcc bind_main.c -ldl

clean:
	rm -rf *.o *.a *.out *.so