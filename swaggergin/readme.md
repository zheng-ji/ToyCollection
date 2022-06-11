gin 使用 Swagger

go get -u github.com/swaggo/swag/cmd/swag

Run the Swag at your Go project root path(for instance ~/root/go-peoject-name), Swag will parse comments and generate required files(docs folder and docs/doc.go) at ~/root/go-peoject-name/docs.

* swag init

go get -u github.com/swaggo/gin-swagger
go get -u github.com/swaggo/files

使用
http://localhost:8080/swagger/index.html#/example/get_example_helloworld
