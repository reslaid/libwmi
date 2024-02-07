#ifndef REQUIRED_LIBWMI
#define REQUIRED_LIBWMI

#pragma region includes

  #include <iostream>
  #include <comdef.h>
  #include <WbemIdl.h>

    #pragma comment(lib, "wbemuuid.lib")

#pragma endregion

  class WmiLib
  {
  public:
      WmiLib();
      ~WmiLib();
  
      bool connect();
      void disconnect();
  
      bool execQuery(const std::wstring& query);
      bool next();
  
      std::wstring getString(const std::wstring& propName);
      int getInt(const std::wstring& propName);
      double getDouble(const std::wstring& propName);
      bool getBool(const std::wstring& propName);
  
  private:
      IWbemLocator* pLoc;
      IWbemServices* pSvc;
      IEnumWbemClassObject* pEnumerator;
      IWbemClassObject* pclsObj;
  };

#endif