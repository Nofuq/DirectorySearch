
//Changed from Danya Sinicin Added Recursive iterator in dir_runner
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "md5.h"
using namespace std;
using namespace boost::filesystem;
using boost::property_tree::ptree;
void show(string a){
cout << a << endl;

}


path main_path_get(){
	path p;
	string s;
	getline(cin, s);
	getline(cin, s);
	p = s;
	while (!is_directory(p)){
		cout << "Error: " << p << " is not valid path" << endl
			<< "Enter valid path" << endl;
		getline(cin, s);
		p = s;
	}
	return p;
}

string md5constructor(path p){
	ifstream o;

	o.open(p.string(), ifstream::in);
	string text;
	while (!o.eof()){
		text += o.get();
	}
	string a = md5(text);
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
			current_file.put("File Hash", md5constructor(p));
			current_file.put("File Size", file_size(*dir_itr));
			string s = p.string();
			current_file.put("File Path", s.erase(0, main_p.string().length()));
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
	pt.add_child("Path Files", files_array);
	return pt;
}
void directory_scanner(){
	vector<string> news;
	vector<string> deleted;
	vector<string> changed;
	cout << "Enter your Folder Path" << endl;
	path main_path = main_path_get();
	cout << "Enter your Json File Path" << endl;
	path p;
	string s;
	getline(cin, s);
	p = s;
	while (!is_regular_file(p)){
		cout << "ERROR  " << p << " is not valid Json File path" << endl
			<< "Enter valid Json File path" << endl;
		getline(cin, s);
		p = s;
	}
	ptree Json_file;
	read_json(s, Json_file);
	try {
		for (recursive_directory_iterator dir_itr(main_path);
			dir_itr != recursive_directory_iterator();
			++dir_itr)
		{
			if (is_regular_file(*dir_itr)){
				path p = *dir_itr;
				string s = p.string();
				string name = p.filename().string();
				s = s.erase(0, main_path.string().length());
				int k = 0;
				for (auto &v : Json_file.get_child("Path Files")) {
					if (v.second.get<string>("File Name") != "WAS")
						if (s == v.second.get<string>("File Path"))
							if (p.filename().string() == v.second.get<string>("File Name")) {
						if (md5constructor(p) != v.second.get<string>("File Hash")) changed.push_back(name);
						k = 1; v.second.put<string>("File Name", "WAS");  break;
							}
				}
				if (k == 0) news.push_back(name);
			}
		}
		for (auto &v : Json_file.get_child("Path Files")){
			if (v.second.get<string>("File Name") != "WAS")  deleted.push_back(v.second.get<string>("File Name"));
		}
	}
	catch (...) { cout << "\nERROR! \nDON`T RENAME FILES AND FOLDERS IN YOUR PATH WHILE PROGRAMM IS WORKING! "; }

	if (changed.size() != 0)cout << "Changed Files:" << endl;
	for_each(changed.begin(), changed.end(), show);
	if (news.size() != 0)cout << "New Files:" << endl;
	for_each(news.begin(), news.end(), show);
	if (deleted.size() != 0)cout << "Deleted Files:" << endl;
	for_each(deleted.begin(), deleted.end(), show);
	if ((changed.size() == 0) && (news.size() == 0) && deleted.size() == 0) cout << "No Changes" << endl;
}




int main(){
	int k;
	cout << "0-Make new Json File \n1-Scan Directory for changes\n";
	cin >> k;
	if (k == 0)
		write_json("output.json", path_reader());
	else
		directory_scanner();
	cout << "end";
	cin.get();
	return 0;
}
