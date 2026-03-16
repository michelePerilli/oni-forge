#include <service/VanillaCatalogService.hpp>

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
    VanillaCatalogService vanilla(repos, logger);
    vanilla.loadFromFolder(R"(D:\Dev\java\oniforge\vanilla\xml)");

    logger.info("Vanilla ONCC count: " + std::to_string(vanilla.getOnccFiles().size()));
    logger.info("Vanilla ONCV count: " + std::to_string(vanilla.getOncvFiles().size()));
    logger.info("Vanilla TRAC count: " + std::to_string(vanilla.getTracFiles().size()));

    return 0;
}
