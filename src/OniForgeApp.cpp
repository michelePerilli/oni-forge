#include <iostream>
#include "component/xml/XmlDocument.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

int main() {
    std::cout << "This is OniForge!" << std::endl;

    XmlDocument document;

    if (XmlReader reader; !reader.read(R"(D:\Dev\mods\oni\TCTFagent\ONCCTCTFagent.xml)", document)) {
        std::cerr << "Failed to read document." << std::endl;
        return 1;
    }

    std::cout << "File read successfully." << std::endl;

    if (XmlWriter writer; !writer.write(document, R"(D:\Dev\mods\oni\TCTFagent\ONCCTCTFagent.xml)")) {
        std::cerr << "Failed to write document." << std::endl;
        return 1;
    }

    std::cout << "File written successfully." << std::endl;

    return 0;
}