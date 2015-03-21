#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "sha1.h"


using namespace boost::filesystem;

void main(int argc, char* argv[1])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::string dir_path = "C:\\";
	std::cout << "Enter the directory path:\n" << std::endl;


	try
	{
		std::cin >> dir_path;
		path path = dir_path;
		if (!is_directory(path))
			throw "\nIncorrect path, try again!\n\n";
	}
	catch (const char* msg)
	{
		std::cout << msg;
		system("pause");
		std::exit(1);
	}


	std::ofstream fout("result.csv");//creating CSV 
	fout << "Path" << ";" << "Name" << ";" << "Size" << ";" << "Hash" << std::endl;


	path path = dir_path;
	recursive_directory_iterator itr(path);
	while (itr != recursive_directory_iterator())
	{
		if (!is_directory(itr->status()))
		{	/*
			unsigned char hash[20];
			char hexstring[41];
			int length = strlen(itr->path().string().c_str());
			sha1::calc(itr->path().string().c_str(), length, hash);
			sha1::toHexString(hash, hexstring);
			*/
			fout << itr->path().string() << "; " << itr->path().filename() << "; " << file_size(itr->path()) << std::endl;
			++itr;
		}
		else
		{
			itr = ++itr;
		}
	}
}
