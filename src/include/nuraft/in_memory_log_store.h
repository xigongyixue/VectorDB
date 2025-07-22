#pragma once

#include "libnuraft/event_awaiter.hxx"
#include "libnuraft/internal_timer.hxx"
#include "libnuraft/log_store.hxx"

#include "database/vector_database.h"

#include <atomic>
#include <map>
#include <mutex>

namespace nuraft {

class raft_server;

class inmem_log_store: public log_store {
    public:
        inmem_log_store(VectorDatabase* vector_database_);
        ~inmem_log_store();
        __nocopy__(inmem_log_store); // 禁止复制构造函数

        // 获取下一个日志槽位
        ulong next_slot() const;

        // 获取起始索引
        ulong start_index() const;

        // 获取最后一个日志
        ptr<log_entry> last_entry() const;

        // 追加日志
        ulong append(ptr<log_entry>& entry);

        // 在指定索引处写日志
        void write_at(ulong index, ptr<log_entry>& entry);

        // 返回指定范围的日志 [start, end)
        ptr<std::vector<ptr<log_entry>>> log_entries(ulong start, ulong end);

        // *** (获得累计日志大小>=batch_size_hint_in_bytes)
        ptr<std::vector<ptr<log_entry>>> log_entries_ext(ulong start, ulong end, int64 batch_size_hint_in_bytes = 0);

        ptr<log_entry> entry_at(ulong index);

        ulong term_at(ulong index);

        // 打包以index开始指定数量的日志
        ptr<buffer> pack(ulong index, int32 cnt);

        // 使用包
        void apply_pack(ulong index, buffer& pack);

        // 删除此前的日志
        bool compact(ulong last_log_index);

        bool flush();

        void close();

        ulong last_durable_index();

        void set_disk_delay(raft_server* raft, size_t delay_ms);


    private:
        // 创建日志条目副本
        static ptr<log_entry> make_clone(const ptr<log_entry>& entry);

        // 模拟硬盘异步写
        void disk_emul_loop();

        std::map<ulong, ptr<log_entry> > logs_; // <索引，日志>

        mutable std::mutex logs_lock_; // 日志锁

        std::atomic<long> start_idx_; // 第一个日志的索引

        raft_server* raft_server_bwd_pointer_; // raft服务器的反向指针

        std::atomic<size_t> disk_emul_delay; // 模拟硬盘写延迟

        std::map<uint64_t, uint64_t> disk_emul_logs_being_written_; // <时间戳，日志索引> 在相应时间戳后的日志已被持久化

        std::unique_ptr<std::thread> disk_emul_thread_; // 线程：更新最后一个持久化的索引

        std::atomic<bool> disk_emul_thread_stop_signal_; // 结束线程的信号

        EventAwaiter disk_emul_ea_; // 事件等待：模拟硬盘延迟

        std::atomic<uint64_t> disk_emul_last_durable_index_; // 最后一个持久化的日志索引

        VectorDatabase* vector_database_; // 
};

};