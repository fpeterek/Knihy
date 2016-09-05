//
//  main.cpp
//  opakovani
//
//  Created by Filip Peterek on 05.09.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#include "FileHandler.hpp"


int main(int argc, const char * argv[]) {

    FileHandler fh;
    
    try { fh.OpenFile("knihy.txt"); }
    catch (FileNotOpenException e) { std::cout << e.Message() << std::endl; return -1; }
    
    fh.ReadFile();
    
    std::cout << "Počet záznamů: " << fh.ItemCount() << std::endl;
    std::cout << "Počet knih: " << fh.BookCount() << std::endl;
    std::cout << "Počet DVD: " << fh.DvdCount() << std::endl;
    std::cout << "Hmostnost knih: " << fh.BookWeight() / 1000 << " kg " << fh.BookWeight() % 1000 << " g" << std::endl;
    std::cout << "Nejnovější kniha: " << fh.NewestBook().name << std::endl;
    
    try { fh.OutputToHtml(); }
    catch (FileNotOpenException e)      { std::cout << e.Message() << std::endl; return -1; }
    catch (FileNotClosedException e)    { std::cout << e.Message() << std::endl; return -1; }

    
    try { fh.CloseFile(); }
    catch (FileNotClosedException e) { std::cout << e.Message() << std::endl; return -1; }
    
    return 0;
    
}
