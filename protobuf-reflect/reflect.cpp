#include <iostream>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
int main()
{
    // 准备配置好文件系统
    google::protobuf::compiler::DiskSourceTree sourceTree;
    // 将当前路径映射为项目根目录 ， project_root 仅仅是个名字，你可以使用你想要的合法名字.
    sourceTree.MapPath("test_pb","./");
    // 配置动态编译器.
    google::protobuf::compiler::Importer importer(&sourceTree, NULL);
    // 动态编译proto源文件。 
    importer.Import("test_pb/test.proto");
    // 现在可以从编译器中提取类型的描述信息.
    auto descriptor1 = importer.pool()->FindMessageTypeByName("T.Test");
    // 创建一个动态的消息工厂.
    google::protobuf::DynamicMessageFactory factory;
    // 从消息工厂中创建出一个类型原型.
    auto proto1 = factory.GetPrototype(descriptor1);
    // 构造一个可用的消息.
    auto message1= proto1->New();
    // 下面是通过反射接口给字段赋值.
    auto reflection1 = message1->GetReflection();
    auto filed1 = descriptor1->FindFieldByName("id");
    reflection1->SetInt32(message1,filed1,1);
    // 打印看看
    std::cout << message1->DebugString();
    // 删除消息.
    delete message1 ;
    return 0 ;
}
wq
