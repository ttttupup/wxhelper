package com.example.wxhk.util;

import com.example.wxhk.model.request.GetGroupMembers;
import com.example.wxhk.model.response.ContactList;
import com.example.wxhk.model.response.GroupMembers;
import org.dromara.hutool.core.lang.Console;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;

import java.time.Duration;
import java.time.LocalDateTime;
import java.util.List;

@SpringBootTest
class HttpSendUtilTest {


    @Test
    void 获取当前登陆微信id() {
        String s = HttpSendUtil.获取当前登陆微信id();
    }

    @Test
    void 联系人列表() {
        ContactList contactList = HttpSendUtil.联系人列表();

        List<ContactList.DataBean> data = contactList.getData();
        for (ContactList.DataBean datum : data) {
            Console.log(datum.getWxid(),datum.getUserName());
        }
        Console.log(contactList);
    }
    @Test
    void 开启hook() {

    }
    @Test
    void 关闭ook() {
        HttpSendUtil.关闭hook();
    }

    @Test
    void 获取群成员() {
        GroupMembers 获取群成员 = HttpSendUtil.获取群成员(new GetGroupMembers().setChatRoomId("24964676359@chatroom"));
        Console.log(获取群成员);

        Duration between = Duration.between(LocalDateTime.now(), LocalDateTime.now());
    }
}