#include <string>

class Objects{
public:
    std::wstring ReadFile(const std::string &str);
    void StartTyper(const std::wstring &str, int delay);
};