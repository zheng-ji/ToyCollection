package main

import (
	"github.com/AdRoll/goamz/aws"
	"github.com/AdRoll/goamz/dynamodb"
)

type DDB struct {
	Region    string
	AccessKey string
	SecretKey string

	ItemCount int64

	TableName  string
	Table      *dynamodb.Table
	PrimaryKey dynamodb.PrimaryKey
}

func NewDDB(region, accessKey, secretKey, tableName string) (ddb *DDB, err error) {
	ddb = new(DDB)

	ddb.TableName = tableName
	ddb.AccessKey = accessKey
	ddb.SecretKey = secretKey

	auth := aws.Auth{AccessKey: ddb.AccessKey, SecretKey: ddb.SecretKey}
	conn := dynamodb.New(auth, Regions[region])

	ddbDes, err := conn.DescribeTable(tableName)
	ddb.ItemCount = ddbDes.ItemCount

	ddb.PrimaryKey, err = ddbDes.BuildPrimaryKey()
	ddb.Table = conn.NewTable(tableName, ddb.PrimaryKey)

	return
}

func (ddb *DDB) DeleteItem(attr map[string]*dynamodb.Attribute) (err error) {
	key := &dynamodb.Key{
		HashKey: attr[ddb.PrimaryKey.KeyAttribute.Name].Value,
	}
	if ddb.PrimaryKey.HasRange() {
		key.RangeKey = attr[ddb.PrimaryKey.RangeAttribute.Name].Value
	}
	if ok, err := ddb.Table.DeleteItem(key); !ok {
		return err
	}
	return nil
}
