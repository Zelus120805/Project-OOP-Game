#include "MyException.h"

MyException::MyException(const int& code, const std::string& des) : _codeErr(code), _messageDes(des), _fullMessage(BuildMessage(code, des)) { }

std::string MyException::BuildMessage(const int& code, const std::string& des) {
    std::ostringstream oss;
    oss << "Error code: " << code << " - Description: " << des;
    return oss.str();
}

const char* MyException::what() const noexcept {
    return _fullMessage.c_str();
}

const std::string& MyException::GetErrorDescription() {
    return _messageDes;
}

const int& MyException::GetErrorCode() {
    return _codeErr;
}

NullReferenceException::NullReferenceException(const int& code, const std::string& des) : MyException(code, des) { }

IndexOutOfRangeException::IndexOutOfRangeException(const int& code, const std::string& des) : MyException(code, des) { }

FileNotFoundException::FileNotFoundException(const int& code, const std::string& des) : MyException(code, des) { }

InvalidDataException::InvalidDataException(const int& code, const std::string& des) : MyException(code, des) { }