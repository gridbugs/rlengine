TARGET=game
OBJDIR=build
BINDIR=bin

CXX_ENGINE_SRC=$(wildcard src/*/*.cpp)

TEST_DIR=tests
CXX_TEST_SRC=$(wildcard $(TEST_DIR)/*.cpp)
CXX_TEST_OBJ=$(CXX_TEST_SRC:.cpp=.o)
CXX_TEST_BIN=$(CXX_TEST_OBJ:.o=)
CXX=g++
CXXFLAGS=-g -Werror -Wall -std=c++14
ENGINE_CXXFLAGS=-Iinclude
LDLIBS=-lncurses

ENGINE_OBJS=$(addprefix $(OBJDIR)/, $(CXX_ENGINE_SRC:.cpp=.o))

all: $(BINDIR)/$(TARGET)

-include $(ENGINE_OBJS:.o=.d)

.PHONY: tests clean all

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(ENGINE_CXXFLAGS) $< -o $@
	$(CXX) -MM -MT $@ $(CXXFLAGS) $(ENGINE_CXXFLAGS) $< > $(OBJDIR)/$*.d

$(BINDIR)/%: $(ENGINE_OBJS)
	mkdir -p $(dir $@)
	$(CXX) $^ $(LDLIBS) -o $@

tests: $(CXX_TEST_BIN)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(ENGINE_CXXFLAGS) $< -o $@

$(TEST_DIR)/%: $(ENGINE_OBJS) $(TEST_DIR)/%.o
	$(CXX) $^ $(LDLIBS) -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	find . -name '*.swp' -delete

