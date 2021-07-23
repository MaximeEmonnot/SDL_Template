#pragma once
#include "EngineException.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <fstream>

namespace JSONParser {
	class Reader {
	public:
		void ReadFile(const std::string& filePath);
		bool IsValueAvailable(const std::string& name);
		rapidjson::Value& GetValueOf(const std::string& name);
	private:
		rapidjson::Document doc;
	};
	class Writer {
	public:
		Writer();

		//Add a simple member
		template <typename...Args>
		void AddValueForMember(const std::string& memberName, Args... value) {
			rapidjson::Value v(rapidjson::kArrayType);
			SetArrayValue(v, value...);
			rapidjson::Value name;
			name.SetString(memberName.c_str(), doc.GetAllocator());
			doc.AddMember(name, v, doc.GetAllocator());
		}
		template <typename T>
		void AddValueForMember(const std::string& memberName, T value) {
			rapidjson::Value name;
			name.SetString(memberName.c_str(), doc.GetAllocator());
			rapidjson::Value v(value);
			doc.AddMember(name, v, doc.GetAllocator());
		}
		void AddStringValueForMember(const std::string& memberName, const std::string& value);

		//Add an object member, that can contains simple members
		template<typename...Args>
		void AddObjectMember(const std::string& objectName, const std::string& memberName, Args...value) {
			if (doc.HasMember(objectName.c_str())) {
				rapidjson::Value v(rapidjson::kArrayType);
				SetArrayValue(v, value...);
				rapidjson::Value name(memberName.c_str(), doc.GetAllocator());
				doc.FindMember(objectName.c_str())->value.AddMember(name, v, doc.GetAllocator());
			}
			else {
				rapidjson::Value v;
				v.SetObject();
				VALUEOBJECT_AddValueForMember(v, memberName, value...);
				rapidjson::Value name(objectName.c_str(), doc.GetAllocator());
				doc.AddMember(name, v, doc.GetAllocator());
			}
		}
		template<typename T>
		void AddObjectMember(const std::string& objectName, const std::string& memberName, T value) {
			if (doc.HasMember(objectName.c_str())) {
				rapidjson::Value name(memberName.c_str(), doc.GetAllocator());
				rapidjson::Value v(value);
				doc.FindMember(objectName.c_str())->value.AddMember(name, v, doc.GetAllocator());
			}
			else {
				rapidjson::Value v;
				v.SetObject();
				VALUEOBJECT_AddValueForMember(v, memberName, value);
				rapidjson::Value name(objectName.c_str(), doc.GetAllocator());
				doc.AddMember(name, v, doc.GetAllocator());
			}
		}
		void AddObjectStringMember(const std::string& objectName, const std::string& memberName, const std::string& value);


		void SaveJsonAt(const std::string& path);
	private:

		template<typename...Args>
		void VALUEOBJECT_AddValueForMember(rapidjson::Value& v, const std::string& memberName, Args... value) {
			if (!v.IsObject()) 
				throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Writing.\nType of error: Tried to add a member to a non-object member.\nPlease check your JSON file.", __LINE__);

			rapidjson::Value subV(rapidjson::kArrayType);
			SetArrayValue(subV, value...);
			rapidjson::Value name;
			name.SetString(memberName.c_str(), doc.GetAllocator());
			v.AddMember(name, subV, doc.GetAllocator());
		}
		template<typename T>
		void VALUEOBJECT_AddValueForMember(rapidjson::Value& v, const std::string& memberName, T value) {
			if (!v.IsObject())
				throw EngineException("JSON Exception caught", __FILE__, "An error has been caught during JSON File Writing.\nType of error: Tried to add a member to a non-object member.\nPlease check your JSON file.", __LINE__);
			rapidjson::Value name;
			name.SetString(memberName.c_str(), doc.GetAllocator());
			v.AddMember(name, value, doc.GetAllocator());
		}
		void VALUEOBJECT_AddValueForMember(rapidjson::Value& v, const std::string& memberName, const std::string& value);

		template<typename T>
		void SetArrayValue(rapidjson::Value& v, T value) {
			v.PushBack(value, doc.GetAllocator());
		}
		template<typename T, typename...Args>
		void SetArrayValue(rapidjson::Value& v, T first, Args... values) {
			v.PushBack(first, doc.GetAllocator());
			SetArrayValue(v, values...);
		}

	private:
		rapidjson::Document doc;
	};
};