run: build
	./client

build:
	gcc -lm buffer.c client.c commands.c helpers.c parson.c requests.c -o client -Wall

clean:
	rm -f client