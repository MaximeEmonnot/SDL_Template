#include "JSONParser.h"

JSONParser::JSONParser(const std::string& filePath)
{
	std::ifstream file;
	file.open(filePath);
	if (!file)
		throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Opening.\nPlease check JSON file path.", __LINE__);
	rapidjson::IStreamWrapper isw(file);
	doc.ParseStream(isw);
	if (doc.HasParseError()) {
		switch (doc.GetParseError()) {
		case rapidjson::ParseErrorCode::kParseErrorDocumentEmpty :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: The document is empty.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorDocumentRootNotSingular :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: The document root must not follow by other values.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorValueInvalid :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Invalid value.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorObjectMissName :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Missing a name for object member.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorObjectMissColon :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Missing a colon after a name of object member.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorObjectMissCommaOrCurlyBracket :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Missing a comma or '}' after an object member.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorArrayMissCommaOrSquareBracket :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Missing a comma or ']' after an array element.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorStringUnicodeEscapeInvalidHex :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Incorrect hex digit after \\u escape in string.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorStringUnicodeSurrogateInvalid :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: The surrogate pair in string is invalid.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorStringEscapeInvalid :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Invalid escape character in string.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorStringMissQuotationMark :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Missing a closing quotation mark in string.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorStringInvalidEncoding :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Invalid encoding in string.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorNumberTooBig :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Number too big to be stored in double.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorNumberMissFraction :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Miss fraction part in number.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorNumberMissExponent :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Miss exponent in number.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorTermination :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Parsing was terminated.\nPlease check the document.", __LINE__);
			break;
		case rapidjson::ParseErrorCode::kParseErrorUnspecificSyntaxError :
			throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Parsing.\nError information: Unspecific syntax error.\nPlease check the document.", __LINE__);
			break;
		default:
			break;
		}
	}
	file.close();
}

rapidjson::Value& JSONParser::GetValueOf(const std::string& name)
{
	rapidjson::Value& v = doc[name.c_str()];
	return v;
}
