//-----------------------------------------------------------------------------
#ifndef FREQ_H
#define FREQ_H
//-----------------------------------------------------------------------------
#include <filesystem>
#include <string>
#include <map>
#include <vector>
//-----------------------------------------------------------------------------
namespace Logic {
//-----------------------------------------------------------------------------
namespace fs = std::filesystem;

class Freq final
{
    using TokenMap=std::map<std::string, size_t>;
    struct Token
    {
        Token(std::string token, size_t count = 1)
            :m_token(std::move(token)), m_count(count)
        {}
        bool operator<(const Token& rhs) const
        {
            return std::tie(m_count, rhs.m_token) > std::tie(rhs.m_count, m_token);
        }
        std::string m_token;
        size_t m_count;
    };

public:
    Freq(const std::string& aInputFile, const std::string& aOutputFile);
    bool Run();

private:
    std::string ReadData() const;
    std::string FormatString(std::string&& aData) const;
    std::vector<std::string> ConvertToVector(std::string&& aData) const;
    TokenMap CalculateTokensFreq(std::vector<std::string>&& aData) const;
    std::vector<Token> SortOutputData(TokenMap&& aData) const;
    void WriteData(std::vector<Token>&& aData);
private:
    fs::path m_inpath;
    fs::path m_outpath;
};
//-----------------------------------------------------------------------------
} // namespace Logic
//-----------------------------------------------------------------------------
#endif // FREQ_H
//-----------------------------------------------------------------------------
