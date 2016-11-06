# -*- coding: utf-8 -*-
from collections import OrderedDict

items = (
    ('A', 1),
    ('B', 2),
    ('C', 3)
)

print type(items)

regular_dict = dict(items)

print type(regular_dict)
print regular_dict

ordered_dict = OrderedDict(items)

print 'Regular Dict:'
for k, v in regular_dict.items():
    print k, v

print 'Ordered Dict:'
for k, v in ordered_dict.items():
    print k, v

