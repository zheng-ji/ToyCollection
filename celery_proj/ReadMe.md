* 启动worker 

```
python backend.py worker -Q test -l info --beat
```

* 执行计算任务

```
python test.py
```

----

* 执行crontab

```
python crontab.py worker --beat -l info
```
