#ifndef REQUIRED_LIBWMI
#define REQUIRED_LIBWMI

#pragma region includes

  #include <iostream>
  #include <comdef.h>
  #include <WbemIdl.h>

    #pragma comment(lib, "wbemuuid.lib")

#pragma endregion

/**
* @brief Class for working with Windows Management Instrumentation (WMI)
*/
class WmiLib
{
public:
    /**
    * @brief Constructor for the WmiLib class
    */
    WmiLib();

    /**
    * @brief WmiLib class destructor
    */
    ~WmiLib();

    /**
    * @brief Establishes a connection to WMI
    * @return true if the connection was successful, false otherwise
    */
    bool connect();

    /**
    * @brief Closes the WMI connection
    */
    void disconnect();

    /**
    * @brief Performs a WMI query
    * @param query WMI query in WQL format
    * @return true if the request was successful, false otherwise
    */
    bool execQuery(const std::wstring& query);

    /**
    * @brief Moves to the next query result object
    * @return true if the transition was successful, false otherwise
    */
    bool next();

    /**
    * @brief Gets the string value of a WMI object property
    * @param propName Property name
    * @return String value of the property
    */
    std::wstring getString(const std::wstring& propName);

    /**
     * @brief Gets the integer value of a WMI object property
     * @param propName Property name
     * @return The integer value of the property
     */
    int getInt(const std::wstring& propName);

    /**
    * @brief Gets the floating point value of a WMI object property
    * @param propName Property name
    * @return The float value of the property
    */
    double getDouble(const std::wstring& propName);

    /**
    * @brief Gets the boolean value of a WMI object property
    * @param propName Property name
    * @return Boolean property value
    */
    bool getBool(const std::wstring& propName);


private:
    IWbemLocator* pLoc; ///< Pointer to IWbemLocator
    IWbemServices* pSvc; ///< Pointer to IWbemServices
    IEnumWbemClassObject* pEnumerator; ///< Pointer to IEnumWbemClassObject
    IWbemClassObject* pclsObj; ///< Pointer to IWbemClassObject
};

#endif
