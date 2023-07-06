package com.example.wxhk.util;

import com.example.wxhk.model.PrivateChatMsg;
import com.example.wxhk.model.request.*;
import com.example.wxhk.model.response.ContactList;
import com.example.wxhk.model.response.GroupMembers;
import com.example.wxhk.tcp.vertx.ArrHandle;
import com.example.wxhk.tcp.vertx.InitWeChat;
import io.vertx.core.json.JsonObject;
import org.dromara.hutool.core.util.XmlUtil;
import org.dromara.hutool.log.Log;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

/**
 * 常见方法
 *
 * @author wt
 * @date 2023/05/29
 */
public class HttpSendUtil {

    protected static final Log log = Log.get();

    public static JsonObject 通过好友请求(PrivateChatMsg msg) {
        Document document = XmlUtil.parseXml(msg.getContent());
        String encryptusername = document.getDocumentElement().getAttribute("encryptusername");
        String ticket = document.getDocumentElement().getAttribute("ticket");
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.通过好友申请, new JsonObject().put("v3", encryptusername).put("v4", ticket).put("permission", "0"));
    }

    public static JsonObject 确认收款(PrivateChatMsg msg) {
        try {
            String content = msg.getContent();
            Document document = XmlUtil.parseXml(content);
            Node paysubtype = document.getElementsByTagName("paysubtype").item(0);
            if ("1".equals(paysubtype.getTextContent().trim())) {
                // 手机发出去的
                String textContent = document.getElementsByTagName("receiver_username").item(0).getTextContent();
                if (!InitWeChat.WXID_MAP.contains(textContent)) {
                    return new JsonObject().put("spick", true);
                }
                Node transcationid = document.getDocumentElement().getElementsByTagName("transcationid").item(0);
                Node transferid = document.getDocumentElement().getElementsByTagName("transferid").item(0);
                return HttpSyncUtil.exec(HttpAsyncUtil.Type.确认收款, new JsonObject().put("wxid", msg.getFromUser())
                        .put("transcationId", transcationid.getTextContent())
                        .put("transferId", transferid.getTextContent()));

            }
            // 如果是确认接受收款,则跳过
            return new JsonObject();

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }


    public static JsonObject 发送文本(String wxid, String msg) {
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.发送文本, JsonObject.mapFrom(new SendMsg().setMsg(msg).setWxid(wxid)));
    }

    public static JsonObject 发送文本(String msg) {
        return 发送文本(ArrHandle.getPriMsg().getFromUser(), msg);
    }

    public static JsonObject 发送at文本(String chatRoomId, String wxids, String msg) {
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.发送at文本, JsonObject.mapFrom(new SendMsg().setMsg(msg).setWxids(wxids).setChatRoomId(chatRoomId)));
    }

    public static JsonObject 发送at文本(String wxids, String msg) {
        return 发送at文本(ArrHandle.getPriMsg().getFromGroup(), wxids, msg);
    }

    public static JsonObject 发送图片(String wxid, String msg) {
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.发送图片, JsonObject.mapFrom(new SendMsg().setImagePath(msg).setWxid(wxid)));
    }

    public static JsonObject 发送图片(String msg) {
        return 发送图片(ArrHandle.getPriMsg().getFromUser(), msg);
    }

    public static JsonObject 发送文件(String wxid, String msg) {
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.发送文件, JsonObject.mapFrom(new SendMsg().setFilePath(msg).setWxid(wxid)));
    }

    public static JsonObject 发送文件(String msg) {
        return 发送文件(ArrHandle.getPriMsg().getFromUser(), msg);
    }

    public static JsonObject 添加好友(AddFriends p) {
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.添加好友, p.toJson());
    }


    public static String 获取当前登陆微信id() {
        JsonObject exec = HttpSyncUtil.exec(HttpAsyncUtil.Type.获取登录信息, new JsonObject());
        return exec.getJsonObject("data").getString("wxid");
    }

    public static ContactList 联系人列表(){
        JsonObject exec = HttpSyncUtil.exec(HttpAsyncUtil.Type.联系人列表, new JsonObject());
        return exec.mapTo(ContactList.class);
    }
    public static JsonObject 开启hook(OpenHook hook){
        JsonObject exec = HttpSyncUtil.exec(HttpAsyncUtil.Type.开启hook,hook.toJson());
        return exec;
    }
    public static JsonObject 关闭hook(){
        JsonObject exec = HttpSyncUtil.exec(HttpAsyncUtil.Type.关闭hook,new JsonObject());
        return exec;
    }

    public static GroupMembers 获取群成员(GetGroupMembers p){
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.获取群成员, p.toJson()).mapTo(GroupMembers.class);

    }


    public static JsonObject 确认收款(ConfirmThePayment payment){
        return HttpSyncUtil.exec(HttpAsyncUtil.Type.确认收款, payment.toJson());
    }


    @Deprecated
    public static com.example.wxhk.infe.SendMsg of(HttpAsyncUtil.Type type) {
        switch (type) {

            case 检查微信登陆 -> {

            }
            case 获取登录信息 -> {
            }
            case 发送文本 -> {
                return new SendText();
            }
            case 发送at文本 -> {
                return new SendAtText();
            }
            case 发送图片 -> {
                return new SendImg();
            }
            case 发送文件 -> {
                return new SendFile();
            }

        }
        return new SendText();
    }

}
