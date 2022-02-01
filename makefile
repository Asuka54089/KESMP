run:
	g++ -std=c++17 ./src/*.cpp -o run -lstdc++fs

pg:
	g++ -std=c++17 ./src/*.cpp -pg -o run_pg -lstdc++fs

clean:
	rm run
