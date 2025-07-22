#pragma once

#include "libnuraft/nuraft.hxx"

#include "in_memory_log_store.h"

#include <string>

namespace nuraft {

// 管理节点的状态信息
class in_memory_state_mgr: public state_mgr {
    public:
        in_memory_state_mgr(int srv_id, const std::string& endpoint, VectorDatabase* vector_database)
            : my_id_(srv_id), my_endpoint_(endpoint), cur_log_store_(cs_new<inmem_log_store>(vector_database)) {
                // 初始化服务器配置和集群配置
                my_srv_config_ = cs_new<srv_config>(srv_id, endpoint);
                saved_config_ = cs_new<cluster_config>();
                saved_config_->get_servers().push_back(my_srv_config_);
            }

        ~in_memory_state_mgr() {}

        // 加载集群配置
        ptr<cluster_config> load_config() {
            return saved_config_;
        }
        
        // 保存集群配置
        void save_config(const cluster_config& config) {
            ptr<buffer> buf = config.serialize();
            saved_config_ = cluster_config::deserialize(*buf);
        }

        // 保存节点状态
        void save_state(const srv_state& state) {
            ptr<buffer> buf = state.serialize();
            saved_state_ = srv_state::deserialize(*buf);
        }

        // 读取节点状态
        ptr<srv_state> read_state() {
            return saved_state_;
        }

        // 加载日志存储
        ptr<log_store> load_log_store() {
            return cur_log_store_;
        }

        // 获取服务器id
        int32 server_id() {
            return my_id_;
        }

        void system_exit(const int exit_code) {
        }

        // 获取服务器配置
        ptr<srv_config> get_srv_config() const {
            return my_srv_config_;
        }

    private:
        int my_id_;
        std::string my_endpoint_;
        ptr<inmem_log_store> cur_log_store_;
        ptr<srv_config> my_srv_config_;
        ptr<cluster_config> saved_config_;
        ptr<srv_state> saved_state_;
        
};

};
