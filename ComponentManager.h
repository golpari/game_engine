#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "lua.hpp"
#include "LuaBridge.h"

#include "EngineUtils.h"

#include <filesystem>

static lua_State* lua_state;
static std::unordered_map<std::string, std::shared_ptr<luabridge::LuaRef>> component_tables; //map id to component type

class ComponentManager {

public:
    static void Initialize();
    static void InitializeState();
    static void InitializeFunctions();
    static void InitializeComponents();
    static void EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table);

    static void Print(const std::string& message) {
        std::cout << message << std::endl;
    }
    static void PrintError(const std::string& message) {
        std::cerr << message << std::endl;
    }
};

#endif