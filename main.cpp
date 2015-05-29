//Changed from Danya Sinicin Added Recursive iterator in dir_runner
#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/serialization/serialization.hpp> //was included two times
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "sha256.h"
#include <boost/serialization/vector.hpp>
//deleted not needed include

using namespace std;
using namespace boost::filesystem;


class Filex{
public:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & size;
		ar & path;
		ar & hash;
	}

	string name;
	string size;
	string path;
	string hash;


	Filex(){};
	Filex(std::string a, string b, string d, string c) {
		name = a;
		size = b;
		path = d;
		hash =c;
	}
	

};

void show(Filex a){
	if (!exists(a.path)) cout << a.path << " deleted" << endl;
	else cout << a.path <<endl;
}

bool isEqual(Filex a, Filex b){
	if ((a.name == b.name) && (a.size == b.size) && (a.path == b.path) && (a.hash == b.hash)) return 1;
	else return 0;
}

bool Changed(Filex a, Filex b){
	if ((a.name == b.name) && (a.path == b.path) && (a.hash != b.hash)) return 1;
	else return 0;
}

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
string sha256constructor(path p){
	ifstream o;

	o.open(p.string(), ifstream::in);
	string text;
	while (!o.eof()){
		text += o.get();
	}
	string a = sha256(text);
	o.close();
	return a;
};

path file_path_get(){
	cout << "Enter File path" << endl;
	path p;
	string s;
	getline(cin, s);
	p = s;
	while (!is_regular_file(p)){
		cout << "Error: " << p << " is not valid File path" << endl
			<< "Enter valid path" << endl;

		getline(cin, s);
		p = s;
	}
	return p;
}

void dir_runner(int inp){ //changed to have input value
	//moved to main(4)
	path main_p = main_path_get();
	vector<Filex>filexs;
	for (recursive_directory_iterator dir_itr(main_p); dir_itr != recursive_directory_iterator();++dir_itr)
	{
		if (is_regular_file(*dir_itr)){
			path p = *dir_itr;
			
				string name = p.filename().string();
				string size = to_string(file_size(p));
				string path = p.string();
				string hash = sha256constructor(p);
				filexs.push_back(Filex(name, size, path, hash));

			
		}
	}
	if (inp == 2) {
		path fpth; //moved to not make clear path in case of inp==1
		fpth = file_path_get(); //moved to not have 2 if(inp==2) (3)
		vector<Filex> news;
		//didnt actually need a deleted vector
		vector<Filex> out; 
		ifstream i(fpth.string());
		//moved 2 rows higher to have both vectors at same place
		boost::archive::text_iarchive ia(i);
		ia & out;
		for (int i1 = 0; i1 < filexs.size(); i1++){
			bool deleted = false;
			bool changed = false;
			for (int j1 = 0; j1 < out.size(); j1++){
				int del = 0;
				deleted = isEqual(filexs[i1], out[j1]);
				changed = Changed(filexs[i1], out[j1]);
				 //ch equals changed rly(not needed(4))
				if (deleted == true){ //same for deleted(3)
					out.erase(out.begin() + j1);
				}
			}
			if (deleted == false && changed == false){
				news.push_back(filexs[i1]);
			}
		}

		if (out.size() != 0){
		cout << "Changed Files" << endl;
		for_each(out.begin(), out.end(), show);
		}
		else if (news.size() != 0){
			cout << "New Files" << endl;
			for_each(news.begin(), news.end(), show);
		}
		else {
			cout << "No Changes" << endl;
			i.close();
		} //not needed so much else-ifs (6)
	}
	
	ofstream o("output.txt");
		boost::archive::text_oarchive oa(o);
		oa & filexs;
		o.close();
	
}

int main(){
	int inp=0;
	cout << "File Searcher Porgramm" << endl;	//new
	while (inp != 1 || inp != 2){	//added while cycle
	cout << "To make a new File input 1" << endl;	//changed to look better and easier
	cout << "To scan Directory for changes input 2" << endl;	//same
	cin >> inp;
	if (inp == 1 || inp == 2) break; //new
	cout << "Bad input,try again" << endl; //new
}	//end of a cycle
	dir_runner(inp); //input value
	system("pause");
	return 0;
}
