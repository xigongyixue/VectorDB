#include <vector>
#include <iostream>
#include <fstream>
#include <typeinfo>

#include "index/faiss_index.h"
#include "logger/logger.h"
#include "common/constants.h" 

#include <faiss/IndexIDMap.h>
#include <faiss/IndexFlat.h>
#include <faiss/index_io.h> 

FaissIndex::FaissIndex(faiss::Index* index) : index(index) {}

void FaissIndex::insert_vectors(const std::vector<float>& data, uint64_t label) {
    long id = static_cast<long>(label);

    // 1:单个向量 data.data():向量数据的指针 id:向量ID
    index->add_with_ids(1, data.data(), &id);
}

std::pair<std::vector<long> , std::vector<float> > FaissIndex::search_vectors(const std::vector<float>& query, int k, const roaring_bitmap_t* bitmap) {
    int dim = index->d; // 向量的维度
    int num_queries = query.size() / dim; // 计算查询向量的数量
    std::vector<long> indices(num_queries * k); // 查询结果
    std::vector<float> distances(num_queries * k); // 查询结果距离

    // 根据传入的位图过滤id
    faiss::SearchParameters search_params;
    RoaringBitmapIDSelector selector(bitmap);
    if(bitmap != nullptr) {
        search_params.sel = &selector;
    }

    index->search(num_queries, query.data(), k, distances.data(), indices.data(), &search_params); // 执行查询

    GlobalLogger->debug("Retrieved values:");
    for (size_t i = 0; i < indices.size(); ++i) {
        if (indices[i] != -1) {
            GlobalLogger->debug("ID: {}, Distance: {}", indices[i], distances[i]);
        } else {
            GlobalLogger->debug("No specific value found");
        }
    }

    return {indices, distances}; // 返回每个查询向量的KNN的{向量ID,距离}
}

void FaissIndex::remove_vectors(const std::vector<long>& ids) { 
    faiss::IndexIDMap* id_map = dynamic_cast<faiss::IndexIDMap*>(index);
    if (id_map) {
        // 初始化IDSelectorBatch对象
        faiss::IDSelectorBatch selector(ids.size(), ids.data());
        id_map->remove_ids(selector);
    } else {
        throw std::runtime_error("Underlying Faiss index is not an IndexIDMap");
    }
}

void FaissIndex::saveIndex(const std::string& file_path) {
    faiss::write_index(index, file_path.c_str());
}

void FaissIndex::loadIndex(const std::string& file_path) {
    std::ifstream file(file_path); // 尝试打开文件
    if (file.good()) { // 检查文件是否存在
        file.close();
        if (index != nullptr) {
            delete index;
        }
        index = faiss::read_index(file_path.c_str());
    } else {
        GlobalLogger->warn("File not found: {}. Skipping loading index.", file_path);
    }
}