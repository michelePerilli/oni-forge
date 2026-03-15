#include <iostream>
#include <pugixml.hpp>

int main() {
    std::cout << "This is oni forge!" << std::endl;
    pugi::xml_document doc;
    std::cout << "PugiXML version: " << PUGIXML_VERSION << " is ready!" << std::endl;
    return 0;
}