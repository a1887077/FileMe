default: standalone

standalone: standalone/source/* include/fileme/*
	cmake -S standalone -B build/standalone
	cmake --build build/standalone -j4 --config Release


all: standalone/source/* source/* test/source/* include/fileme/*
	cmake -S all -B build
	cmake --build build -j4

test: test/source/* include/fileme/*
	cmake -S test -B build/test
	cmake --build build/test -j4
	CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

format:
	cmake -S test -B build/test

format-changes: format
	cmake --build build/test --target format

format-fix: format
	cmake --build build/test --target fix-format

run: standalone
	./build/standalone/FileMe

clean:
	rm -rf build
