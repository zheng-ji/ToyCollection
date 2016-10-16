#!/usr/bin/env python
# encoding: utf-8

class Field(object):
    # 负责保存数据库表的字段名和字段类型
    def __init__(self, name, column_type):
        self.name = name
        self.column_type = column_type
    def __str__(self):
        return '<%s:%s>' % (self.__class__.__name__, self.name)
# 各种类型的field
class StringField(Field):
    def __init__(self, name):
        super(StringField, self).__init__(name, 'varchar(100)')

class IntegerField(Field):
    def __init__(self, name):
        super(IntegerField, self).__init__(name, 'bigint')        

# 定义出自定义元类, 用于创建不同表(不同类)
class ModelMetaclass(type):
    def __new__(cls, name, bases, attrs):
        # 基类
        if name=='Model':
            return type.__new__(cls, name, bases, attrs)
        # ORM设置
        #print "attr",attrs
        mappings = dict()
        for k, v in attrs.iteritems():
            if isinstance(v, Field):
                print('Found mapping: %s==>%s' % (k, v))
                mappings[k] = v
        # 不要把实例属性和类属性使用相同的名字。
        # ModelMetaclass会删除掉User类的所有类属性，目的就是避免造成混淆。
        for k in mappings.iterkeys():
            attrs.pop(k)
        # 将属于列的属性先提到mapping,保存到__mappings__中,其余用于创建类.
        attrs['__table__'] = name # 假设表名和类名一致
        attrs['__mappings__'] = mappings # 保存属性和列的映射关系
        return type.__new__(cls, name, bases, attrs)

# 基类
class Model(dict):
    __metaclass__ = ModelMetaclass

    def __init__(self, **kw):
        super(Model, self).__init__(**kw)

    def __getattr__(self, key):
        try:
            return self[key]
        except KeyError:
            raise AttributeError(r"'Model' object has no attribute '%s'" % key)

    def __setattr__(self, key, value):
        self[key] = value

    def save(self):
        fields = []
        params = []
        args = []
        # __mappings__属性由元类创建,对应于列.
        for k, v in self.__mappings__.iteritems():
            fields.append(v.name) #列名
            params.append('?')
            args.append(getattr(self, k, None))
        sql = 'insert into %s (%s) values (%s)' % (self.__table__, ','.join(fields), ','.join(params))
        print('SQL: %s' % sql)
        print('ARGS: %s' % str(args))

# 调用接口,使用ORM框架,这里想定义一个User类来操作对应的数据库表User
# 其中，父类Model和属性类型StringField、IntegerField是由ORM框架提供的，剩下的魔术方法比如save()全部由基类借助元类的处理自动完成。虽然metaclass的编写会比较复杂，但ORM的使用者用起来却异常简单。
class User(Model):
    # 定义类的属性到列的映射：
    id = IntegerField('id')
    name = StringField('username')
    email = StringField('email')
    password = StringField('password')

    def __init__(self, **kw):
        super(User, self).__init__(**kw)
        self.name = "football"


# 创建一个实例：
u = User(id=12345, name='Michael', email='test@orm.org', password='my-pwd')
# 保存到数据库：
u.save()
