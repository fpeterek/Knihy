//
//  FileHandler.hpp
//  opakovani
//
//  Created by Filip Peterek on 05.09.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#ifndef FileHandler_hpp
#define FileHandler_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


/************************************************************************************************************************************/
/*                                                                                                                                  */
/*                                                         ----------------                                                         */
/*                                                         -- Exceptions --                                                         */
/*                                                         ----------------                                                         */
/*                                                                                                                                  */
/************************************************************************************************************************************/


class FileNotOpenException : public std::exception{
    
    std::string _errorMessage;
    
public:
    
    FileNotOpenException(std::string filename);
    const char * what();
    
};

class FileNotClosedException : public std::exception {
    
    std::string _errorMessage;
    
public:
    
    FileNotClosedException(std::string filename);
    const char * what();
    
};

/************************************************************************************************************************************/
/*                                                                                                                                  */
/*                                                            ----------                                                            */
/*                                                            -- Item --                                                            */
/*                                                            ----------                                                            */
/*                                                                                                                                  */
/************************************************************************************************************************************/

enum class ISBN {
    
    Isbn_10,
    Isbn_13,
    None
    
};

struct Item {
    
    std::string id, name, format, year, weight, isbn;
    Item(std::vector<std::string> &);
    
};

/************************************************************************************************************************************/
/*                                                                                                                                  */
/*                                                       -----------------                                                          */
/*                                                       -- FileHandler --                                                          */
/*                                                       -----------------                                                          */
/*                                                                                                                                  */
/************************************************************************************************************************************/


class FileHandler {
    
    std::ifstream _input;
    std::string _filename;
    std::vector<Item> _items;
    
    
public:
    
    void OpenFile(std::string filename);
    void ReadFile();
    void CloseFile();
    
    unsigned long ItemCount();
    unsigned long BookCount();
    unsigned long DvdCount();
    unsigned int BookWeight();
    void EditIsbn();
    Item NewestBook();
    static ISBN CheckIsbn(Item &);
    
    void ParseLine(std::string &);
    void OutputToHtml();
    
};

#endif /* FileHandler_hpp */