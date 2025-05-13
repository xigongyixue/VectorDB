#include "index/index_factory.h"
#include "faiss/IndexFlat.h"
#include "faiss/IndexIDMap.h"

void IndexFactory::init(IndexType type, int dim, MetricType metric) {
    faiss::MetricType faiss_metric = (metric == MetricType::L2) ? faiss::METRIC_L2 : faiss::METRIC_INNER_PRODUCT;

    switch (type) {
        case IndexType::FLAT:
            index_map[type] = new FaissIndex(new faiss::IndexIDMap(new faiss::IndexFlat(dim, faiss_metric)));
        default:
            break;
    }
}

void* IndexFactory::getIndex(IndexType type) const {
    auto it = index_map.find(type);
    if(it != index_map.end()) {
        return it->second;
    }
    return nullptr;
}

namespace {
    IndexFactory globalIndexFactory;
} 

// 单例工厂
IndexFactory* getGlobalIndexFactory() {
    return &globalIndexFactory;
}