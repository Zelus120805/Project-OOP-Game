#ifndef _MYEXCEPTION_H_
#define _MYEXCEPTION_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class MyException : public std::exception {
protected:
	std::string _messageDes;
	int _codeErr;
	std::string _fullMessage;

	static std::string BuildMessage(const int& code, const std::string& des);
public:
	MyException(const int& code, const std::string& des);
    ~MyException() noexcept = default;
public:
	const char* what() const noexcept override;
	const std::string& GetErrorDescription();
	const int& GetErrorCode();
};

class NullReferenceException : public MyException {
public:
	NullReferenceException(const int& code, const std::string& des);
};

class IndexOutOfRangeException : public MyException {
public:
	IndexOutOfRangeException(const int& code, const std::string& des);
};

class FileNotFoundException : public MyException {
public:
	FileNotFoundException(const int& code, const std::string& des);
};

class InvalidDataException : public MyException {
public:
	InvalidDataException(const int& code, const std::string& des);
};

#endif // _ERROR_H_