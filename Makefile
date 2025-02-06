SRC_DIR = src
BUILD_DIR = build
BUILD_FILE = system_manager
INSTALL_DIR = bin
DIST_DIR = dist
DOC_DIR = docs
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES = $(OBJ_FILES:.o=.d)
CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -MMD -MP
LDFLAGS =

.PHONY: all build run install dist docs clean distclean help 

all: build

build: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(BUILD_DIR)/$(BUILD_FILE) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP_FILES)

run: build
	$(BUILD_DIR)/$(BUILD_FILE)

install: build
	@if not exist $(INSTALL_DIR) mkdir $(INSTALL_DIR)
	@copy /Y $(BUILD_DIR)\$(BUILD_FILE).exe $(INSTALL_DIR)\$(BUILD_FILE).exe > nul

dist: build
	@if not exist $(DIST_DIR) mkdir $(DIST_DIR)
	@copy /y $(BUILD_DIR)\$(BUILD_FILE).exe $(DIST_DIR)\$(BUILD_FILE).exe > nul
	@tar -czvf $(DIST_DIR)/$(BUILD_FILE).tar.gz -C $(DIST_DIR) . > nul
	@del /q $(DIST_DIR)\$(BUILD_FILE).exe

docs: 
	@doxygen Doxyfile

clean:
	@if exist $(BUILD_DIR)\nul for %%f in ($(BUILD_DIR)\*.o) do del /q %%f
	@if exist $(BUILD_DIR)\nul for %%f in ($(BUILD_DIR)\*.d) do del /q %%f

distclean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR) > nul
	@if exist $(DOC_DIR) rmdir /s /q $(DOC_DIR) > nul
	@if exist $(INSTALL_DIR) rmdir /s /q $(INSTALL_DIR) > nul
	@if exist $(DIST_DIR) rmdir /s /q $(DIST_DIR) > nul

echo:
	@echo "Echo"

help:
	@echo Usage:
	@echo   make            - build the project
	@echo   make run        - build and run the project
	@echo   make install    - install the compiled project
	@echo   make dist       - create distribution package
	@echo   make docs       - generate documentation
	@echo   make clean      - clean temporary files
	@echo   make distclean  - clean all files and reset project
	@echo   make help       - show this help message

.DEFAULT:
	@echo Usage:
	@echo   make            - build the project
	@echo   make run        - build and run the project
	@echo   make install    - install the compiled project
	@echo   make dist       - create distribution package
	@echo   make docs       - generate documentation
	@echo   make clean      - clean temporary files
	@echo   make distclean  - clean all files and reset project
	@echo   make help       - show this help message