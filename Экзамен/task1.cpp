#include <iostream>
#include <windows.h>
#include <string>
#include <clocale>
#include <conio.h>

using namespace std;



wstring ConvertStrToWstr(string str);
double GetFreeDiskSpace(LPCWSTR disk_letter);
double GetUsedDiskSpace(LPCWSTR disk_letter);
double ConvertBytesToGigabytes(double bytes);

int main()
{
    setlocale(LC_ALL, "russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int ALPHABET_SIZE = 26, ARRAY_SIZE = 30;
    int drives = GetLogicalDrives();

    for (int i = 0, pointer = 1; i < ALPHABET_SIZE; ++i, pointer = pointer << 1)
    {
        if ((drives & pointer) != 0)
        {
            wchar_t* volume_name = new wchar_t[ARRAY_SIZE];
            wchar_t* file_system_name = new wchar_t[ARRAY_SIZE];
            unsigned long serial_number;
            wstring disk_letter = ConvertStrToWstr((char)('A' + i) + (string)":\\");
            GetVolumeInformationW(disk_letter.c_str(), volume_name, sizeof(volume_name)//измени команду
                * ARRAY_SIZE, &serial_number, 0, 0, file_system_name,
                sizeof(file_system_name) * ARRAY_SIZE);
            cout << "Диск: " << (char)('A' + i) << '\n';
            cout << "Занятое пространство: " << GetUsedDiskSpace(disk_letter.c_str())
                << " гигабайт\n";
            delete[] volume_name;
            delete[] file_system_name;
        }
    }
    _getch();
    return 0;
}


wstring ConvertStrToWstr(string str)
{
    return wstring(str.begin(), str.end());
}

double GetUsedDiskSpace(LPCWSTR disk_letter)
{
    DWORD sectors_per_cluster, bytes_per_sector, free_clusters_number,
        clusters_total_number;
    GetDiskFreeSpaceW(
        disk_letter,
        &sectors_per_cluster,
        &bytes_per_sector,
        &free_clusters_number,
        &clusters_total_number);

    return ConvertBytesToGigabytes(double(sectors_per_cluster) * bytes_per_sector
        * clusters_total_number) - GetFreeDiskSpace(disk_letter);
}

double ConvertBytesToGigabytes(double bytes)
{
    return bytes / (double(1024) * 1024 * 1024);
}

double GetFreeDiskSpace(LPCWSTR disk_letter)
{
    DWORD sectors_per_cluster, bytes_per_sector, free_clusters_number,
        clusters_total_number;
    GetDiskFreeSpaceW(
        disk_letter,
        &sectors_per_cluster,
        &bytes_per_sector,
        &free_clusters_number,
        &clusters_total_number);

    return ConvertBytesToGigabytes(double(sectors_per_cluster) * bytes_per_sector
        * free_clusters_number);
}