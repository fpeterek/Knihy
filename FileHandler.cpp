//
//  FileHandler.cpp
//  opakovani
//
//  Created by Filip Peterek on 05.09.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#include "FileHandler.hpp"


/************************************************************************************************************************************/
/*                                                                                                                                  */
/*                                                         ----------------                                                         */
/*                                                         -- Exceptions --                                                         */
/*                                                         ----------------                                                         */
/*                                                                                                                                  */
/************************************************************************************************************************************/


FileNotOpenException::FileNotOpenException(std::string filename) {
    
    _errorMessage = "Soubor " + filename + " se nepodařilo otevřít. ";
    
}

const char * FileNotOpenException::what() {
    
    return _errorMessage.c_str();
    
}

FileNotClosedException::FileNotClosedException(std::string filename) {
    
    _errorMessage = "Soubor " + filename + " se nepodařilo zavřít. ";
    
}

const char * FileNotClosedException::what() {
    
    return _errorMessage.c_str();
    
}


/************************************************************************************************************************************/
/*                                                                                                                                  */
/*                                                            ----------                                                            */
/*                                                            -- Item --                                                            */
/*                                                            ----------                                                            */
/*                                                                                                                                  */
/************************************************************************************************************************************/


Item::Item(std::vector<std::string> & params) {
    
    id      = params[0];
    name    = params[1];
    format  = params[2];
    year    = params[3];
    weight  = params[4];
    isbn    = params[5];
    
}


/************************************************************************************************************************************/
/*                                                                                                                                  */
/*                                                       -----------------                                                          */
/*                                                       -- FileHandler --                                                          */
/*                                                       -----------------                                                          */
/*                                                                                                                                  */
/************************************************************************************************************************************/


void FileHandler::OpenFile(std::string filename) {
    
    _input.open(filename);
    _filename = filename;
    
    if (not _input.is_open()) {
        
        throw FileNotOpenException(filename);
        
    }
    
}

void FileHandler::ReadFile() {
    
    std::string line;
    std::getline(_input, line);
    
    while (not _input.eof()) {
        
        std::getline(_input, line);
        ParseLine(line);
        
    }
    
    EditIsbn();
    
}

void FileHandler::CloseFile() {
    
    _input.close();
    
    if (_input.is_open()) {
        
        throw FileNotClosedException(_filename);
        
    }
    
}

void FileHandler::ParseLine(std::string & line) {
    
    std::vector<std::string> tokens;
    std::string token;
    size_t position = 0;
    
    while ((position = line.find(";")) != std::string::npos) {
        
        token = line.substr(0, position);
        tokens.emplace_back(token);
        line.erase(0, position + 1);
        
    }
    
    if (tokens.size() == 6) _items.emplace_back(tokens);
    
}

unsigned long FileHandler::ItemCount() {
    
    return _items.size();
    
}

unsigned long FileHandler::BookCount() {
    
    unsigned long books = 0;
    
    for (auto & i : _items) {
        
        if (i.format == "Kniha") ++books;
        
    }
    
    return books;
    
}

unsigned long FileHandler::DvdCount() {
    
    unsigned long dvds = 0;
    
    for (auto & i : _items) {
        
        if (i.format == "DVD") ++dvds;
        
    }
    
    return dvds;
    
}

unsigned int FileHandler::BookWeight() {
    
    unsigned int weight;
    
    for (auto & i : _items) {
        
        try {
            
            if (i.format == "Kniha") weight += std::stoi(i.weight);
            
        }
        catch (std::invalid_argument e) { std::cerr << "Neplatná hodnota u objektu " << i.name << std::endl; continue; }
        
    }
    
    return weight;
    
}

Item FileHandler::NewestBook() {
    
    Item item = _items[0];
    
    for (auto & i : _items) {
        
        if (i.format == "Kniha" and i.year > item.year) item = i;
        
    }
    
    return item;
    
}

void FileHandler::EditIsbn() {
    
    std::string isbnStr;
    size_t pos = 0;
    
    for (auto & i : _items) {
        
        isbnStr = i.isbn;
        
        while ( (pos = isbnStr.find("-")) != std::string::npos) {
            
            isbnStr.replace(pos, 1, "");
            
        }
        
        i.isbn = isbnStr;
        
    }
    
}

ISBN FileHandler::CheckIsbn(Item & i) {
    
    if (i.isbn.length() == 10) return ISBN::Isbn_10;
    else if (i.isbn.length() == 13) return ISBN::Isbn_13;
    
    return ISBN::None;
    
}

void FileHandler::OutputToHtml() {
    
    std::stringstream output;
    
    output << "<html lang=\"cs\"><head><meta charset=\"UTF-8\" /><title>Seznam knih</title></head><body>";
    output << "<h1>ISBN 10</h1>";
    output << "<table><thead><td>číslo</td><td>název</td><td>nosič</td><td>rok vydání</td><td>ISBN</td></thead><tbody>";
    
    for (auto & i : _items) {
        
        if (FileHandler::CheckIsbn(i) == ISBN::Isbn_10) {
            output << "<tr>";
            output << "<td>" << i.id << "</td>";
            output << "<td>" << i.name << "</td>";
            output << "<td>" << i.format << "</td>";
            output << "<td>" << i.year << "</td>";
            output << "<td>" << i.weight << "</td>";
            output << "<td>" << i.isbn << "</td>";
            output << "</tr>";
        }
        
    }
    
    output << "</tbody></table><h1>ISBN 13</h1>";
    output << "<table><thead><td>číslo</td><td>název</td><td>nosič</td><td>rok vydání</td><td>ISBN</td></thead><tbody>";
    
    for (auto & i : _items) {
        
        if (FileHandler::CheckIsbn(i) == ISBN::Isbn_13) {
            output << "<tr>";
            output << "<td>" << i.id << "</td>";
            output << "<td>" << i.name << "</td>";
            output << "<td>" << i.format << "</td>";
            output << "<td>" << i.year << "</td>";
            output << "<td>" << i.weight << "</td>";
            output << "<td>" << i.isbn << "</td>";
            output << "</tr>";
        }
        
    }
    
    output << "</tbody></table></body></html>";
    
    std::ofstream outputFile;
    outputFile.open("vypis.html");
    if (not outputFile.is_open()) throw FileNotOpenException("vypis.html");
    outputFile << output.str();
    std::cout << "Byl vytvořen soubor vypis.html" << std::endl;
    outputFile.close();
    if (outputFile.is_open()) throw FileNotClosedException("vypis.html");
    
}