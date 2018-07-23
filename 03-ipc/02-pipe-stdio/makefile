all: teste printargv

teste: teste.o pexec.o
	g++ -o teste teste.o pexec.o

clean:
	rm -f teste printargv *.o *~
	