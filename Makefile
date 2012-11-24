NAME=glitcher
CXX=g++
CFLAGS=$(shell pkg-config --cflags opencv) -Wall
CXXFLAGS=$(CFLAGS)
LDFLAGS=
LIBS=$(shell pkg-config --libs opencv) -lstdc++
OBJ=$(NAME).o

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(LIBS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(NAME) $(OBJ)
