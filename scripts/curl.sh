# flat索引测试
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.8], "id":2, "indexType": "FLAT"}' http://localhost:8000/insert
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.5], "k":2, "indexType": "FLAT"}' http://localhost:8000/search
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.5], "k":2, "indexType": "FLAT1"}' http://localhost:8000/search

# hnsw索引测试
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.2], "id":3, "indexType": "HNSW"}' http://localhost:8000/insert
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.5], "k":2, "indexType": "HNSW"}' http://localhost:8000/search

# 混合索引测试
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.555555], "id":3, "indexType": "FLAT", "Name":"hello", "Ci":1111}' http://localhost:8000/upsert
curl -X POST -H "Content-Type: application/json" -d '{"id":3}' http://localhost:8000/query

# 过滤索引测试
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.9], "id":6, "int_field":47,"indexType": "FLAT"}' http://localhost:8000/upsert
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.9], "k":6, "indexType": "FLAT", "filter":{"fieldName":"int_field","value":47, "op":"="}}' http://localhost:8000/search
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.9], "k":6, "indexType": "FLAT", "filter":{"fieldName":"int_field","value":47, "op":"!="}}' http://localhost:8000/search

# 持久化测试
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.9], "id":6, "int_field":47,"indexType": "FLAT"}' http://localhost:8000/upsert
curl -X POST -H "Content-Type: application/json" -d '{}' http://localhost:8000/admin/snapshot
# ---------------中断程序，重启----------------------------
curl -X POST -H "Content-Type: application/json" -d '{"id":6}' http://localhost:8000/query
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.9], "k":6, "indexType": "FLAT", "filter":{"fieldName":"int_field","value":47, "op":"="}}' http://localhost:8000/search
