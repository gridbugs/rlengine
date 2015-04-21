TARGET=game
CXX_COMMON_SRC=$(wildcard engine/src/*/*.cpp)

TEST_DIR=tests
CXX_TEST_SRC=$(wildcard $(TEST_DIR)/*.cpp)
CXX_TEST_OBJ=$(CXX_TEST_SRC:.cpp=.o)
CXX_TEST_BIN=$(CXX_TEST_OBJ:.o=)
CXX=g++
CXXFLAGS=-g -Werror -Wall -std=c++11 -Iengine/include
LDLIBS=-lncurses

OBJDIR=build
COMMON_OBJS=$(addprefix $(OBJDIR)/, $(CXX_COMMON_SRC:.cpp=.o))

APPDIR=app

all: $(TARGET)

-include $(COMMON_OBJS:.o=.d)

$(TARGET): $(COMMON_OBJS) $(OBJDIR)/$(APPDIR)/$(TARGET).o
	$(CXX) $^ $(LDLIBS) -o $@


$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $< -o $@
	$(CXX) -MM -MT $@ $(CXXFLAGS) $< > $(OBJDIR)/$*.d


.PHONY: tests clean

tests: $(CXX_TEST_BIN)
runtests: tests
	find $(TEST_DIR) -type f -executable | while read p; do echo $$p ; ./$$p ; echo ; done
	stty sane

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(TEST_DIR)/%: $(COMMON_OBJS) $(TEST_DIR)/%.o
	$(CXX) $^ $(LDLIBS) -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
	find tests/ -not -name '*.[ch]pp' -type f -delete
	find -name '*.swp' -delete

