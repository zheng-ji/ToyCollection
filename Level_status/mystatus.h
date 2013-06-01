#ifndef _STATUS_H_
#define _STATUS_H_
#include "myslice.h"
#include <stdio.h>
#include <iostream>
using namespace std;

class Status {
    public:
        Status():state_(NULL) {}
        ~Status() {delete[] state_;}
        Status(const Status & s);

        static Status Ok() {return Status();}

        static Status NotFound(const Slice& msg,const Slice& msg2 = Slice()) {
            return Status(kNotFound,msg,msg2);
        }

        static Status Corruption (Slice& msg,const Slice& msg2 = Slice()) {
            return Status(kCorruption,msg,msg2);
        }


        static Status NotSupported(const Slice& msg, const Slice& msg2 = Slice()) {
            return Status(kNotSupported, msg, msg2);
        }

        static Status InvalidArgument(const Slice& msg, const Slice& msg2 = Slice()) {
            return Status(kInvalidArgument, msg, msg2);
        }
        static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) {
            return Status(kIOError, msg, msg2);
        }
        bool ok() const { return (state_ == NULL); }

        bool IsNotFound() const { return code() == kNotFound; }

        bool IsCorruption() const { return code() == kCorruption; }

        bool IsIOError() const { return code() == kIOError; }

        string ToString() const;

    private:
        const char* state_;

        enum Code {
            kOk = 0,
            kNotFound = 1,
            kCorruption = 2,
            kNotSupported = 3,
            kInvalidArgument = 4,
            kIOError = 5,
        };

        Code code() const {
            return (state_ == NULL)?kOk:static_cast<Code>(state_[4]);
        }

        static const char* CopyState(const char * s);
        Status(Code code, const Slice& msg, const Slice& msg2);

        inline void operator= (const Status& s) {
            if (state_ != s.state_) {
                delete[] state_;
                state_ = (s.state_ == NULL)? NULL:CopyState(s.state_);
            }
        }
};

#endif
