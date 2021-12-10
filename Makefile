OBJS:=src/main.c src/io_handler.c src/converter.c

bin/main: ./bin $(OBJS)
	gcc $(OBJS) -o bin/main

./bin:
	mkdir bin

run: ./bin/main
	./bin/main

clean:
	rm -f ./bin/*