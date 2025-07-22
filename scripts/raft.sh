# 使用配置文件启动 /home/cc/vectorDB/build/src/vdbserver
./vdb_server ../../../conf1.ini

# 提升为主节点
curl -X POST -H "Content-Type: application/json" -d '{}' http://localhost:8080/admin/setLeader

# 查看节点信息
curl -X GET http://localhost:8080/admin/listNode

# 写数据
curl -X POST -H "Content-Type: application/json" -d '{"id": 6, "vectors": [0.9], "int_field": 49, "indexType": "FLAT"}' http://localhost:8080/upsert

# 读数据
curl -X POST -H "Content-Type: application/json" -d '{"vectors": [0.9], "k": 5, "indexType": "FLAT", "filter":{"fieldName":"int_field","value":43, "op":"!="}}' http://localhost:8080/search

# 加从节点
curl -X POST -H "Content-Type: application/json" -d '{"nodeId": 2, "endpoint": "127.0.0.1:8082"}' http://localhost:8080/admin/addFollower
curl -X GET http://localhost:8080/admin/listNode