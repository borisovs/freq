//-----------------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>
//-----------------------------------------------------------------------------
#include "freq.h"
#include "exceptions.h"
//-----------------------------------------------------------------------------
namespace Logic {
//-----------------------------------------------------------------------------

static void PrintElapsedime(const char* func, long count)
{
    std::cout << std::setw(10) << func <<":\t" << count << " ms" << std::endl;
}
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
    auto before = std::chrono::high_resolution_clock::now();
    auto data = ReadData();
    PrintElapsedime("ReadData", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count());

    before = std::chrono::high_resolution_clock::now();
    auto rootNode = BuildTrie(std::move(data));
    PrintElapsedime("BuildTrie", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count());

    before = std::chrono::high_resolution_clock::now();
    TraceTrie(rootNode.get());
    PrintElapsedime("TraceTrie", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count());

    before = std::chrono::high_resolution_clock::now();
    WriteTrie();
    PrintElapsedime("WriteTrie", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count());

    return true;
}
//-----------------------------------------------------------------------------

std::vector<char> Freq::ReadData() const
{
    std::ifstream ifs(m_inpath.string(), std::ifstream::binary);
    if(!ifs.is_open())
        throw Error::OpenFileException(m_inpath.string());

    auto fileSize = fs::file_size(m_inpath);
    std::vector<char> buf(fileSize);

    ifs.read(buf.data(), fileSize);
    return buf;
}
//-----------------------------------------------------------------------------

std::unique_ptr<Freq::TrieNode> Freq::BuildTrie(std::vector<char>&& aVec) const
{
    auto rootNode = std::make_unique<TrieNode>(nullptr, 0, 0);
    auto pNode = rootNode.get();
    for (auto c : aVec)
    {
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
        {
            if (pNode != rootNode.get())
            {
                pNode->IncCount();
            }
            pNode = rootNode.get();
            continue;
        }

        pNode = pNode->AddChar(std::tolower(c));
    }

    if (pNode != rootNode.get())
    {
        pNode->IncCount();
    }
    return rootNode;
}
//-----------------------------------------------------------------------------

void Freq::TraceTrie(TrieNode* aNode)
{
    if (aNode->m_count)
    {
        m_wordsByCount[aNode->m_count].push_back(aNode);
        m_counts.insert(aNode->m_count);
    }

    for (int i = 0; i < 26; ++i)
    {
        if (aNode->m_items[i])
        {
            TraceTrie(aNode->m_items[i]);
        }
    }
}
//-----------------------------------------------------------------------------

void Freq::WriteTrie()
{
    std::ofstream ofs(m_outpath.string(), std::ofstream::binary);
    if(!ofs.is_open())
        throw Error::OpenFileException(m_outpath.string());

    std::vector<int> vecCounts(m_counts.begin(), m_counts.end());
    std::sort(vecCounts.begin(), vecCounts.end(), std::greater<int>());

    for (auto count : vecCounts)
    {
        for (auto iter = m_wordsByCount[count].begin(); iter != m_wordsByCount[count].end(); ++iter)
        {
            auto node = *iter;
            std::string str((size_t)node->m_lenght, 0);
            int i = node->m_lenght - 1;
            while (i >= 0 and node->m_char)
            {
                str[i--] = node->m_char;
                node = node->m_parent;
            }
            ofs << count <<' '<<str<<'\n';
        }
    }
}
//-----------------------------------------------------------------------------
} // namespace Logic
//-----------------------------------------------------------------------------
