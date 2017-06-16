package main

import (
	"errors"
	"github.com/AdRoll/goamz/dynamodb"
	"github.com/fatih/color"
	"strings"
)

var (
	ErrFiledInValid  = errors.New("invalid field")
	ErrRawStrInValid = errors.New("raw string invalid")
)

type Tokenizer struct {
	rawStr          string
	TableName       string
	NeedFields      []string
	ConditionFields []*ConditionOp
}

type ConditionOp struct {
	Field    string
	Operator string
	Value    string
}

func NewTokenizer(whereStr string) (tokenizer *Tokenizer, err error) {
	tokenizer = new(Tokenizer)

	tokenizer.rawStr = whereStr
	tokenizer.NeedFields = []string{}
	tokenizer.ConditionFields = []*ConditionOp{}
	err = tokenizer.buildFields()

	return tokenizer, err
}

func (t *Tokenizer) buildFields() (err error) {

	f := func(c rune) bool {
		return c == ',' || c == ' ' || c == ';'
	}
	fields := strings.FieldsFunc(t.rawStr, f)

	if len(fields) < 2 || strings.ToUpper(fields[0]) != SELECT {
		return ErrRawStrInValid
	}

	// Fill NeedFields
	fromIndex := 1
	for _, token := range fields[1:] {
		fromIndex++
		if strings.ToUpper(token) == FROM {
			break
		}
		t.NeedFields = append(t.NeedFields, token)
	}

	// Fill TableName
	t.TableName = fields[fromIndex]

	// Fill ConditionFields 表达式形如 a = 2　中间要有空格，这是个不优雅的规定，
	// 后面应该要想办法智能起来

	numOfAnds := strings.Count(strings.ToUpper(t.rawStr), AND)
	whereIndex := fromIndex + 2
	if whereIndex <= len(fields) {
		conditionArr := fields[whereIndex:]

		if len(conditionArr) != (numOfAnds+1)*TUPLENUM-1 {
			return ErrRawStrInValid
		}

		for i := 0; i <= numOfAnds; i++ {
			conditionOp := new(ConditionOp)
			conditionOp.Field = conditionArr[TUPLENUM*i+0]
			conditionOp.Operator = conditionArr[TUPLENUM*i+1]
			conditionOp.Value = conditionArr[TUPLENUM*i+2]
			t.ConditionFields = append(t.ConditionFields, conditionOp)
		}
	}

	return nil
}

func (t *Tokenizer) Display() {
	color.Green("Dynamodb Table: %s\n", t.TableName)
	color.Green("SQL: %s\n", t.rawStr)
	color.Green("NeedFields: %v\n", t.NeedFields)
	color.Green("Select Condition as Below:\n")
	for _, token := range t.ConditionFields {
		color.Green("%s %s %s\n", token.Field, token.Operator, token.Value)
	}
}

func (t *Tokenizer) Condition(attr map[string]*dynamodb.Attribute) bool {

	flag := true

	for _, token := range t.ConditionFields {

		if _, ok := attr[token.Field]; !ok {
			flag = flag && false
		}

		left := attr[token.Field].Value
		right := token.Value

		switch token.Operator {
		case "=":
			flag = flag && (left == right)

		case "<":
			flag = flag && (left < right)

		case "<=":
			flag = flag && (left <= right)

		case ">":
			flag = flag && (left > right)

		case ">=":
			flag = flag && (left >= right)

		case "!=":
			flag = flag && (left != right)
		}
	}
	return flag
}
