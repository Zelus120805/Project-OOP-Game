#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "MyException.h"

TEST_CASE("MyException basic functionality") {
    MyException ex(404, "File not found");
    
    CHECK(ex.GetErrorCode() == 404);
    CHECK(ex.GetErrorDescription() == "File not found");
    CHECK(std::string(ex.what()) == "Error code: 404 - Description: File not found");
}

TEST_CASE("NullReferenceException behaves like MyException") {
    NullReferenceException ex(1001, "Null reference");
    
    CHECK(ex.GetErrorCode() == 1001);
    CHECK(ex.GetErrorDescription() == "Null reference");
    CHECK(std::string(ex.what()) == "Error code: 1001 - Description: Null reference");
}

TEST_CASE("IndexOutOfRangeException behaves like MyException") {
    IndexOutOfRangeException ex(2002, "Index out of range");
    
    CHECK(ex.GetErrorCode() == 2002);
    CHECK(ex.GetErrorDescription() == "Index out of range");
    CHECK(std::string(ex.what()) == "Error code: 2002 - Description: Index out of range");
}

TEST_CASE("FileNotFoundException behaves like MyException") {
    FileNotFoundException ex(3003, "File not found");
    
    CHECK(ex.GetErrorCode() == 3003);
    CHECK(ex.GetErrorDescription() == "File not found");
    CHECK(std::string(ex.what()) == "Error code: 3003 - Description: File not found");
}

TEST_CASE("InvalidDataException behaves like MyException") {
    InvalidDataException ex(4004, "Invalid data");
    
    CHECK(ex.GetErrorCode() == 4004);
    CHECK(ex.GetErrorDescription() == "Invalid data");
    CHECK(std::string(ex.what()) == "Error code: 4004 - Description: Invalid data");
}