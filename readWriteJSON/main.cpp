//
//  main.cpp
//  readWriteJSON
//
//  Created by Anna Buzian on 27.01.17.
//  Copyright © 2017 Anna Buzian. All rights reserved.
//
#include <limits.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

// JSON parser
#include "src/json.hpp"

// JSON Table
#include "src/TextTable.h"


using namespace std;
using json = nlohmann::json;

const string ARRAY_KEYS[5] = {"rep_date", "agreement", "recipient", "sender", "summ"};

json readFromFile(string pathFile){
    string inputPath = pathFile;
    json jsonFromFile;
    
    fstream fin(inputPath);
    if (fin) {
        jsonFromFile << fin;
        if ( ! jsonFromFile[0].empty() ){
            for (int it = 0; it < jsonFromFile.size(); it++) {
                for (int i = 0; i < sizeof(ARRAY_KEYS)/sizeof(*ARRAY_KEYS); i++){
                    if (jsonFromFile[it][ARRAY_KEYS[i]].empty()){
                        cerr << "Ошибка: Неправильная структура JSON. В элементе " << it << " нет значения по ключу - " << ARRAY_KEYS[i] << "." << endl;
                        exit(1);
                    }
                }
            }
            
        }else{
            cerr << "Ошибка. Файл пустой, либо неправильная структура JSON.\n";
            exit(1);
        }
    }
    else {
        cerr << "Ошибка открытия входящего файла \"" << inputPath << "\"" << endl;
        exit(1);
    }
    return jsonFromFile;
};

int writeToFile(string pathFile, json jsonInput){
    string inputPath = pathFile;
    ofstream ostr;
    ostr.open(inputPath);
    if (ostr) {
        ostr << jsonInput;
        if (ostr.bad()) {
            cerr << "Unrecoverable write error" << endl;
            return 1;
        }
        ostr.close();
    }
    else {
        cerr << "Output file open error \"" << inputPath << "\"" << endl;
        return 1;
    }
    return 0;
};

TextTable viewJSON(json jsonInput){
    json jsonInputTable = jsonInput;
    TextTable t( '-', '|', '+' );
    for (int i = 0; i < sizeof(ARRAY_KEYS)/sizeof(*ARRAY_KEYS); i++){
        t.add( ARRAY_KEYS[i] );
    }
    t.endOfRow();
    for (int it = 0; it < jsonInputTable.size(); it++) {
        for (int i = 0; i < sizeof(ARRAY_KEYS)/sizeof(*ARRAY_KEYS); i++){
            t.add( jsonInputTable[it][ARRAY_KEYS[i]] );
        }
        t.endOfRow();
    }
    
    t.setAlignment( 2, TextTable::Alignment::RIGHT );
    return t;
};

json searchDataFromJSON(json jsonInput){
    json jsonInputTable = jsonInput;
    json conditionSearch;
    cout << "Введите условия для поиска.\n";
    for (int i = 0; i < sizeof(ARRAY_KEYS)/sizeof(*ARRAY_KEYS); i++){
        cout << "Условие (==, <=, <, >, >=, match) для поля " << ARRAY_KEYS[i] << ": ";
        cout << cin.get() << endl;
        if (cin.get() != '\n') {
            cin >> conditionSearch[ARRAY_KEYS[i]]["condition"];
            cin.clear();
        }
        cout << endl;
        cout << "Значение для поля " << ARRAY_KEYS[i] << ": ";
        cout << cin.get() << endl;
        if (cin.get() != '\n') {
            cin >> conditionSearch[ARRAY_KEYS[i]]["value"];
            cin.clear();
        }
        cout << endl;
    }
    
//    jsonInput.find("rep_date").;
    cout << conditionSearch << endl;
    
    return jsonInputTable;
};


json editDataJSON(json jsonInput){
    json jsonInputTable = jsonInput;
    return jsonInputTable;
};

int selectOperationJSON(json jsonInput, string pathFile){
    int inputOperation;
    string in_string;
    const char *OPERATION_JSON = getenv("OPERATION_JSON");
    json jsonInputFile = jsonInput;
    
    cout << "Введите, пожалуйста, номер операции, которую нужно выполнить с json (VIEW - 1, search - 2, edit - 3, exit - 0): ";
    cin >> in_string;
    if (cin.get() == '\n'){
        inputOperation = atoi(OPERATION_JSON);
    }else if (cin.fail()){
        cin.clear();
        cerr << "Ошибка ввода!\n";
        exit(1);
    }
    inputOperation = atoi(in_string.c_str());
    
    switch ( inputOperation )
    {
        case 0:
            exit(1);
        case 1:
            cout << viewJSON(jsonInputFile) << endl;
            selectOperationJSON(jsonInputFile, pathFile);
            break;
        case 2:{
            json searchJSON = searchDataFromJSON(jsonInputFile);
            cout << viewJSON(searchJSON) << endl;
            selectOperationJSON(jsonInputFile, pathFile);
            break;}
        case 3:{
            json editJSON = editDataJSON(jsonInputFile);
            cout << viewJSON(editJSON) << endl;
            writeToFile(pathFile, editJSON);
            selectOperationJSON(jsonInputFile, pathFile);
            break;}
        default:
            cerr << "Ошибка: Введен номер несуществующей операции!\n";
            exit(1);
    }
    
    return 0;
}

int main(int argc, char** argv)
{
    string pathFile = argc != 2 ? "" : argv[1];
    
    if (argc < 2)
    {
        return EXIT_FAILURE;
    }
    
    json jsonString = readFromFile(pathFile);
    selectOperationJSON(jsonString, pathFile);
    return 0;
};
