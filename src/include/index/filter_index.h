#include <string>
#include <map>

#include "roaring/roaring.h"
#include "scalar_storage.h"

class FilterIndex
{
    public:
        enum class Operation {
            EQUAL,
            NOT_EQUAL
        };

        FilterIndex();

        // 添加整数字段过滤条件，关联id
        void addIntFieldFilter(const std::string &fieldname, int64_t value, uint64_t id);

        // 更新整数字段过滤条件
        void updateIntFieldFilter(const std::string &fieldname, int64_t* old_value, int64_t new_value, uint64_t id);

        // 获取整数字段过滤条件对应的位图
        void getIntFieldFilterBitmap(const std::string& fieldname, Operation op, int64_t value, roaring_bitmap_t* result_bitmap);

        // 序列化
        std::string serializeIntFieldFilter();

        // 反序列化
        void deserializeIntFieldFilter(const std::string& serialized_data);

        // 持久化
        void saveIndex(ScalarStorage& scalar_storage, const std::string& key);
        void loadIndex(ScalarStorage& scalar_storage, const std::string& key);

    private:
        // 字段名 -> 字段值 -> 位图
        std::map<std::string, std::map<long, roaring_bitmap_t*>> intFieldFilter;
};
 
 