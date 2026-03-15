#include "component/logger/Logger.hpp"
#include "component/xml/XmlDocument.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

int main() {
    Logger logger("oniforge.log");

    logger.separator();
    logger.info("This is OniForge!");

    XmlDocument document;

    if (const XmlReader reader(logger); !reader.read(R"(D:\Dev\mods\oni\TCTFagent\ONCCTCTFagent.xml)", document)) {
        logger.error("Failed to read document.");
        return 1;
    }

    logger.info("File read successfully.");

    if (const XmlWriter writer(logger); !writer.write(document, R"(D:\Dev\mods\oni\TCTFagent\ONCCTCTFagent.xml)")) {
        logger.error("Failed to write document.");
        return 1;
    }

    logger.info("File written successfully.");

    return 0;
}
