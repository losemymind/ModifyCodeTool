
#include "FoundationKit/GenericPlatformMacros.hpp"
#include <string>
#include <vector>

NS_FK_BEGIN

class PlatformMisc
{
public:
    static std::vector<std::string>   GetCommandLineArgs();
    static std::string                ExecuteSystemCommand(const std::string& command);
};

NS_FK_END