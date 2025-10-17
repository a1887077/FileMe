default: build/standalone

# build the application
build/standalone: standalone/source/* include/fileme/* source/*
	cmake -S standalone -B build/standalone
	cmake --build build/standalone -j4 --config Release

# build the unit testing suite
build/test: test/source/* include/fileme/* source/*
	cmake -S test -B build/test
	cmake --build build/test -j4

# build all targets
all: standalone/source/* source/* test/source/* include/fileme/*
	cmake -S all -B build
	cmake --build build -j4

# assorted formatting targets
format:
	cmake -S test -B build/test

format-changes: format
	cmake --build build/test --target format

format-fix: format
	cmake --build build/test --target fix-format

# run the application
run: build/standalone
	./build/standalone/FileMe

# run the unit tests
test: build/test
	CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# clean the build directory
clean:
	rm -rf build
