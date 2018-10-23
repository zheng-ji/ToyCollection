#include <iostream>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "sb.pb.h"

using namespace std;
using namespace google::protobuf;

Message* createMessage(const string &typeName) {
    Message *message = NULL;
    // 查找message的descriptor
    const Descriptor *descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
    if (descriptor) {
        // 创建default message(prototype)
        const Message *prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (NULL != prototype) {
            // 创建一个可修改的message
            message = prototype->New();
            cout << "here3" << endl;
        }
    } else {
        cout << "here6" << endl;
    }
    return message;
}

int main() {
    // 使用上面的函数创建一个新的UserInfo message
    Message *msg = createMessage("mmuxsb.student");
    if (NULL == msg) {
        // 创建失败，可能是消息名错误，也可能是编译后message解析器
        // 没有链接到主程序中。
        cout << "here5" << endl;
        return -1;
    }

    // 获取message的descriptor
    const Descriptor* descriptor = msg->GetDescriptor();
    // 获取message的反射接口，可用于获取和修改字段的值
    const Reflection* reflection = msg->GetReflection();

    // 根据字段名查找message的字段descriptor
    const FieldDescriptor* idField = descriptor->FindFieldByName("age");
    // 将id设置为100
    if (NULL != idField) {
        reflection->SetInt32(msg, idField, 100);
        cout << "here2" << endl;
    }
    std::cout << msg->ShortDebugString() << endl;

    // ... 其他操作

    // 最后删除message
    delete msg;

    return 0;
}
