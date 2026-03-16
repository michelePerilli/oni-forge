#include "repository/OniRepositoryRegistry.hpp"

#include "component/logger/Logger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

int main() {
    Logger logger("oniforge.log");

    logger.separator();
    logger.info("This is OniForge!");

    const XmlReader reader(logger);
    const XmlWriter writer(logger);
    const ONCCRepository onccRepository(reader, writer, logger);
    const ONCVRepository oncvRepository(reader, writer, logger);
    const TRACRepository tracRepository(reader, writer, logger);
    const OniRepositoryRegistry repos {
        .oncc = onccRepository,
        .oncv = oncvRepository,
        .trac = tracRepository
    };

    const auto trac = repos.trac.load(R"(D:\Dev\mods\oni\TCTFagent\TRACTCTFagent_animations.xml)");
    const auto oncv = repos.oncv.load(R"(D:\Dev\mods\oni\.data\xml\ONCV\ONCVninja_easy.xml)");
    const auto oncc = repos.oncc.load(R"(D:\Dev\mods\oni\TCTFagent\ONCCTCTFagent.xml)");

    if (!trac) {
        logger.error("Failed to load TRAC.");
        return 1;
    }
    if (!oncv) {
        logger.error("Failed to load ONCV.");
        return 1;
    }
    if (!oncc) {
        logger.error("Failed to load ONCC.");
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

    if (!onccRepository.save(*oncc)) {
        logger.error("Failed to save ONCC.");
        return 1;
    }

    return 0;
}
