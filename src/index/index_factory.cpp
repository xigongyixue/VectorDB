#include "index/index_factory.h"
#include "index/faiss_index.h"
#include "index/hnswlib_index.h"
#include "faiss/IndexFlat.h"
#include "faiss/IndexIDMap.h"

// IndexFlatCodes继承Index，将向量数据按照写入顺序连续存储在内存，不提供查询函数
// IndexFlat继承IndexFlatCodes，实现了search函数，底层使用顺序遍历
// IndexIDMap继承Index，使用了vector数组，存储向量和对应ID


void IndexFactory::init(IndexType type, int dim, int num_data, MetricType metric) {
    faiss::MetricType faiss_metric = (metric == MetricType::L2) ? faiss::METRIC_L2 : faiss::METRIC_INNER_PRODUCT;

    switch (type) {
        case IndexType::FLAT:
            index_map[type] = new FaissIndex(new faiss::IndexIDMap(new faiss::IndexFlat(dim, faiss_metric)));
        case IndexType::HNSW:
            index_map[type] = new HNSWLibIndex(dim, num_data, metric, 16, 200);
            break;
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

// 匿名命名空间，变量仅限当前文件访问
namespace {
    IndexFactory globalIndexFactory;
} 

// 单例工厂
IndexFactory* getGlobalIndexFactory() {
    return &globalIndexFactory;
}