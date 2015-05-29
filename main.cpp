
//Changed from Danya Sinicin Added Recursive iterator in dir_runner
#include <iostream>
//deleted as we don`t need vectors anymore
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "md5.h"
using namespace std;
using namespace boost::filesystem;
using boost::property_tree::ptree;

//deleted as we don`t need vectors anymore(3)


path main_path_get(){
	cout << "Enter your path" << endl;
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
path json_path_get(){ //moved from directory scanner (14)
	cout << "Enter your Json File Path" << endl;
	path p;
	string s;
	getline(cin, s);
	p = s;
	while (!is_regular_file(p)){
		cout << "Error  " << p << " is not valid Json File path" << endl  //changed to look better and easier
			<< "Enter valid Json File path" << endl;
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
			//deleted string s = p.string();
			current_file.put("File Path", p.string()); //deleted s.erase(0, main_p.string().length())
			pt.push_back(make_pair("", current_file));
		}
	}
}

ptree path_reader(){
	 //moved to main_path_get()
	ptree pt;
	ptree files_array;
	path main_path = main_path_get();
	dir_runner(main_path, files_array);
	pt.add_child("Path Files", files_array);
	return pt;
}
void directory_scanner(){
	int changes=0;//new
	//deleted as we don`t need vectors anymore(3)
	path main_path = main_path_get();
	path json_path = json_path_get();
	//moved to json_path get()
	ptree Json_file;
	read_json(json_path.string(), Json_file);
	try {
		for (recursive_directory_iterator dir_itr(main_path);
			dir_itr != recursive_directory_iterator();
			++dir_itr)
		{
			if (is_regular_file(*dir_itr)){
				path p = *dir_itr;
				//deleted string s = p.string();
				//not rly needed string name = p.filename().string();
				//deleted s = s.erase(0, main_path.string().length());
				int k = 0;
				for (auto &v : Json_file.get_child("Path Files")) {
					if (v.second.get<string>("File Name") != "was") //changed was to non caps
						if (p.string() == v.second.get<string>("File Path"))
							if (p.filename().string() == v.second.get<string>("File Name")) {
						if (md5constructor(p) != v.second.get<string>("File Hash"))
							cout << p.filename().string() << " was changed" << endl; changes++; //changed to be readable
							k = 1; //changed to be readable
							v.second.put<string>("File Name", "was"); //changed to be readable
							break; //changed to be readable
						//changed was to non caps
							}
				}
				if (k == 0) cout << p.filename().string() << " is a new file" << endl; changes++;//changed name to p.filename().string()
			}
		}
		for (auto &v : Json_file.get_child("Path Files")){
			if (!exists(v.second.get<string>("File Path"))) cout << v.second.get<string>("File Name") << " was deleted" << endl; changes++; //changed was to non caps,and added exists
		}
	}
	catch (...) { cout << "\nError! \nDo not rename files and folders in your path while programm is working!"; } //changed to look better and easier
	//deleted as we don`t need vectors anymore(6)
	if(changes==0)cout << "Directory has no changes" << endl; //new
}




int main(){
	int k = 0; //new
	cout << "File Searcher Porgramm" << endl;	//new
	while (k != 1 || k != 2){	//added while cycle
	cout << "To make a new Json File input 1" << endl;	//changed to look better and easier
	cout<<"To scan Directory for changes input 2"<<endl;	//same
	cin >> k;
	if (k == 1 || k == 2) break; //new
	cout << "Bad input,try again" << endl; //new
}	//end of a cycle
	if (k == 1)
		write_json("output.json", path_reader());
	else if (k==2)
		directory_scanner();
	cout << "end";
	cin.get();
	return 0;
}
//total count of changed strings - 54
