#include "logger/logger.h"
#include "index/index_factory.h"
#include "httpserver/http_server.h"
#include "database/vector_database.h"

int main() {
    // 初始化日历记录器
    init_global_logger();
    set_log_level(spdlog::level::debug);
    GlobalLogger->info("Global logger initialized");

    // 初始化全局IndexFactory实例
    int dim = 1, num_data = 100;
    IndexFactory* globalIndexFactory = getGlobalIndexFactory();
    globalIndexFactory->init(IndexFactory::IndexType::FLAT, dim);
    globalIndexFactory->init(IndexFactory::IndexType::HNSW, dim, num_data);
    GlobalLogger->info("Global IndexFactory initialized");

    // 初始化VectorDatabase对象
    std::string db_path = "ScalarStorage"; // RocksDB路径
    VectorDatabase vector_database(db_path);
    GlobalLogger->info("VectorDatabase initialized");

    // 创建并启动服务器
    HttpServer server("localhost", 8000, &vector_database);
    GlobalLogger->info("HttpServer created");
    server.start();

    return 0;
}