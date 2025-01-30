//-----------------------------------------------------------------------------
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
//-----------------------------------------------------------------------------
#include <exception>
#include <string>
//-----------------------------------------------------------------------------
namespace Error{
//-----------------------------------------------------------------------------

class BaseException: public std::exception
{
public:
    BaseException(std::string aWhat)
        :m_what(std::move(aWhat))
    {}

    virtual const char *what() const noexcept override
    {
        return m_what.c_str();
    }

private:
    std::string m_what;
};
//-----------------------------------------------------------------------------

class ArgumentCountException final: public BaseException
{
public:
    ArgumentCountException()
        :BaseException("Error: wrong argument count...")
    {}
};
//-----------------------------------------------------------------------------

class WriteFileException final: public BaseException
{
public:
    WriteFileException(const std::string& aFile)
        :BaseException(std::string("Error: doesn't have permissions to create file: " + aFile))
    {}
};
//-----------------------------------------------------------------------------

class OpenFileException final: public BaseException
{
public:

    OpenFileException(const std::string& aFile)
        :BaseException(std::string("Error: could not open file: " + aFile))
    {}
};
//-----------------------------------------------------------------------------
} // namespace Error
//-----------------------------------------------------------------------------
#endif // EXCEPTIONS_H
//-----------------------------------------------------------------------------
