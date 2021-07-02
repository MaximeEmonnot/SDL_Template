#include "JSONParser.h"

JSONParser::JSONParser(const std::string& filePath)
{
	std::ifstream file;
	file.open(filePath);
	if (!file)
		throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Opening.\nPlease check JSON file path.", __LINE__);
	rapidjson::IStreamWrapper isw(file);
	doc.ParseStream(isw);
	file.close();
}

rapidjson::Value& JSONParser::GetValueOf(const std::string& name)
{
	rapidjson::Value& v = doc[name.c_str()];
	return v;
}
