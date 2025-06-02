CC = g++
CXXFLAGS = -Werror -Wpedantic -Wall -std=c++17 -pthread -I/usr/include
LDFLAGS_TEST = -pthread /usr/local/lib/libgtest.a /usr/local/lib/libgtest_main.a -lpthread

SRCS := ascii85_decoder.cpp main.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := ascii85_decoder

TEST_SRC := unit_tests.cpp
TEST_OBJ := $(TEST_SRC:.cpp=.o)
TEST_TARGET := unit_tests

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(filter-out main.o, $(OBJS)) $(TEST_OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS_TEST)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o