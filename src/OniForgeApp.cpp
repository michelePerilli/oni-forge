#include "component/logger/Logger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"
#include "repository/TRACRepository.hpp"

int main() {
    Logger logger("oniforge.log");

    logger.separator();
    logger.info("This is OniForge!");

    const XmlReader reader(logger);
    const XmlWriter writer(logger);
    const TRACRepository tracRepository(reader, writer, logger);

    const auto trac = tracRepository.load(R"(D:\Dev\mods\oni\TCTFagent\TRACTCTFagent_animations.xml)");

    if (!trac) {
        logger.error("Failed to load TRAC.");
        return 1;
    }

    logger.info("Loaded TRAC with parent collection: " + trac->parentCollection);

    if (!tracRepository.save(*trac, R"(D:\Dev\mods\oni\TCTFagent\TRACTCTFagent_animations.xml)")) {
        logger.error("Failed to save TRAC.");
        return 1;
    }

    logger.info("TRAC saved successfully.");

    return 0;
}
