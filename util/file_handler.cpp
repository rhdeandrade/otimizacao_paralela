#ifndef file_handler_h
#define file_handler_h

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class FileHandler {

  public:
    std::vector<string> open_file(string file_name);

};

std::vector<string> FileHandler::open_file(string file_name) {
  
  fstream myfile;
  std::vector<string> dados;
  string line;

  //cout << "Open File: " << file_name << "\n";

  myfile.open(file_name.c_str(), fstream::in); 

  if (myfile.is_open()) {
    
    while (!myfile.eof()) {
      
      getline(myfile, line);
      
      if (!line.empty()) {
        dados.push_back(line);
      }
    }

    myfile.close();
  }
  else {
    cout << "Arquivo não encontrado\n";
  }

  return dados;

}

#endif