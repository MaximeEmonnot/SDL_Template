#pragma once
#include <string>

class SDLException
{
public:
	SDLException(const std::string& file, unsigned int line, const std::string& note) noexcept
		:
		file(file),
		note(note),
		line(line)
	{}

	inline const std::string& GetFile() const noexcept {
		return file;
	}
	inline const std::string& GetNote() const noexcept {
		return note;
	}
	inline unsigned int GetLine() const noexcept {
		return line;
	}
	inline std::string GetLocation() const noexcept {
		return std::string("[File]: \"") + file + std::string("\"\n[Line]: ") + std::to_string(line);
	}
	inline std::string GetMessage() const noexcept {
		return GetNote() + "\nDetails:\n" + GetLocation();
	}
	virtual inline std::string GetType() const noexcept = 0;

private:
	std::string file;
	std::string note;
	unsigned int line;
};

