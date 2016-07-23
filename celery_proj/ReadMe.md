* 启动worker 

```
python backend.py worker -Q test -l info
```

* 执行计算任务

```
python test.py
```

----

* 执行crontab

```
python crontab.py worker -B -l info
```
