#include "index/index_factory.h"
#include "index/faiss_index.h"
#include "index/hnswlib_index.h"
#include "index/filter_index.h"

#include <faiss/IndexFlat.h>
#include <faiss/IndexIDMap.h>

// 匿名命名空间，变量仅限当前文件访问
namespace {
    IndexFactory globalIndexFactory;
} 

// 单例工厂
IndexFactory* getGlobalIndexFactory() {
    return &globalIndexFactory;
}

// IndexFlatCodes继承Index，将向量数据按照写入顺序连续存储在内存，不提供查询函数
// IndexFlat继承IndexFlatCodes，实现了search函数，底层使用顺序遍历
// IndexIDMap继承Index，使用了vector数组，存储向量和对应ID

void IndexFactory::init(IndexType type, int dim, int num_data, MetricType metric) {
    faiss::MetricType faiss_metric = (metric == MetricType::L2) ? faiss::METRIC_L2 : faiss::METRIC_INNER_PRODUCT;

    switch (type) {
        case IndexType::FLAT:
            index_map[type] = new FaissIndex(new faiss::IndexIDMap(new faiss::IndexFlat(dim, faiss_metric)));
            break;
        case IndexType::HNSW:
            index_map[type] = new HNSWLibIndex(dim, num_data, metric, 16, 200);
            break;
        case IndexType::FILTER:
            index_map[type] = new FilterIndex();
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

void IndexFactory::saveIndex(const std::string& folder_path, ScalarStorage& scalar_storage) {

    for (const auto& index_entry : index_map) {
        IndexType index_type = index_entry.first;
        void* index = index_entry.second;

        // 为每个索引类型生成一个文件名
        std::string file_path = folder_path + std::to_string(static_cast<int>(index_type)) + ".index";

        // 根据索引类型调用相应的 saveIndex 函数
        if (index_type == IndexType::FLAT) {
            static_cast<FaissIndex*>(index)->saveIndex(file_path);
        } else if (index_type == IndexType::HNSW) {
            static_cast<HNSWLibIndex*>(index)->saveIndex(file_path);
        } else if (index_type == IndexType::FILTER) { // 保存 FilterIndex 类型的索引
            static_cast<FilterIndex*>(index)->saveIndex(scalar_storage, file_path);
        }
    }
}

void IndexFactory::loadIndex(const std::string& folder_path, ScalarStorage& scalar_storage) {
    for (const auto& index_entry : index_map) {
        IndexType index_type = index_entry.first;
        void* index = index_entry.second;

        // 为每个索引类型生成一个文件名
        std::string file_path = folder_path + std::to_string(static_cast<int>(index_type)) + ".index";

        // 根据索引类型调用相应的 loadIndex 函数
        if (index_type == IndexType::FLAT) {
            static_cast<FaissIndex*>(index)->loadIndex(file_path);
        } else if (index_type == IndexType::HNSW) {
            static_cast<HNSWLibIndex*>(index)->loadIndex(file_path);
        } else if (index_type == IndexType::FILTER) { // 加载 FilterIndex 类型的索引
            static_cast<FilterIndex*>(index)->loadIndex(scalar_storage, file_path);
        }
    }
}