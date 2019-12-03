all: one 
	@echo Done.

one:
	g++ -c -Wall -Wno-unknown-pragmas -Wno-format -O3   tinyxml/tinyxml.cpp -o tinyxml.o
	g++ -c -Wall -Wno-unknown-pragmas -Wno-format -O3   tinyxml/tinyxmlparser.cpp -o tinyxmlparser.o
	g++ -c -Wall -Wno-unknown-pragmas -Wno-format -O3   tinyxml/tinyxmlerror.cpp -o tinyxmlerror.o
	g++ -c -Wall -Wno-unknown-pragmas -Wno-format -O3   tinyxml/tinystr.cpp -o tinystr.o
	g++ -c Circle.cpp -o circle.o
	g++ -c Line.cpp -o line.o
	g++ -c Arena.cpp -o arena.o
	g++ -c Bala.cpp -o bala.o
	g++ -c imageloader.cpp -o imageloader.o
	g++ -o trabalhocg trab.cpp  tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o circle.o line.o arena.o bala.o imageloader.o -lGL -lGLU -lglut -lm
	@echo compile ready!
	@echo

run: one
	./trabalhocg ./
	@echo test completed!
	@echo 

clean:
	rm tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o circle.o line.o arena.o bala.o
	@echo clean completed!
	@echo