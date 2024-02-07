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
    WmiLib() : pLoc(NULL), pSvc(NULL), pEnumerator(NULL), pclsObj(NULL) {}

    /**
    * @brief WmiLib class destructor
    */
    ~WmiLib()
    {
        disconnect();
    }

    /**
    * @brief Establishes a connection to WMI
    * @return true if the connection was successful, false otherwise
    */
    bool connect()
    {
        HRESULT hres;

        hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres))
        {
            std::cerr << "Failed to initialize COM library. Error code: " << std::hex << hres << std::endl;
            return false;
        }

        hres = CoInitializeSecurity(
            NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE,
            NULL
        );

        if (FAILED(hres))
        {
            std::cerr << "Failed to initialize security. Error code: " << std::hex << hres << std::endl;
            CoUninitialize();
            return false;
        }

        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            (LPVOID*)&pLoc
        );

        if (FAILED(hres))
        {
            std::cerr << "Failed to create IWbemLocator object. Error code: " << std::hex << hres << std::endl;
            CoUninitialize();
            return false;
        }

        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"),
            NULL,
            NULL,
            0,
            NULL,
            0,
            0,
            &pSvc
        );

        if (FAILED(hres))
        {
            std::cerr << "Could not connect to WMI namespace. Error code: " << std::hex << hres << std::endl;
            pLoc->Release();
            CoUninitialize();
            return false;
        }

        hres = CoSetProxyBlanket(
            pSvc,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            NULL,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE
        );

        if (FAILED(hres))
        {
            std::cerr << "Could not set proxy blanket. Error code: " << std::hex << hres << std::endl;
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return false;
        }

        return true;
    }

    /**
    * @brief Closes the WMI connection
    */
    void disconnect()
    {
        if (pEnumerator != NULL)
        {
            pEnumerator->Release();
            pEnumerator = NULL;
        }

        if (pSvc != NULL)
        {
            pSvc->Release();
            pSvc = NULL;
        }

        if (pLoc != NULL)
        {
            pLoc->Release();
            pLoc = NULL;
        }

        CoUninitialize();
    }

    /**
    * @brief Performs a WMI query
    * @param query WMI query in WQL format
    * @return true if the request was successful, false otherwise
    */
    bool execQuery(const std::wstring& query)
    {
        HRESULT hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t(query.c_str()),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator
        );
        return SUCCEEDED(hres);
    }

    /**
    * @brief Moves to the next query result object
    * @return true if the transition was successful, false otherwise
    */
    bool next()
    {
        ULONG uReturn;
        HRESULT hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        return (uReturn != 0);
    }

    /**
    * @brief Gets the string value of a WMI object property
    * @param propName Property name
    * @return String value of the property
    */
    std::wstring getString(const std::wstring& propName)
    {
        VARIANT vtProp;
        HRESULT hres = pclsObj->Get(propName.c_str(), 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres) && vtProp.vt == VT_BSTR)
        {
            return std::wstring(vtProp.bstrVal);
        }
        return L"";
    }

    /**
    * @brief Gets the integer value of a WMI object property
    * @param propName Property name
    * @return The integer value of the property
    */
    int getInt(const std::wstring& propName)
    {
        VARIANT vtProp;
        HRESULT hres = pclsObj->Get(propName.c_str(), 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres) && vtProp.vt == VT_I4)
        {
            return vtProp.intVal;
        }
        return 0;
    }

    /**
    * @brief Gets the floating point value of a WMI object property
    * @param propName Property name
    * @return The float value of the property
    */
    double getDouble(const std::wstring& propName)
    {
        VARIANT vtProp;
        HRESULT hres = pclsObj->Get(propName.c_str(), 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres) && vtProp.vt == VT_R8)
        {
            return vtProp.dblVal;
        }
        return 0.0;
    }

    /**
    * @brief Gets the boolean value of a WMI object property
    * @param propName Property name
    * @return Boolean property value
    */
    bool getBool(const std::wstring& propName)
    {
        VARIANT vtProp;
        HRESULT hres = pclsObj->Get(propName.c_str(), 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres) && vtProp.vt == VT_BOOL)
        {
            return vtProp.boolVal != VARIANT_FALSE;
        }
        return false;
    }

private:
    IWbemLocator* pLoc; ///< Pointer to IWbemLocator
    IWbemServices* pSvc; ///< Pointer to IWbemServices
    IEnumWbemClassObject* pEnumerator; ///< Pointer to IEnumWbemClassObject
    IWbemClassObject* pclsObj; ///< Pointer to IWbemClassObject
};

#endif
