run: build
	./client

build:
	gcc buffer.c client.c helpers.c parson.c requests.c commands.c -o client -Wall

clean:
	rm -f client