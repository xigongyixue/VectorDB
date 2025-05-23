#include "logger/logger.h"
#include "index/index_factory.h"
#include "httpserver/http_server.h"

int main() {
    init_global_logger();
    set_log_level(spdlog::level::debug);
    GlobalLogger->info("Global logger initialized");
    
    int dim = 1, num_data = 50;
    IndexFactory* globalIndexFactory = getGlobalIndexFactory();
    globalIndexFactory->init(IndexFactory::IndexType::HNSW, dim, num_data);
    GlobalLogger->info("Global IndexFactory initialized");

    HttpServer server("localhost", 8000);
    GlobalLogger->info("HttpServer created");
    server.start();
    return 0;
}