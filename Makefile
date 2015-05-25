APP_DIR=apps
OBJDIR=build
BINDIR=bin
APP_OBJ_DIR=$(OBJDIR)/app_build
APP_BIN_DIR=$(OBJDIR)/apps
APPMK=$(APP_DIR)/app.mk

all: $(patsubst $(APP_DIR)/%/Makefile,$(BINDIR)/%,$(wildcard $(APP_DIR)/*/Makefile))

ENGINE_DIR=engine
CXX_ENGINE_SRC=$(wildcard $(ENGINE_DIR)/src/*/*.cpp)

TEST_DIR=tests
CXX_TEST_SRC=$(wildcard $(TEST_DIR)/*.cpp)
CXX_TEST_OBJ=$(CXX_TEST_SRC:.cpp=.o)
CXX_TEST_BIN=$(CXX_TEST_OBJ:.o=)
CXX=g++
CXXFLAGS=-g -Werror -Wall -std=c++11
ENGINE_CXXFLAGS=-I$(ENGINE_DIR)/include
LDLIBS=-lncurses

ENGINE_OBJS=$(addprefix $(OBJDIR)/, $(CXX_ENGINE_SRC:.cpp=.o))


include $(wildcard $(APP_DIR)/*/Makefile)

-include $(ENGINE_OBJS:.o=.d)

.PHONY: tests clean all


$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(ENGINE_CXXFLAGS) $< -o $@
	$(CXX) -MM -MT $@ $(CXXFLAGS) $(ENGINE_CXXFLAGS) $< > $(OBJDIR)/$*.d


tests: $(CXX_TEST_BIN)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(ENGINE_CXXFLAGS) $< -o $@

$(TEST_DIR)/%: $(ENGINE_OBJS) $(TEST_DIR)/%.o
	$(CXX) $^ $(LDLIBS) -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	find tests/ -not -name '*.[ch]pp' -type f -delete
	find -name '*.swp' -delete

