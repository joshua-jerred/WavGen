test:
	g++ example.cpp wavgen.cpp
	./a.out

clean:
	rm a.out
	rm test.wav