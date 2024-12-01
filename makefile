build/render: build/intermediate/main.o build/intermediate/list.o build/intermediate/matrix.o build/intermediate/mesh.o build/intermediate/stringUtils.o build/intermediate/render.o build/intermediate/transform.o
	mkdir -p build
	gcc build/intermediate/main.o build/intermediate/list.o build/intermediate/matrix.o build/intermediate/mesh.o build/intermediate/stringUtils.o build/intermediate/render.o build/intermediate/transform.o -o build/render -Wall

build/intermediate/main.o: src/main.c
	mkdir -p build/intermediate
	gcc src/main.c -c -o build/intermediate/main.o -Wall

build/intermediate/list.o: src/list.c src/list.h
	mkdir -p build/intermediate
	gcc src/list.c -c -o build/intermediate/list.o -Wall

build/intermediate/matrix.o: src/matrix.c src/matrix.h
	mkdir -p build/intermediate
	gcc src/matrix.c -c -o build/intermediate/matrix.o -Wall

build/intermediate/stringUtils.o: src/stringUtils.c src/stringUtils.h
	mkdir -p build/intermediate
	gcc src/stringUtils.c -c -o build/intermediate/stringUtils.o -Wall

build/intermediate/mesh.o: src/mesh.c src/mesh.h
	mkdir -p build/intermediate
	gcc src/mesh.c -c -o build/intermediate/mesh.o -Wall

build/intermediate/render.o: src/render.c src/render.h
	mkdir -p build/intermediate
	gcc src/render.c -c -o build/intermediate/render.o -Wall

build/intermediate/transform.o: src/transform.c src/transform.h
	mkdir -p build/intermediate
	gcc src/transform.c -c -o build/intermediate/transform.o -Wall

clean:
	rm -rf build
