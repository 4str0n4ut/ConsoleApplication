#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "sha1.h"

using namespace boost::filesystem;

void fchange(std::fstream & file, std::vector <std::string> & vec) 
{
	std::string temp;
	while (!file.eof())
	{
		getline(file, temp);
		int k = 0;
		std::string dir = "";
		std::string myhash = "";
		for (int i = 0; i<temp.size(); i++)
		{
			if (temp[i] == ';')
			{
				k++;
				continue;
			}
			switch (k)
			{
			case 0:
				dir = dir + temp[i];
				break;
			case 3:
				myhash = myhash + temp[i];
				break;
			default:
				break;
			}
		}
		if (dir == "" || myhash == "")
		{
			break;
		}
		vec.push_back(dir);
		if (!exists(dir))
		{
			std::cout << "The file has been deleted or moved to another folder: " << dir << std::endl;
		}
		boost::filesystem::path mp(dir);
		std::string time = std::to_string(last_write_time(mp.string()));
		unsigned char hash[20];
		char hexstring[41];
		int length = strlen(mp.string().c_str());
		int length1 = strlen(time.c_str());
		sha1::calc((mp.string() + time).c_str(), length + length1, hash);
		sha1::toHexString(hash, hexstring);
		if (exists(dir) && (hexstring != myhash))
		{
			std::cout << "The file has been modified: " << dir << std::endl;
		}

	}
}

void wchange(std::fstream & file, const std::vector <std::string> & vec, std::string & directory)
{
	path mypath(directory);
	directory_iterator end_itr;
	for (directory_iterator itr(mypath); itr != end_itr; ++itr)
	{
		if (itr->path().string() == (directory))
		{
		continue;
		}
		if (is_directory(itr->status()))
		{
			std::string temp = itr->path().string();
			wchange(file, vec, temp);
		}
		else
		{
			bool existence = false;
			for (int i = 0; i < vec.size(); i++)
			{
				if (itr->path().string() == vec[i])
				{
					existence = true;
					break;
				}
			}
			if (!existence)
			{
				std::cout << "The file has been created or added from another folder: " << itr->path().string() << std::endl;
			}

			unsigned char hash[20];
			char hexstring[41];
			std::string time = std::to_string(last_write_time(itr->path().string()));
			int length = strlen(itr->path().string().c_str());
			int length1 = strlen(time.c_str());
			sha1::calc((itr->path().string() + time).c_str(), length+length1, hash);
			sha1::toHexString(hash, hexstring);

			file << itr->path().string() << ";" << itr->path().filename() << ";" << file_size(itr->path()) << ";" << hexstring << std::endl;
		}
	}
}


void main(int argc, char* argv[1])
{
	setlocale(LC_ALL, "Russian");
	std::string dir_path;

	if (!exists("result.csv"))
	{
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
		path path = dir_path;
		recursive_directory_iterator itr(path);
		while (itr != recursive_directory_iterator())
		{
			if (!is_directory(itr->status()))
			{
				std::string time = std::to_string(last_write_time(itr->path().string()));
				unsigned char hash[20];
				char hexstring[41];
				int length = strlen(itr->path().string().c_str());
				int length1 = strlen(time.c_str());
				sha1::calc((itr->path().string() + time).c_str(), length+length1, hash);
				sha1::toHexString(hash, hexstring);


				fout << itr->path().string() << ";" << itr->path().filename() << ";" << file_size(itr->path()) << ";" << hexstring << std::endl;
				++itr;
			}
			else
			{
				itr = ++itr;
			}
		}
		fout.close(); //closing CSV
		std::cout << "\n" << "result.csv in project current directory" << std::endl;
		system("pause");
	}
	else
	{
		std::vector <std::string> vec;
		std::cout << "result.csv has been created already." << std::endl;
		std::cout << "Please, enter the path of the previous scan: \n" << std::endl;
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

		std::cout << "\n \n ...SEARCHING FOR CHANGES... \n \n" << std::endl;
		std::fstream file("result.csv", std::ios_base::in);
		fchange(file, vec);
		file.close();
		file.open("result.csv", std::ios_base::out);
		wchange(file, vec, dir_path);
		file.close();
		std::cout << std::endl << "Updated: " << std::endl << "result.csv" << std::endl;
		system("pause");
	}
}