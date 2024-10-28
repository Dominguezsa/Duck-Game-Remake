.PHONY: all test clean editor client common server build

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

compile-client:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug  -DTALLER_SERVER=OFF -DTALLER_CLIENT=ON $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)


run-tests: compile-debug
	./build/taller_tests

all: clean run-tests

clean:
	rm -Rf build-*/
