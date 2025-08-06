#include "FieldManager.h"
#include <cassert>
FieldManager::FieldGroup* FieldManager::GetFieldGroup(std::string name)
{
    if (fieldGroup_.contains(name)) {
        return fieldGroup_[name].get();
    }
    assert(false);
    return nullptr;
}
