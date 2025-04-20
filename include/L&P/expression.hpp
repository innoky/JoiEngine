#include <string>
#include <vector>
#include <utility>

class Parser
{
public:
    std::string content;
    std::vector<std::pair<std::string, short>> tokens;

    std::vector<std::pair<std::string, short>> parse();
};