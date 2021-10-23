# ProxyServer
# 2021

PROJECT = ProxyServer

# Compiler and Language STD
CXX	= @g++
STD	= -std=c++11

# Source directories
SRC_DIR := ./src
BIN_DIR := ./bin
BLD_DIR := ./build
DOC_DIR := ./doc/doxy

# Libraries
LIBS :=
LIB_DIR := /usr/lib

# Headers
INCLUDE := $(SRC_DIR)

# Compiler flags
WARN := -Wall
OPTI := -O0
LINK :=
OTHR := -pthread

# Join all flags
CXXFLAGS := $(STD) $(WARN) $(OPTI) $(LINK) $(OTHR) -I$(INCLUDE) -L$(LIB_DIR) $(LIBS)

# Objects
OBJS = $(BLD_DIR)/Server.o $(BLD_DIR)/Client.o
DEPS = $(SRC_DIR)/Connection.hpp $(SRC_DIR)/Exception.hpp

main: pre $(BIN_DIR)/$(PROJECT)

pre:
	@mkdir -p $(BLD_DIR) $(BIN_DIR)

# Main compilation
$(BIN_DIR)/$(PROJECT): $(SRC_DIR)/$(PROJECT).cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BLD_DIR)/Server.o: $(SRC_DIR)/Server.cpp $(SRC_DIR)/Server.hpp $(DEPS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(BLD_DIR)/Client.o: $(SRC_DIR)/Client.cpp $(SRC_DIR)/Client.hpp $(DEPS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

# Phony's and Commands

.PHONY: clean
clean:
	@rm -rf $(BLD_DIR) $(BIN_DIR) $(DOC_DIR)

.PHONY: docs
docs:
	@mkdir $(DOC_DIR)