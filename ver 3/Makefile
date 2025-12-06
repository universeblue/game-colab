CXX      = g++
CXXFLAGS = -std=c++2a -Wall -Wextra -Isrc

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR = src

# همه‌ی فایل‌های cpp داخل src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# فایل‌های شیء
OBJ = $(SRC:.cpp=.o)

# نام خروجی برنامه
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(SFML_LIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

