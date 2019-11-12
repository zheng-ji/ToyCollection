/?pretty" -H 'Content-Type:application/json' -d'
    {
        "settings" : {
            "index":{
                "refresh_interval": "2s",
                "auto_expand_replicas" : "0-all",
                "number_of_shards" : "5",
                "translog": { "sync_interval" : "2s","durability" : "async" },
                "indexing": { "slowlog" : { "threshold" : { "index" : { "warn" : "3s" }}}},
                "search": { "slowlog" : { "threshold" : { "fetch" : { "info" : "500ms" },"query" : { "info" : "1m" }}}}, 
                "analysis": { "analyzer": { "title_analyzer": { "type" : "custom", "tokenizer": "whitespace", "filter":  [ "cjk_width", "lowercase", "porter_stem" ] } }
                }
            }
        },
        "mappings" : {
            "group_member": {
                "dynamic": "strict",
                "properties": {
                    "user_uin": { "type": "long" },
                    "split_nickname": { "type": "text", "analyzer": "title_analyzer" },
                    "nickname": { "type": "text","analyzer" :"title_analyzer" }, 
                    "update_time" : {"type": "date"}, 
                    "group_feed_info": {
                      "type": "nested", 
                      "properties": {
                        "group_id": { "type": "long" },
                        "feed_num": { "type": "integer" }
                      }
                    }
                }
            }
        }
    }';


GET group_member_v2/_search
{
    "query":  {
      "bool":{
        "must": {
            "nested" : {
            "path" : "group_feed_info",
            "query" : {
                "bool" : {
                    "must" : [
                    { "match" : {"group_feed_info.group_id" : 13104377202860687} },
                    { "range" : {"group_feed_info.feed_num": {"gt" : 0}} }
                  ]
                }
            }
          }
        },
       "minimum_should_match": 1,
        "should": [
                    { "match": {"split_nickname": {"query": "test","operator": "and"}}},
                    { "constant_score": {"boost": 1000,"_name": "phrase_boost","filter": {"match_phrase": {"split_nickname": "吖"}}}
                }
          ]
      }
    },
    "sort" : 
       {
          "group_feed_info.feed_num" : {
             "mode" :  "avg",
             "order" : "desc",
             "nested": {
                "path": "group_feed_info",
                 "filter": {
                   "term" : { "group_feed_info.group_id" : 13104377202860687 }
                }
             }
          }
       }
    
}
