#include "ComponentManager.h"

void ComponentManager::Initialize()
{
    InitializeState();
    InitializeFunctions();
    InitializeComponents();
}

//this func is done
void ComponentManager::InitializeState()
{
    lua_state = luaL_newstate(); //lua_state object
    luaL_openlibs(lua_state); //initialize some default libraries
}

void ComponentManager::InitializeFunctions() {
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Debug")
        .addFunction("Log", ComponentManager::Print)
        .addFunction("LogError", ComponentManager::PrintError)
        .endNamespace();
}

void ComponentManager::InitializeComponents() {
    /*You may “instance” component types like these by creating a new empty Lua table and then establishing inheritance from a base table (the table from the .lua file).
            Establish a base table for each component type via luaL_dofile().
            Once you’ve loaded a table with luaL_dofile(), you can retrieve it with luabridge::getglobal()
            Create a new empty table to represent the new component instance via luabridge::newTable()
            Capture the reference in a luabridge::LuaRef variable. Note that these objects have no default constructor, so may not simply create one on its own. Consult lecture / discussion for tips, and be careful not to accidentally instantiate them via a container such as std::vector<luabridge::LuaRef>.
            Establish inheritance using the raw lua api
*/

    // loop through all the possible files in the component_types 
    if (EngineUtils::CheckPathExists("resources/component_types/", false)) {
        for (const auto& entry : std::filesystem::directory_iterator("resources/component_types/")) {
            if (luaL_dofile(lua_state, entry.path().string().c_str()) != LUA_OK)
            {
                std::cout << "problem with lua file " << entry.path().stem().string();
                exit(0);
            }

            std::string componentName = entry.path().stem().string();
            component_tables.insert({ componentName,
                std::make_shared<luabridge::LuaRef>(
                luabridge::getGlobal(lua_state, componentName.c_str()))
                });
        }
    }    
}

//this func is done
void ComponentManager::EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table)
{
    /* We must create a metatable to establish inheritance in lua. */
    luabridge::LuaRef new_metatable = luabridge::newTable(lua_state);
    new_metatable["__index"] = parent_table;

    /* We must use the raw lua C-API (lua stack) to perform a "setmetatable" operation. */
    instance_table.push(lua_state);

    new_metatable.push(lua_state);

    lua_setmetatable(lua_state, -2);

    lua_pop(lua_state, 1);
}
