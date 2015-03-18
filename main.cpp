//Changed from Danya Sinicin Added Recursive iterator in dir_runner
#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "md5.h"

using namespace std;
using namespace boost::filesystem;
using boost::property_tree::ptree;

path main_path_get(){
	path p;
	string s;
	getline(cin, s);
	p = s;
	while (!is_directory(p)){
		cout << "Error: " << p << " is not valid path"<< endl
			<<"Enter valid path" << endl ;
		getline(cin, s);
		p = s;
	}
	return p;
} 

string md5constructor(path p){
ifstream o;

o.open(p.string(),ifstream::in);
				string text;
				while(!o.eof()){
				text+=o.get();
				}
string a=md5(text);
o.close();
return a;
};

void dir_runner(path main_p, ptree &pt){
	
	for (recursive_directory_iterator dir_itr(main_p);
		dir_itr != recursive_directory_iterator();
		++dir_itr)
	{
		if (is_regular_file(*dir_itr)){
				path p = *dir_itr;
				
				ptree current_file;
				current_file.put("File Name", p.filename().string());
				current_file.put("File Size (Bytes) ", file_size(p) );
				current_file.put("File Path", p.string());
				current_file.put("File Hash Summ MD5 format ",md5constructor(p));
				pt.push_back(make_pair("", current_file));
		}
	}
}

ptree path_reader(){
	cout << "Enter your path" << endl;
	ptree pt;
	ptree files_array;
	path main_path = main_path_get();
	dir_runner(main_path, files_array);
	pt.add_child( "Path ( " + main_path.string() + " )  Files " , files_array);
	return pt;
}


int main(){
	setlocale(LC_ALL, "Russian");
	write_json("Directory_Files.json", path_reader()); 
	system("pause");
	return 0;
}