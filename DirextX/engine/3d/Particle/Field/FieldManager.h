#pragma once
#include <list>
#include "Field.h"
#include "Vector3.h"
#include <unordered_map>
#include <string>
#include <memory>

class FieldManager
{
public:
	struct FieldGroup {
		std::list<std::unique_ptr<Field>> field;
		bool isOn;
		Vector3 center;
	};

	FieldGroup* GetFieldGroup(std::string name);



private:
	std::unordered_map<std::string, std::unique_ptr<FieldGroup>> fieldGroup_;
};

