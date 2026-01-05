
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iincludes -IAPHTTP

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = includes
APHTTP_DIR = APHTTP

TARGET = UTCook

APHTTP_SRCS = $(wildcard $(APHTTP_DIR)/server/*.cpp) \
              $(wildcard $(APHTTP_DIR)/utils/*.cpp)

SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(APHTTP_SRCS)

LDFLAGS =
ifeq ($(OS),Windows_NT)
	LDFLAGS += -lws2_32
endif

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

force_build: clean $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking final executable: $@"
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $< to $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up build artifacts..."
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean force_build
