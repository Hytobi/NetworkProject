CC := gcc
SRC_DIR ?= src
CJSON ?= cJSON
OBJ_DIR ?= obj
TEST_DIR ?= test
BIN_DIR ?= bin
DOC_DIR ?= doc
DEBUG ?= 1
# LIB ?= 
INCLU ?= -I./lib/include

ifeq '$(DEBUG)' '1'
CFLAGS ?= -Wall -MMD -g $(INCLU)
else
CFLAGS ?= -Wall -MMD -O3 -DNDEBUG $(INCLU)
endif

LDFLAGS = -lcunit $(LIB)



SRC := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/$(CJSON)/*.c)
OBJS := $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o) $($(SRC_DIR)/$(CJSON)/*.c=.o))

SRC_TEST := $(wildcard $(TEST_DIR)/*.c)
OBJS_TEST := $(addprefix $(OBJ_DIR)/, $(SRC_TEST:.c=.o))

DEPS := $(OBJS:.o=.d)
DEPS_TEST := $(OBJS_TEST:.o=.d)

TARGET ?= bin/exec
TARGET_TEST ?= bin/execTest

.PHONY: clean mrproper

all: createRep $(TARGET) #$(TARGET_TEST)

createRep:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR)
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR)/$(CJSON)
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)
	@mkdir -p $(DOC_DIR)

$(TARGET): createRep $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) 
	@echo ""

$(TARGET_TEST):  $(OBJS) $(OBJS_TEST)
	$(CC) -o $(TARGET_TEST) $(filter-out $(OBJ_DIR)/$(SRC_DIR)/main.o ,$(OBJS)) $(OBJS_TEST) $(LDFLAGS) 
	

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
run:
	make
	@./$(TARGET)

runtest:
	make
	@./$(TARGET_TEST)

doc:
	@doxygen Doxyfile

install:
	@./bash/packUtils.sh

arbo:
	@./bash/arbo.sh

memory:
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)

cleandoc:
	rm -rf $(DOC_DIR)/*

mrproper: clean
	rm -rf $(BIN_DIR)

-include $(DEPS) $(DEPS_TEST)