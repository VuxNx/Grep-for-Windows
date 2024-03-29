#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;


bool _is_appropriate_ = true;
char driver_list[100];
DWORD driver_count = 0;

void check_version_ ();
void scan_and_print();
void walk_to_driver(string);
void walk_deep (const fs::path&, string);

int main()
{
    check_version_ ();
    if (_is_appropriate_){
        scan_and_print();
        string key_word;
        cout << "Your key word: "; cin >> key_word;
        walk_to_driver(key_word);
    }
    return EXIT_SUCCESS;     
}

void check_version_ ()
{
    string string_year = to_string (__cplusplus);
    string string_ver = string_year.substr (2, 2);
    int _ver_ = stoi (string_ver);
    if (_ver_ < 17)
    {
        cout << "Your current version is: " << _ver_ << ". It is not suitable for this program.\n";
        _is_appropriate_ = false;
    }
    else
        cout << "Your current version is: " << _ver_ << ". It is suitable for this program.\n";

}

void scan_and_print()
{
    driver_count = GetLogicalDriveStrings (100, driver_list);
    if (driver_count == 0)
    {
        cerr << "Error: " << GetLastError();
    }
    else
    {
        cout << "Success to access to physical drivers in computer !!\n";
    }
}

void walk_deep (const fs::path& dir, string key_word)
{
    if (fs::is_directory(dir)){
        try{
            for (const auto& entry : fs::directory_iterator(dir)){
                walk_deep (entry.path(), key_word);
            }
        }
        catch (const fs::filesystem_error& e){
            cerr << "One folder cannot access because of some reasons! \n";
        }
    }
    if (fs::is_regular_file(dir)){
        fstream file_temp (dir);
        stringstream buffer;
        buffer << file_temp.rdbuf();
        if (buffer.str().find(key_word) != string::npos){
            cout << dir << endl;
        }
    }
}

void walk_to_driver(string key_word)
{
    int turn_to_walk = 1;
    while (turn_to_walk <= driver_count/4)
    {
        try{
            char make_dir[4];
            make_dir[0]= driver_list[(turn_to_walk-1)*4];
            make_dir[1]=':';
            make_dir[2]='\\';
            fs::path dir= make_dir;
            walk_deep (make_dir, key_word);
            cout<< "\n---------------------------------------\n";
            turn_to_walk ++;
            
        }
        catch (...){
            cerr<<"Have Error in this driver !\n";
            
        }
        
    }

}

