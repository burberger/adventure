CPP_FILES := $(wildcard src/*.cpp)
HPP_FILES := $(wildcard src/*.hpp)
TEST_FILES := $(wildcard tests/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CXX_FLAGS := -g -Wall -std=c++11

main: main.cpp $(OBJ_FILES)
	g++ $(CXX_FLAGS) -o $@ $^ -lreadline

test: $(TEST_FILES) $(OBJ_FILES) $(HPP_FILES)
	g++ $(CXX_FLAGS) -o $@ $^ -lboost_unit_test_framework

obj/%.o: src/%.cpp
	g++ $(CXX_FLAGS) -c -o $@ $<

clean:
	rm -f obj/*.o
	rm -f test
