run: build
	./client

build:
	gcc buffer.c client.c helpers.c parson.c requests.c -o client -Wall -Werror

clean:
	rm -f client