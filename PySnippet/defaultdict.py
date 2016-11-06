# -*- coding: utf-8 -*-
from collections import defaultdict

members = [
    # Age, name
    ['male', 'John'],
    ['male', 'Jack'],
    ['female', 'Lily'],
    ['male', 'Pony'],
    ['female', 'Lucy'],
]

print type(members)
for item in members:
    print item

result = defaultdict(list)

print type(result)

for sex, name in members:
    result[sex].append(name)

print result

