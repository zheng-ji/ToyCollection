#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
# # file: client.py
from gearman import GearmanClient  
new_client = GearmanClient(['127.0.0.1:4730'])  
current_request = new_client.submit_job('ping', 'zhengji')  
new_result = current_request.result  
print new_result

