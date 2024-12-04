build:
	cmake --build build

builddir:
	cmake -S . -B build

clean:
	rm -fr build

.PHONY: clean build
