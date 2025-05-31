#include "index/filter_index.h"
#include "logger/logger.h"

FilterIndex::FilterIndex() {};

void FilterIndex::addIntFieldFilter(const std::string &fieldname, int64_t value, uint64_t id) {
    // 新建位图
    roaring_bitmap_t* bitmap = roaring_bitmap_create();
    // 将id加入位图，表示满足过滤条件
    roaring_bitmap_add(bitmap, id);
    // 关联位图、字段名和数值
    intFieldFilter[fieldname][value] = bitmap;

    GlobalLogger->debug("Added int field filter: fieldname={}, value={}, id={}", fieldname, value, id);
}

// 将id从旧值的位图转移到新值的位图
void FilterIndex::updateIntFieldFilter(const std::string& fieldname, int64_t* old_value, int64_t new_value, uint64_t id) {
    if (old_value != nullptr) {
        GlobalLogger->debug("Updated int field filter: fieldname={}, old_value={}, new_value={}, id={}", fieldname, *old_value, new_value, id);
    } else {
        GlobalLogger->debug("Updated int field filter: fieldname={}, old_value=nullptr, new_value={}, id={}", fieldname, new_value, id);
    }    
    
    auto it = intFieldFilter.find(fieldname);
    if (it != intFieldFilter.end()) {
        std::map<long, roaring_bitmap_t*>& value_map = it->second;

        // 查找旧值对应的位图，并从位图中删除 ID
        auto old_bitmap_it = (old_value != nullptr) ? value_map.find(*old_value) : value_map.end(); // 使用解引用的 old_value
        if (old_bitmap_it != value_map.end()) {
            roaring_bitmap_t* old_bitmap = old_bitmap_it->second;
            roaring_bitmap_remove(old_bitmap, id);
        }

        // 查找新值对应的位图，如果不存在则创建一个新的位图
        auto new_bitmap_it = value_map.find(new_value);
        if (new_bitmap_it == value_map.end()) {
            roaring_bitmap_t* new_bitmap = roaring_bitmap_create();
            value_map[new_value] = new_bitmap;
            new_bitmap_it = value_map.find(new_value);
        }

        roaring_bitmap_t* new_bitmap = new_bitmap_it->second;
        roaring_bitmap_add(new_bitmap, id);
    } else {
        addIntFieldFilter(fieldname, new_value, id);
    }
}

void FilterIndex::getIntFieldFilterBitmap(const std::string& fieldname, Operation op, int64_t value, roaring_bitmap_t* result_bitmap) {
    auto it = intFieldFilter.find(fieldname);
    if(it != intFieldFilter.end()) {
        auto& value_map = it->second;
        if(op == Operation::EQUAL) { // 等于操作
            auto bitmap_it = value_map.find(value);
            if(bitmap_it != value_map.end()) {
                // 将找到的位图与结果合并返回
                roaring_bitmap_or_inplace(result_bitmap, bitmap_it->second);
                GlobalLogger->debug("Retrieved EQUAL bitmap for fieldname={}, value={}", fieldname, value);
            }
        } else if(op ==Operation::NOT_EQUAL) { // 不等于操作
            for(const auto& entry: value_map) {
                if(entry.first != value) {
                    // 合并结果
                    roaring_bitmap_or_inplace(result_bitmap, entry.second);
                }
            }
            GlobalLogger->debug("Retrieved NOT_EQUAL bitmap for fieldname={}, value={}", fieldname, value);
        }
    }
}