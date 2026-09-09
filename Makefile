all:
	g++ test_for_cash.cpp -o test
	./test
arc:
	g++ arc_hash.cpp -o test
	./test