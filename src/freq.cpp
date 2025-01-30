//-----------------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <future>
//-----------------------------------------------------------------------------
#include "freq.h"
#include "exceptions.h"
//-----------------------------------------------------------------------------
namespace Logic {
//-----------------------------------------------------------------------------

Freq::Freq(const std::string& aInputFile, const std::string& aOutputFile)
    : m_inpath(aInputFile),
      m_outpath(aOutputFile)
{
    if(!fs::exists(m_inpath))
        throw Error::OpenFileException(m_inpath);
}
//-----------------------------------------------------------------------------

bool Freq::Run()
{
    auto vec = ConvertToVector( FormatString( ReadData() ));
    auto calcData = CalculateTokensFreq(std::move(vec));
    auto sortedData = SortOutputData(std::move(calcData));
    WriteData(std::move(sortedData));
    return true;
}
//-----------------------------------------------------------------------------

std::string Freq::ReadData() const
{
    std::string buf;
    buf.resize(fs::file_size(m_inpath));

    std::ifstream ifs(m_inpath.string(), std::ifstream::binary);
    if(!ifs.is_open())
        throw Error::OpenFileException(m_inpath.string());

    ifs.read(&buf[0], buf.size());
    return buf;
}
//-----------------------------------------------------------------------------

std::string Freq::FormatString(std::string&& aData) const
{
    std::transform(aData.begin(), aData.end(), aData.begin(),
        [](auto&& c){
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                return std::tolower(c);
            else
                return static_cast<int>(' ');
        });
    return aData;
}
//-----------------------------------------------------------------------------

std::vector<std::string> Freq::ConvertToVector(std::string&& aData) const
{
    std::vector<std::string> vec;
    vec.reserve(aData.size());

    auto delim = ' ';
    size_t startIdx = 0;
    size_t endIdx = 0;

    while ((startIdx = aData.find_first_not_of(delim, endIdx))!= std::string::npos)
    {
        endIdx = aData.find(delim, startIdx);
        vec.push_back(aData.substr(startIdx, endIdx - startIdx));
    }
    return vec;
}
//-----------------------------------------------------------------------------

Freq::TokenMap Freq::CalculateTokensFreq(std::vector<std::string>&& aData) const
{
    // lamda function for populating the data in async task
    auto getTokensCount = [this](auto&& first, auto&& last)
    {
        Freq::TokenMap res;
        while(first != last)
        {
            auto count = res[*first];
            res[*first] += 1;
            ++first;
        }
        return res;
    };

    const auto threadCount = 4;
    auto tokenPerThread = aData.size() / threadCount;
    auto tailtokens = aData.size() % threadCount;

    std::vector<std::future<Freq::TokenMap>> futures;
    auto first = aData.begin();
    if(tokenPerThread)
    {
        for(auto i=0; i<threadCount; ++i)
        {
            futures.push_back(std::async(std::launch::async, getTokensCount, first, first + tokenPerThread));
            first += tokenPerThread;
        }
    }
    if(tailtokens)
    {
        futures.push_back(std::async(std::launch::async, getTokensCount, first, aData.end()));
    }

    //save data from all async tasks into the
    Freq::TokenMap totalData;
    for (auto&& fut : futures)
    {
        auto data = fut.get();
        for(auto&& [k,v] : data)
        {
            auto count = totalData[k];
            count += v;
            totalData[k] = count;
        }
    }
    return totalData;
}
//-----------------------------------------------------------------------------

std::vector<Freq::Token> Freq::SortOutputData(TokenMap&& aData) const
{
    std::vector<Token> results;
    results.reserve(aData.size());

    std::transform(aData.begin(), aData.end(),
                   std::back_inserter(results),
                   [](auto&& p){return Token(p.first, p.second);});

    std::sort(results.begin(), results.end());

    return results;
}
//-----------------------------------------------------------------------------

void Freq::WriteData(std::vector<Token>&& aData)
{
    std::ofstream ofs(m_outpath.string(), std::ofstream::binary);
    if(!ofs.is_open())
        throw Error::OpenFileException(m_outpath.string());
    for(auto&& el : aData)
    {
        ofs << el.m_count<<' '<<el.m_token<<'\n';
    }
}
//-----------------------------------------------------------------------------
} // namespace Logic
//-----------------------------------------------------------------------------
