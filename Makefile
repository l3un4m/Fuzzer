all:
	gcc -o fuzzer src/*.c
	./fuzzer ./extractor_x86_64

clean:
	rm fuzzer *.tar
