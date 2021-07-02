#pragma once
#include <string>

class EngineException
{
public:
	EngineException(const std::string& type, const std::string& file, const std::string& note, unsigned int line) noexcept
		:
		mType(type),
		mFile(file),
		mNote(note),
		mLine(line)
	{}

	inline std::string GetType() const noexcept {
		return mType;
	}
	inline const std::string& GetFile() const noexcept {
		return mFile;
	}
	inline const std::string& GetNote() const noexcept {
		return mNote;
	}
	inline unsigned int GetLine() const noexcept {
		return mLine;
	}
	inline std::string GetLocation() const noexcept {
		return std::string("[File]: \"") + mFile + std::string("\"\n[Line]: ") + std::to_string(mLine);
	}
	inline std::string GetMessage() const noexcept {
		return GetNote() + "\nDetails:\n" + GetLocation();
	}

private:
	std::string		mType;
	std::string		mFile;
	std::string		mNote;
	unsigned int	mLine;
};

