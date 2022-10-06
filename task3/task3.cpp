#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;



bool isDirectory(wstring path);
void getItems(wstring dirName);
wstring toRussian(wstring str);


string copyName;


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_CTYPE, "rus");
    string nameDir;

    cout << "Введите путь к директории: ";
    getline(cin, nameDir, '\n');
    cout << "Введите имя папки:";
    getline(cin, copyName, '\n');

    getItems(toRussian(wstring(nameDir.begin(), nameDir.end())));

    return 0;
}

void getItems(wstring dirName)
{
    if (!isDirectory(dirName))
    {
        cout << "Нужен путь к папке, а не файлу\n";
    }
    else {

        wstring pathEnd = L"*";
        // Если не корень
        if (dirName[dirName.length() - 1] != L'\\' && dirName[dirName.length() - 1] != L'/')
            pathEnd = L"\\*";
        dirName += pathEnd;
        WIN32_FIND_DATAW file;
        // Поиск папки
        HANDLE hFind = FindFirstFileW(dirName.c_str(), &file);
        FindNextFileW(hFind, &file);
        dirName.erase(dirName.length() - 1, 1);
        wstring name = toRussian(wstring(copyName.begin(), copyName.end()));
        if (hFind != INVALID_HANDLE_VALUE)
        {
            while (FindNextFileW(hFind, &file) != NULL) {

                if (wcscmp(file.cFileName, L".") != 0 && wcscmp(file.cFileName, L"..") != 0)
                {
                    if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Если папка
                    {
                        if (&file.cFileName[0] == name) { // Если название подходит
                            wcout << dirName + &file.cFileName[0] << "\n";
                            getItems(dirName + &file.cFileName[0]);
                        }
                        else {
                            getItems(dirName + &file.cFileName[0]);
                        }
                    }

                }
            }
            FindClose(hFind);
        }
        else
            cout << "Ошибка чтения директории\n";
    }
}



bool isDirectory(wstring path)
{
    DWORD fileAtr = GetFileAttributesW(path.c_str());
    return fileAtr != INVALID_FILE_ATTRIBUTES &&
        fileAtr & FILE_ATTRIBUTE_DIRECTORY;
}




wstring toRussian(wstring str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        if (str[i] > 255)
            str[i] -= 64432;
    }

    return str;
}



