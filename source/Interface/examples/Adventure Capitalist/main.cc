#include <iostream>
#include <string>

#include "gameInterface.hh"

int main() {
    GameInterface gameInterface;

    while (true) {
        std::cout << "1.) Money: "  << gameInterface.get_money()  << std::endl;
        std::cout << "2.) Angels: " << gameInterface.get_angels() << std::endl;
        
        char selected;
        std::cin >> selected;
        
        switch(selected) {
            case '1': {
                std::cout << std::endl << "Value: ";
                double value;
                std::cin >> value;
                gameInterface.set_money(value);
                break;
            }
            
            case '2': {
                std::cout << std::endl << "Value: ";
                double value;
                std::cin >> value;
                gameInterface.set_angels(value);
            }
            
            default: 
                continue;        
        };
    }

    return 0;
}