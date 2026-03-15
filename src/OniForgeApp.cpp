#include <repository/ONCVRepository.hpp>

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
    const ONCVRepository oncvRepository(reader, writer, logger);
    const TRACRepository tracRepository(reader, writer, logger);

    const auto trac = tracRepository.load(R"(D:\Dev\mods\oni\TCTFagent\TRACTCTFagent_animations.xml)");
    const auto oncv = oncvRepository.load(R"(D:\Dev\mods\oni\.data\xml\ONCV\ONCVninja_easy.xml)");

    if (!trac) {
        logger.error("Failed to load TRAC.");
        return 1;
    }
    if (!oncv) {
        logger.error("Failed to load ONCV.");
        return 1;
    }

    if (!tracRepository.save(*trac)) {
        logger.error("Failed to save TRAC.");
        return 1;
    }

    if (!oncvRepository.save(*oncv)) {
        logger.error("Failed to save ONCV.");
        return 1;
    }

    logger.info("TRAC saved successfully.");

    return 0;
}
