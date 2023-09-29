#include <iostream>
#include <vector>
#include <windows.h>
#include <oledb.h>

struct Employee {
    int id;
    std::wstring name;
    int age;
};

int main() {
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM: 0x" << std::hex << hr << std::dec << std::endl;
        return 1;
    }

    IDataSourceLocator* pDataSourceLocator = nullptr;
    hr = CoCreateInstance(
        CLSID_DataLinks,           // OLE DB Data Links
        nullptr,                   // Not aggregating the object
        CLSCTX_INPROC_SERVER,
        IID_IDataSourceLocator,
        (void**)&pDataSourceLocator
    );

    if (FAILED(hr)) {
        std::cerr << "Failed to create Data Links instance: 0x" << std::hex << hr << std::dec << std::endl;
        CoUninitialize();
        return 1;
    }

    // Initialize OLE DB
    hr = pDataSourceLocator->Initialize();
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize OLE DB: 0x" << std::hex << hr << std::dec << std::endl;
        pDataSourceLocator->Release();
        CoUninitialize();
        return 1;
    }

    // Create an OLE DB connection
    IDBCreateSession* pDBCreateSession = nullptr;
    hr = pDataSourceLocator->CreateSession(nullptr, IID_IDBCreateSession, (IUnknown**)&pDBCreateSession);
    if (FAILED(hr)) {
        std::cerr << "Failed to create OLE DB session: 0x" << std::hex << hr << std::dec << std::endl;
        pDataSourceLocator->Release();
        CoUninitialize();
        return 1;
    }

    // Open a database connection (replace "your_connection_string" and "your_query" with actual values)
    IDBInitialize* pDBInitialize = nullptr;
    hr = pDBCreateSession->CreateSession(nullptr, IID_IDBInitialize, (IUnknown**)&pDBInitialize);
    if (FAILED(hr)) {
        std::cerr << "Failed to create OLE DB initialize object: 0x" << std::hex << hr << std::dec << std::endl;
        pDBCreateSession->Release();
        pDataSourceLocator->Release();
        CoUninitialize();
        return 1;
    }

    // Initialize the data source connection with your connection string
    hr = pDBInitialize->Initialize();
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize data source: 0x" << std::hex << hr << std::dec << std::endl;
        pDBInitialize->Release();
        pDBCreateSession->Release();
        pDataSourceLocator->Release();
        CoUninitialize();
        return 1;
    }

    // Execute your SQL query (replace "your_query" with your actual query)
    ICommandText* pCommandText = nullptr;
    hr = pDBInitialize->QueryInterface(IID_ICommandText, (void**)&pCommandText);
    if (FAILED(hr)) {
        std::cerr << "Failed to obtain ICommandText interface: 0x" << std::hex << hr << std::dec << std::endl;
        pDBInitialize->Release();
        pDBCreateSession->Release();
        pDataSourceLocator->Release();
        CoUninitialize();
        return 1;
    }

    const wchar_t* query = L"SELECT * FROM YourTable"; // Replace with your SQL query
    hr = pCommandText->SetCommandText(DBGUID_DEFAULT, query);
    if (FAILED(hr)) {
        std::cerr << "Failed to set SQL query: 0x" << std::hex << hr << std::dec << std::endl;
        pCommandText->Release();
        pDBInitialize->Release();
        pDBCreateSession->Release();
        pDataSourceLocator->Release();
        CoUninitialize();
        return 1;
    }

    // Execute the query
    IRowset* pRowset = nullptr;
    hr = pCommandText->Execute(nullptr, IID_IRowset, nullptr, nullptr, (IUnknown**)&pRowset);
    if (FAILED(hr)) {
        std::cerr << "Failed to execute SQL query: 0x" << std::hex << hr << std::dec << std::endl;
        pCommandText->Release();
        pDBInitialize->Release();
        pDBCreateSession->Release();
        pDataSourceLocator->Release();
        CoUninitialize();
        return 1;
    }

    // Fetch and serialize the data into Employee structs
    std::vector<Employee> employees;

    HROW hRow;
    hr = pRowset->GetNextRows(nullptr, 0, 1, &hRow);
    while (hr == S_OK || hr == DB_S_ENDOFROWSET) {
        Employee employee;
        DBBINDING bindings[3];
        memset(bindings, 0, sizeof(bindings));

        // Bind the struct members to the result set columns
        bindings[0].iOrdinal = 1;
        bindings[0].wType = DBTYPE_I4;
        bindings[0].obValue = offsetof(Employee, id);
        bindings[0].obLength = 0;
        bindings[0].pTypeInfo = nullptr;
        bindings[0].pObject = nullptr;
        bindings[0].pBindExt = nullptr;

        bindings[1].iOrdinal = 2;
        bindings[1].wType = DBTYPE_WSTR;
        bindings[1].obValue = offsetof(Employee, name);
        bindings[1].obLength = 0;
        bindings[1].pTypeInfo = nullptr;
        bindings[1].pObject = nullptr;
        bindings[1].pBindExt = nullptr;

        bindings[2].iOrdinal = 3;
        bindings[2].wType = DBTYPE_I4;
        bindings[2].obValue = offsetof(Employee, age);
        bindings[2].obLength = 0;
        bindings[2].pTypeInfo = nullptr;
        bindings[2].pObject = nullptr;
        bindings[2].pBindExt = nullptr;

        hr = pRowset->GetData(hRow, hAccessor, &employee, bindings);
        if (SUCCEEDED(hr)) {
            employees.push_back(employee);
        }

        // Move to the next row
        hr = pRowset->ReleaseRows(1, &hRow, nullptr, nullptr, nullptr);
        hr = pRowset->GetNextRows(nullptr, 0, 1, &hRow);
    }

    // Release OLE DB objects
    pRowset->Release();
    pCommandText->Release();
    pDBInitialize->Release();
    pDBCreateSession->Release();
    pDataSourceLocator->Release();
    CoUninitialize();

    // Process and use the retrieved data in the 'employees' vector

    return 0;
}
