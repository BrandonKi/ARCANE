#include "TypeManager.h"

TypeManager::TypeManager() {

}

type_handle TypeManager::add_type(const astring& name) {
    // TODO actually support adding more types
    // this is just placeholder
    type_table.push_back({name, 8});
    return static_cast<type_handle>(type_table.size() - 1);
}

type_handle TypeManager::get_type_handle(const astring& name) {
    for(auto i = 0; i < type_table.size(); ++i) //FIXME if this is a common operation then consider a hash table
        if(type_table[i].name == name)
            return i;
    return -1;
}

Type TypeManager::get_type(type_handle handle) {
    return type_table[handle];
}

void TypeManager::add_alias(const astring& alias, const astring& original) {

}

void TypeManager::define_operator() {

}