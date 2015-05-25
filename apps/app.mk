CXX_SRC=$(wildcard $(APP_DIR)/$(TARGET)/src/*/*.cpp)
OBJS=$(addprefix $(APP_OBJ_DIR)/, $(CXX_SRC:.cpp=.o))
APP_CXXFLAGS=-I$(APP_DIR)/$(TARGET)/include

$(APP_OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(ENGINE_CXXFLAGS) $(APP_CXXFLAGS) $< -o $@
	$(CXX) -MM -MT $@ $(CXXFLAGS) $(ENGINE_CXXFLAGS) $(APP_CXXFLAGS) $< > $(APP_OBJ_DIR)/$*.d

$(BINDIR)/$(TARGET): $(OBJS) $(ENGINE_OBJS)
	mkdir -p $(dir $@)
	$(CXX) $^ $(LDLIBS) -o $@
