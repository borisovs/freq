//-----------------------------------------------------------------------------
#ifndef FREQ_H
#define FREQ_H
//-----------------------------------------------------------------------------
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
//-----------------------------------------------------------------------------
namespace Logic {
//-----------------------------------------------------------------------------
constexpr auto ITEMS_SIZE = 26;
constexpr auto START_SYMBOL = 'a';
//-----------------------------------------------------------------------------
namespace fs = std::filesystem;

class Freq final
{
    struct TrieNode
    {
        TrieNode(TrieNode* aParent, char aChar, size_t aLenght)
            : m_count(0),
              m_parent(aParent),
              m_char(aChar),
              m_lenght(aLenght)
        {
            std::memset(m_items, 0, sizeof(m_items));
        }

        TrieNode* AddChar(char aChar)
        {
            auto idx = aChar - START_SYMBOL;
            if(!m_items[idx])
            {
                m_items[idx] = new TrieNode(this, aChar, m_lenght + 1);
            }
            return m_items[idx];
        }

        void IncCount()
        {
            m_count++;
        }

        size_t m_count;
        TrieNode* m_items[ITEMS_SIZE];
        TrieNode* m_parent;
        char m_char;
        size_t m_lenght;
    };

    using TokenMap = std::unordered_map<int, std::vector<TrieNode*>>;
    using TokenSet = std::unordered_set<int>;

public:
    Freq(const std::string& aInputFile, const std::string& aOutputFile);
    bool Run();

private:
    std::vector<char> ReadData() const;
    std::unique_ptr<TrieNode> BuildTrie(std::vector<char>&& aVec) const;
    void TraceTrie(TrieNode* aNode);
    void WriteTrie();

private:
    fs::path m_inpath;
    fs::path m_outpath;
    TokenMap m_wordsByCount;
    TokenSet m_counts;
};
//-----------------------------------------------------------------------------
} // namespace Logic
//-----------------------------------------------------------------------------
#endif // FREQ_H
//-----------------------------------------------------------------------------
