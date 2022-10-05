#include <iostream>
#include <windows.h>
#include <string>
#include <clocale>
#include <conio.h>
#include <vector>

using namespace std;

vector<string> getKeyboardState();
string convertWcharToStr(wchar_t*);
void printVector(vector<string>);



int main()
{
    setlocale(LC_ALL, "rus");
    vector<string> pressedKeys;
    while (pressedKeys.empty() || pressedKeys[0] != "Esc")
    {
        GetKeyState(0);
        cout << "Нажатые клавиши: ";
        pressedKeys = getKeyboardState();
        printVector(pressedKeys);
        Sleep(2000);
    }
    _getch();
    return 0;
}



vector<string> getKeyboardState()
{
    int keysNumber = 256;
    int keyNameLength = 20;
    BYTE* keys = new BYTE[keysNumber];
    GetKeyboardState(keys);

    vector<string> pressedKeys;

    for (int i = 0; i < keysNumber; ++i)
    {
        if ((keys[i] & (1 << 7)) == (1 << 7))
        {
            wchar_t* keyName = new wchar_t[keyNameLength];

            int keyCode = MapVirtualKeyW(i, MAPVK_VK_TO_VSC);
            GetKeyNameTextW(keyCode << 16, keyName, keyNameLength);
            string strKeyName = convertWcharToStr(keyName);

            if (find(pressedKeys.begin(), pressedKeys.end(), strKeyName) == pressedKeys.end())
                pressedKeys.push_back(strKeyName);

            delete[] keyName;
        }
    }
    delete[] keys;

    return pressedKeys;
}

void printVector(vector<string> vec)
{
    int c = 0;
    if (vec.empty())
        cout << "нажатых клавиш нет\n";
    else
    {
        c++;
        for (vector<string>::iterator it = vec.begin(); it < vec.end() - 1; ++it)
            c++;
        cout << c << '\n';
    }
}

string convertWcharToStr(wchar_t* str)
{
    wstring wstr = str;
    return string(wstr.begin(), wstr.end());
}