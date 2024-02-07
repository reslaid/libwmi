# LibWMI ![Version](https://img.shields.io/badge/Version-0.1.0-green.svg) ![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg) ![Standart](https://img.shields.io/badge/C++_STD-14-red.svg)

# **Dependencies**
- [**Git**](https://git-scm.com/downloads)

# Installing
- **Installation from repository**
  ```bash
  git clone https://github.com/reslaid/libwmi.git
  ```

# Docs
- **Including a header file**
    ```cpp
    #include "libwmi/wmi.hpp"
    ```

- **Creating Instance**
    ```cpp
    WmiLib wmi;
    ```

- **Connecting to WMI**
    ```cpp
    if (!wmi.connect())
    {
        std::wcerr << L"Failed to connect to WMI" << std::endl;
        return 1;
    }
    ```

- **Disconnect from WMI**
    ```cpp
    wmi.disconnect();
    ```

- **Query in WQL format (Example)**
    ```cpp
    if (wmi.execQuery(L"SELECT * FROM Win32_Processor"))
    {
        while (wmi.next())
        {
            std::wcout << L"[+] CPU: " << wmi.getString(L"Name") << std::endl;
            std::wcout << L"    Clock: " << wmi.getInt(L"MaxClockSpeed") << L" Mhz" << std::endl;
            std::wcout << L"    Cores: " << wmi.getInt(L"NumberOfCores") << std::endl;
            std::wcout << L"    Logical Processors: " << wmi.getInt(L"NumberOfLogicalProcessors") << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to execute query for processor information" << std::endl;
    }
    ```
