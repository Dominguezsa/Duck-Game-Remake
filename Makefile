# Definir la variable BUILD_TYPE para elegir entre Debug o Release
BUILD_TYPE ?= Debug

# Otras opciones, como flags adicionales
EXTRA_GENERATE ?=
EXTRA_COMPILE ?=

# Variables de compilación
CMAKE_ARGS := -S . -B ./build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

# Reglas para compilar dependiendo del BUILD_TYPE

compile:
	mkdir -p build/
	cmake $(CMAKE_ARGS) $(EXTRA_GENERATE)
	cmake --build build/ $(EXTRA_COMPILE)

# Reglas específicas para el cliente y el servidor
compile-server:
	mkdir -p build/
	cmake $(CMAKE_ARGS) -DTALLER_SERVER=ON $(EXTRA_GENERATE)
	cmake --build build/ $(EXTRA_COMPILE)

compile-client:
	mkdir -p build/
	cmake $(CMAKE_ARGS) -DTALLER_SERVER=OFF -DTALLER_CLIENT=ON $(EXTRA_GENERATE)
	cmake --build build/ $(EXTRA_COMPILE)

# Regla para ejecutar pruebas en modo debug
run-tests: compile
	./build/taller_tests

# Regla por defecto
all: clean run-tests

# Regla para limpiar la compilación
clean:
	rm -Rf build/
