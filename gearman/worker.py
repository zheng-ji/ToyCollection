#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
# file: worker.py 
import os  
import gearman  
import math  

class CustomGearmanWorker(gearman.GearmanWorker):  
  def on_job_execute(self, current_job):  
      return super(CustomGearmanWorker, self).on_job_execute(current_job)  

def task_callback(gearman_worker, job):  
  print job.data  
  return job.data + " has received"  

new_worker = CustomGearmanWorker(['127.0.0.1:4730'])  
new_worker.register_task("ping", task_callback)  
new_worker.work()
