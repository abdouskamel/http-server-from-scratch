SOURCES = $(wildcard bootstrap/*.c http_parser/*.c server/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

all: $(OBJECTS)
	cc main.c build/bootstrap/*.o build/http_parser/*.o build/server/*.o -o build/http_server

bootstrap/%.o: bootstrap/%.c
	mkdir -p build/bootstrap
	cc -c $< -o build/$@

http_parser/%.o: http_parser/%.c
	mkdir -p build/http_parser
	cc -c $< -o build/$@

server/%.o: server/%.c
	mkdir -p build/server
	cc -c $< -o build/$@

clean:
	rm -r build