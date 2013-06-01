#include "mystatus.h"

const char* CopyState(const char* state) {
    int size;
    memcpy(&size,state,sizeof(size));
    char* result = new char[size+5];
    memcpy(result,state,size+5);
    return result;
}

Status::Status(Code code,const Slice& msg,const Slice& msg2) {
    assert(code != kOk);
    const int len1 = msg.size();
    const int len2 = msg2.size();
    const int size = len1 + (len2? (2 + len2):0);
    char* result = new char[size + 5];
    memcpy(result,&size,sizeof(size));
    
    int length;
    memcpy(&length,result,sizeof(length));
    result[4] = static_cast<char>(code);

    memcpy(result + 5, msg.data(),len1);
    if (len2) {
        result[5+len1] = ':';
        result[6+len1] = ' ';
        memcpy(result+7+len1,msg2.data(),len2);
    }
    state_ = result;
}


string Status::ToString() const {
    if (state_ == NULL) {
        return "OK";
    } else {
        char tmp[30];
        const char* type;
        switch(code()) {
            case kOk:
                type = "OK";
                break;
            case kNotFound:
                type = "NotFound: ";
                break;
            case kCorruption:
                type = "Corruption: ";
                break;
            case kNotSupported:
                type = "Not implemented";
                break;
            case kInvalidArgument:
                type = "Invalid argument: ";
                break;
            case kIOError:
                type = "IO error:";
                break;
            default:
                snprintf(tmp,sizeof(tmp),"Unknown code(%d): ",static_cast<int>(code()));
                type = tmp;
                break;
        }
        string result(type);
        int length;
        memcpy(&length,state_,sizeof(length));
        result.append(state_ + 5,length);
        return result;
    }
}
