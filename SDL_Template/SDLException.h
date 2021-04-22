#pragma once
#include <string>

class SDLException
{
public:
	SDLException(const std::string& file, unsigned int line, const std::string& note)
		:
		file(file),
		note(note),
		line(line)
	{}

	const std::string& GetFile() const {
		return file;
	}
	const std::string& GetNote() const {
		return note;
	}
	unsigned int GetLine() const {
		return line;
	}
	std::string GetLocation() const {
		return std::string("[File]: \"") + file + std::string("\"\n[Line]: ") + std::to_string(line);
	}
	std::string GetMessage() const {
		return GetNote() + "\nDetails:\n" + GetLocation();
	}
	virtual std::string GetType() const = 0;

private:
	std::string file;
	std::string note;
	unsigned int line;
};

