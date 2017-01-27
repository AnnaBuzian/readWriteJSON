//
//  main.cpp
//  readWriteJSON
//
//  Created by Anna Buzian on 27.01.17.
//  Copyright © 2017 Anna Buzian. All rights reserved.
//

#include <string>
#include <iostream>
#include "src/json/value.h"
#include "src/json/reader.h"
#include <fstream>

int main()
{

    std::fstream ifile("/Users/annabuzian/Desktop/Project/readWriteJSON/readWriteJSON/fileStorage/payments.json");
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(ifile, root)) {
        const Json::Value arrayDest = root["dest"];
        for (unsigned int i = 0; i < arrayDest.size(); i++) {
            if (!arrayDest[i].isMember("name"))
                continue;
            std::string out;
            out = arrayDest[i]["name"].asString();
            std::cout << out << "\n";
        }
    }
//using namespace std;
//
//    ifstream fin;
//    ofstream fout;
//    string input;
//    
//    Json::Value object_from_file;
//    Json::Reader reader;
//    bool parsingSuccessful = reader.parse("/Users/annabuzian/Desktop/Project/readWriteJSON/readWriteJSON/fileStorage/payments.txt", object_from_file);
//    if ( !parsingSuccessful )
//    {
//        // report to the user the failure and their locations in the document.
//        std::cout  << "Failed to parse configuration\n"
//        << reader.getFormattedErrorMessages();
//        return 0;
//    }
//    fin.open("/Users/annabuzian/Desktop/Project/readWriteJSON/readWriteJSON/fileStorage/payments.txt");
//    cout << fin.fail() << endl;
//    
//    if(fin.fail())
//        cout << "File failed to open." << endl;
//    fin >> input;
//    
//    fout.open("fileStorage/outputFile.txt");
//    
//    fout << input;
//    return 0;
    
}
