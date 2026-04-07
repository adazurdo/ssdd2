CC = gcc
CFLAGS = -Wall -g -I./
LDFLAGS = -pthread

all: libclaves.so libproxyclaves.so servidor-sock app-cliente

common.o: common.c common.h
	$(CC) $(CFLAGS) -fPIC -c common.c

claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -fPIC -c claves.c

proxy-sock.o: proxy-sock.c claves.h common.h
	$(CC) $(CFLAGS) -fPIC -c proxy-sock.c

servidor-sock.o: servidor-sock.c claves.h common.h
	$(CC) $(CFLAGS) -c servidor-sock.c

app-cliente.o: app-cliente.c claves.h
	$(CC) $(CFLAGS) -c app-cliente.c

# Librerías compartidas
libclaves.so: claves.o
	$(CC) -shared -o libclaves.so claves.o

libproxyclaves.so: proxy-sock.o common.o
	$(CC) -shared -o libproxyclaves.so proxy-sock.o common.o

# Ejecutable servidor
servidor-sock: servidor-sock.o claves.o common.o
	$(CC) $(LDFLAGS) -o servidor-sock servidor-sock.o claves.o common.o

# Ejecutable cliente
app-cliente: app-cliente.o libproxyclaves.so
	$(CC) -o app-cliente app-cliente.o -L. -lproxyclaves

clean:
	rm -f *.o *.so servidor-sock app-cliente data.bin temp