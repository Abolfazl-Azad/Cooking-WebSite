
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iincludes

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = includes

TARGET = UTCOOK

SRCS = $(wildcard $(SRC_DIR)/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

force_build: clean $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking final executable: $@"
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $< to $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up build artifacts..."
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean force_build