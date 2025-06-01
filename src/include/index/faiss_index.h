#pragma once

#include "faiss/Index.h"
#include "faiss/utils/utils.h"
#include "faiss/impl/IDSelector.h"
#include "roaring/roaring.h"

#include <vector>

struct RoaringBitmapIDSelector:faiss::IDSelector {
    RoaringBitmapIDSelector(const roaring_bitmap_t* bitmap): bitmap_(bitmap) {}

    // 重载函数，用于判断id是否在位图中
    bool is_member(int64_t id) const final {
        return roaring_bitmap_contains(bitmap_, static_cast<uint32_t>(id));
    }
    ~RoaringBitmapIDSelector() override {}
    const roaring_bitmap_t* bitmap_;
};

class FaissIndex {
    public:
        FaissIndex(faiss::Index* index);

        // 将单个向量数据和标签写入索引中
        void insert_vectors(const std::vector<float>& data, uint64_t label);

        // 查询与待查询向量最近邻的K个向量
        // 返回找到的向量标签和相应的距离
        std::pair<std::vector<long> , std::vector<float> > search_vectors(const std::vector<float>& query, int k, const roaring_bitmap_t* bitmap = nullptr);

        // 根据id删除向量
        void remove_vectors(const std::vector<long>& ids); 

        // 持久化
        void saveIndex(const std::string& file_path);
        void loadIndex(const std::string& file_path);

    private:
        faiss::Index* index;
};