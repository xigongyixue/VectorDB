#include <vector>

#include "index/faiss_index.h"

FaissIndex::FaissIndex(faiss::Index* index) : index(index) {}

void FaissIndex::insert_vectors(const std::vector<float>& data, uint64_t label) {
    long id = static_cast<long>(label);

    // 1:单个向量 data.data():向量数据的指针 id:向量ID
    index->add_with_ids(1, data.data(), &id);
}

std::pair<std::vector<long> , std::vector<float> > FaissIndex::search_vectors(const std::vector<float>& query, int k) {
    int dim = index->d; // 向量的维度
    int num_queries = query.size() / dim; // 计算查询向量的数量
    std::vector<long> indices(num_queries * k); // 查询结果
    std::vector<float> distances(num_queries * k); // 查询结果距离
    index->search(num_queries, query.data(), k, distances.data(), indices.data()); // 执行查询
    return {indices, distances}; // 返回每个查询向量的KNN的{向量ID,距离}
}