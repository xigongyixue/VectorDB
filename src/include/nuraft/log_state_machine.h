#pragma once

#include <libnuraft/nuraft.hxx>

#include "database/vector_database.h"

#include <atomic>

using namespace nuraft;

// 业务逻辑和行为，处理和提交日志，改变系统状态
class log_state_machine : public state_machine {
    public:
        log_state_machine(): last_committed_idx_(0) { }

        ~log_state_machine() { }

        void setVectorDatabase(VectorDatabase* vector_database);

        // 提交日志，返回结果
        ptr<buffer> commit(const ulong log_idx, buffer& data);

        // 预提交
        ptr<buffer> pre_commit(const ulong log_idx, buffer& data);

        // 更新提交索引
        void commit_config(const ulong log_idx, ptr<cluster_config>& new_conf) {
            last_committed_idx_ = log_idx;
        }

        // 回滚指定日志索引
        void rollback(const ulong log_idx) { }

        int read_logical_snp_obj(snapshot& s, void*& user_snp_ctx, ulong obj_id, ptr<buffer>& data_out, bool& is_last_obj) {
            return 0;
        }

        void save_logical_snp_obj(snapshot& s, ulong& obj_id, buffer& data, bool is_first_obj, bool is_last_obj) { }

        bool apply_snapshot(snapshot& s) {
            return true;
        }

        void free_user_snp_ctx(void*& user_snp_ctx) { }

        ptr<snapshot> last_snapshot() {
            return nullptr;
        }

        // 最后提交的日志索引
        ulong last_commit_index() {
            return last_committed_idx_;
        }

        void create_snapshot(snapshot& s, async_result<bool>::handler_type& when_done) { }

    private:
        std::atomic<uint64_t> last_committed_idx_; // 最后一个已提交日志序号

        VectorDatabase* vector_database_; 
};
