.PHONY: run
all : ./thirdpart/hl.g ./src/main.cpp
	java -jar ./thirdpart/antlr-3.4-complete.jar ./thirdpart/hl.g 
	rm -rf ./haizei_gramma/*
	mv ./thirdpart/*.h ./thirdpart/*.c ./haizei_gramma/
	mv ./hl.tokens ./haizei_gramma/
	g++ --std=c++11 -L./lib -I./include -I./include/antlr -I./haizei_gramma/ -DDEBUG ./src/main.cpp ./src/*.cc ./haizei_gramma/*.c -lantlr3c -o ./bin/Hython

run:
	./bin/Hython ./test_data/input1.hl
	./bin/Hython ./test_data/input2.hl
	./bin/Hython ./test_data/input3.hl
	./bin/Hython ./test_data/input4.hl
	./bin/Hython ./test_data/input5.hl
	./bin/Hython ./test_data/input6.hl
	./bin/Hython ./test_data/input7.hl
