build/render: build/intermediate/main.o build/intermediate/list.o build/intermediate/matrix.o build/intermediate/mesh.o build/intermediate/stringUtils.o build/intermediate/render.o build/intermediate/transform.o build/intermediate/camera.o build/intermediate/debug.o
	mkdir -p build
	gcc build/intermediate/main.o build/intermediate/list.o build/intermediate/matrix.o build/intermediate/mesh.o build/intermediate/stringUtils.o build/intermediate/render.o build/intermediate/transform.o build/intermediate/camera.o build/intermediate/debug.o -pedantic -ansi -lm -g -o build/render -Wall

build/intermediate/main.o: src/main.c
	mkdir -p build/intermediate
	gcc src/main.c -c -g -o build/intermediate/main.o -Wall

build/intermediate/list.o: src/list.c src/list.h
	mkdir -p build/intermediate
	gcc src/list.c -c -g -o build/intermediate/list.o -Wall

build/intermediate/matrix.o: src/matrix.c src/matrix.h
	mkdir -p build/intermediate
	gcc src/matrix.c -c -g -o build/intermediate/matrix.o -Wall

build/intermediate/stringUtils.o: src/stringUtils.c src/stringUtils.h
	mkdir -p build/intermediate
	gcc src/stringUtils.c -c -g -o build/intermediate/stringUtils.o -Wall

build/intermediate/mesh.o: src/mesh.c src/mesh.h
	mkdir -p build/intermediate
	gcc src/mesh.c -c -g -o build/intermediate/mesh.o -Wall

build/intermediate/render.o: src/render.c src/render.h
	mkdir -p build/intermediate
	gcc src/render.c -c -g -o build/intermediate/render.o -Wall

build/intermediate/transform.o: src/transform.c src/transform.h
	mkdir -p build/intermediate
	gcc src/transform.c -c -g -o build/intermediate/transform.o -Wall

build/intermediate/camera.o: src/camera.c src/camera.h
	mkdir -p build/intermediate
	gcc src/camera.c -c -g -o build/intermediate/camera.o -Wall

build/intermediate/debug.o: src/debug.c src/debug.h
	mkdir -p build/intermediate
	gcc src/debug.c -c -g -o build/intermediate/debug.o -Wall

clean:
	rm -rf build
