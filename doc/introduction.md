vdb server
- http port：8080
- node id: 1
- raft endpoint: 127.0.0.1:8081

master server
- http port: 6060
- etcd port: 127.0.0.1:2379
- 提醒：etcd会直接将数据持久化

proxy server
- http port: 80
- instance id: instance1