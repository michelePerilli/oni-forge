#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"

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
    const OniRepositoryRegistry repos{
        .oncc = onccRepository,
        .oncv = oncvRepository,
        .trac = tracRepository
    };
    VanillaCatalogService vanilla(repos, logger);
    vanilla.loadFromFolder(R"(D:\Dev\java\oniforge\vanilla\xml)");

    ProjectCatalogService project(repos, vanilla, logger);
    project.loadFromFolder(R"(D:\Dev\mods\oni\TCTFagent)");

    logger.separator();
    logger.info(std::format("  {:<10} | {:>6} | {:>6} | {:>6}", "Catalog", "ONCC", "ONCV", "TRAC"));
    logger.info("  -----------|--------|--------|--------");
    logger.info(std::format("  {:<10} | {:>6} | {:>6} | {:>6}", "Vanilla", vanilla.getOnccFiles().size(), vanilla.getOncvFiles().size(), vanilla.getTracFiles().size()));
    logger.info(std::format("  {:<10} | {:>6} | {:>6} | {:>6}", "Project", project.getOnccFiles().size(), project.getOncvFiles().size(), project.getTracFiles().size()));
    logger.separator();
    return 0;
}
