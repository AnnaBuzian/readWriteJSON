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
#include <typeinfo>

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
        try{
            jsonFromFile << fin;
        }catch(exception &e){
            cerr << "Ошибка чтения JSONа из файла." << endl;
            exit(1);
        }
        if ( ! jsonFromFile[0].empty()){
            for (int it = 0; it < jsonFromFile.size(); it++) {
                for (int i = 0; i < sizeof(ARRAY_KEYS)/sizeof(*ARRAY_KEYS); i++){
                    if (jsonFromFile[it][ARRAY_KEYS[i]].empty()){
                        cerr << "Ошибка: Неправильная структура JSON. В элементе " << it << " нет значения по ключу - " << ARRAY_KEYS[i] << "." << endl;
                        exit(1);
                    }
                }
            }
            
        }else{
            cerr << "Ошибка: JSON пустой.\n";
            exit(1);
        }
    }
    else {
        cerr << "Ошибка: Файл не найден \"" << inputPath << "\"" << endl;
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
            cerr << "Ошибка записи" << endl;
            return 1;
        }
        ostr.close();
    }
    else {
        cerr << "Ошибка открытия выходного файла \"" << inputPath << "\"" << endl;
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
        if (!jsonInputTable[it].is_null()){
            for (int i = 0; i < sizeof(ARRAY_KEYS)/sizeof(*ARRAY_KEYS); i++){
                t.add( jsonInputTable[it][ARRAY_KEYS[i]] );
            }
            t.endOfRow();
        }
    }
    
    t.setAlignment( 2, TextTable::Alignment::RIGHT );
    return t;
};

json searchDataFromJSON(json jsonInput){
    json jsonInputTable = jsonInput;
    json jsonOuputTable;
    string keyElem;
    string valueElem;
    cout << "Введите наименования столбца для поиска \n";
    getline(cin, keyElem);
    cout << endl;
    cout << "Введите искомое значение \n";
    getline(cin, valueElem);
    cout << endl;
    
    jsonOuputTable.clear();
    for (int i = 0; i < jsonInputTable.size(); i++ ){
        if (jsonInputTable[i][keyElem] == valueElem){
            jsonOuputTable[i] = jsonInputTable[i];
        }
    }
    return jsonOuputTable;
};


json editDataJSON(json jsonInput){
    json jsonInputTable = jsonInput;
    json conditionSearch;
    json changeElem;
    string keyElemEdit;
    string valueElemEdit;
    string keyElem;
    string valueElem;
    cout << "Введите наименования столбца для поиска \n";
    getline(cin, keyElem);
    cout << endl;
    cout << "Введите искомое значение \n";
    getline(cin, valueElem);
    cout << endl;
    cout << "Введите наименования столбца для редактирования \n";
    getline(cin, keyElemEdit);
    cout << endl;
    cout << "Введите значение для редактирования \n";
    getline(cin, valueElemEdit);
    cout << endl;
    for (int i = 0; i < jsonInputTable.size(); i++ ){
        if (jsonInputTable[i][keyElem] == valueElem){
            jsonInputTable[i][keyElemEdit] = valueElemEdit;
        }
    }
    return jsonInputTable;
};

int selectOperationJSON(json jsonInput, string pathFile){
    int inputOperation;
    string in_string;
    const char *OPERATION_JSON = getenv("OPERATION_JSON");
    json jsonInputFile = jsonInput;
    
    cout << "Введите, пожалуйста, номер операции, которую нужно выполнить с json (VIEW - 1, search - 2, edit - 3, exit - 0): ";
    getline(cin, in_string);
    if (in_string.empty()) {
        inputOperation = atoi(OPERATION_JSON);
    }else if (cin.fail()){
        cin.clear();
        cerr << "Ошибка ввода!\n";
        exit(1);
    }else{
        inputOperation = atoi(in_string.c_str());
    }
    
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
            selectOperationJSON(editJSON, pathFile);
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
