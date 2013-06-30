#ifndef _CHAT_MANAGER_H_
#define _CHAT_MANAGER_H_

class CChatManager
{
    public:
        virtual void send_message(int sock_id);
};

#endif
