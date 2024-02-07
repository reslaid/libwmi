#include "libwmi/wmi.hpp"


int main()
{
    WmiLib wmi;
    
    if (!wmi.connect())
    {
        std::wcerr << L"Failed to connect to WMI" << std::endl;
        return 1;
    }

    if (wmi.execQuery(L"SELECT * FROM Win32_Processor"))
    {
        while (wmi.next())
        {
            std::wcout << L"CPU: " << wmi.getString(L"Name") << std::endl;
            std::wcout << L"Clock: " << wmi.getInt(L"MaxClockSpeed") << L" Mhz" << std::endl;
            std::wcout << L"Cores: " << wmi.getInt(L"NumberOfCores") << std::endl;
            std::wcout << L"Logical Processors: " << wmi.getInt(L"NumberOfLogicalProcessors") << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to execute query for processor information" << std::endl;
    }

    wmi.disconnect();

    return 0;
}
