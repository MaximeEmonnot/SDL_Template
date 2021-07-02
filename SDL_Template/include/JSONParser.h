#pragma once
#include "EngineException.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <fstream>

class JSONParser {
public:
	JSONParser(const std::string& filePath);

	rapidjson::Value& GetValueOf(const std::string& name);

private:
	rapidjson::Document doc;
};