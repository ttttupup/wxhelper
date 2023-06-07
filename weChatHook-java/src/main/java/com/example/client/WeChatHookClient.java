package com.example.client;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import lombok.SneakyThrows;
import org.jsoup.Connection;
import org.jsoup.Jsoup;

import java.util.HashMap;
import java.util.Map;

/**
 * @PACKAGE_NAME: com.example.client
 * @NAME: WeChatHookClient
 * @AUTHOR: wxs
 * @DATE: 2023/5/31 15:08
 * @PROJECT_NAME: WeChatHook-java
 **/
public class WeChatHookClient {

    private static final String apiPath = "http://127.0.0.1:19088/api/";

    public static void main(String[] args) {
        System.out.println(check_login());
    }

    /**
     * 检查是否登录
     *
     * @return
     */
    public static JSONObject check_login() {
        String url = apiPath + "?type=0";
        JSONObject response = post(url, null);
        return response;
    }

    /**
     * 登录用户信息
     *
     * @return
     */
    public static JSONObject user_info() {
        String url = apiPath + "?type=8";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 发送文本
     *
     * @param wxid
     * @param msg
     * @return
     */
    public static JSONObject send_text(String wxid, String msg) {
        String url = apiPath + "?type=2";
        Map<String, Object> map = new HashMap<>();
        map.put("wxid", wxid);
        map.put("msg", msg);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 发送@消息
     *
     * @param chatRoomId
     * @param wxids      notify@all
     * @param msg
     * @return
     */
    public static JSONObject send_at(String chatRoomId, String wxids, String msg) {
        String url = apiPath + "?type=3";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("wxids", wxids);
        map.put("msg", msg);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 发送图片
     *
     * @param wxid
     * @param imagePath C:/Users/ww/Downloads/素材图片 (4).jpg
     * @return
     */
    public static JSONObject send_img(String wxid, String imagePath) {
        String url = apiPath + "?type=5";
        Map<String, Object> map = new HashMap<>();
        map.put("wxid", wxid);
        map.put("imagePath", imagePath);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 发送文件
     *
     * @param wxid
     * @param filePath C:/test.txt
     * @return
     */
    public static JSONObject send_file(String wxid, String filePath) {
        String url = apiPath + "?type=6";
        Map<String, Object> map = new HashMap<>();
        map.put("wxid", wxid);
        map.put("filePath", filePath);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * hook 消息
     *
     * @param ip
     * @param port
     * @return
     */
    public static JSONObject hook_msg(String ip, String port) {
        String url = apiPath + "?type=9";
        Map<String, Object> map = new HashMap<>();
        map.put("ip", ip);
        map.put("port", port);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 取消消息hook
     *
     * @return
     */
    public static JSONObject unhook_msg() {
        String url = apiPath + "?type=10";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * hook 图片
     *
     * @return
     */
    public static JSONObject hook_img(String imgDir) {
        String url = apiPath + "?type=11";
        Map<String, Object> map = new HashMap<>();
        map.put("imgDir", imgDir);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 取消hook 图片
     *
     * @param imgDir C:\img
     * @return
     */
    public static JSONObject unhook_img(String imgDir) {
        String url = apiPath + "?type=12";
        Map<String, Object> map = new HashMap<>();
        map.put("imgDir", imgDir);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * hook 语音
     *
     * @return
     */
    public static JSONObject hook_voice(Long msgId) {
        String url = apiPath + "?type=56";
        Map<String, Object> map = new HashMap<>();
        map.put("msgId", msgId);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 取消hook 语音
     *
     * @return
     */
    public static JSONObject unhook_voice() {
        String url = apiPath + "?type=14";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 删除好友
     *
     * @param wxid
     * @return
     */
    public static JSONObject del_friend(String wxid) {
        String url = apiPath + "?type=17";
        Map<String, Object> map = new HashMap<>();
        map.put("wxid", wxid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 网络搜素用户
     *
     * @return
     */
    public static JSONObject search_friend(String keyword) {
        String url = apiPath + "?type=19";
        Map<String, Object> map = new HashMap<>();
        map.put("keyword", keyword);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 添加好友
     *
     * @param wxid
     * @return
     */
    public static JSONObject add_friend(String wxid) {
        String url = apiPath + "?type=20";
        Map<String, Object> map = new HashMap<>();
        map.put("wxid", wxid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 群成员
     *
     * @param chatRoomId
     * @return
     */
    public static JSONObject fetch_chat_room_members(String chatRoomId) {
        String url = apiPath + "?type=25";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 群成员昵称
     *
     * @param chatRoomId
     * @param memberId
     * @return
     */
    public static JSONObject get_member_nickname(String chatRoomId, String memberId) {
        String url = apiPath + "?type=26";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("memberId", memberId);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 删除群成员
     *
     * @param chatRoomId
     * @param memberIds
     * @return
     */
    public static JSONObject del_member(String chatRoomId, String memberIds) {
        String url = apiPath + "?type=27";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("memberIds", memberIds);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 增加群成员
     *
     * @param chatRoomId
     * @param memberIds
     * @return
     */
    public static JSONObject add_member(String chatRoomId, String memberIds) {
        String url = apiPath + "?type=28";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("memberIds", memberIds);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 修改群昵称
     *
     * @param chatRoomId
     * @param wxid
     * @param nickName
     * @return
     */
    public static JSONObject modify_room_name(String chatRoomId, String wxid, String nickName) {
        String url = apiPath + "?type=31";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("wxid", wxid);
        map.put("nickName", nickName);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 获取sqlite3的操作句柄
     *
     * @return
     */
    public static JSONObject get_db_handlers() {
        String url = apiPath + "?type=32";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 查询数据库
     *
     * @param dbHandle
     * @param sql
     * @return
     */
    public static JSONObject query_db_by_sql(String dbHandle, String sql) {
        String url = apiPath + "?type=34";
        Map<String, Object> map = new HashMap<>();
        map.put("dbHandle", dbHandle);
        map.put("sql", sql);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * hook 日志
     *
     * @return
     */
    public static JSONObject hook_log() {
        String url = apiPath + "?type=36";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 取消hook日志
     *
     * @return
     */
    public static JSONObject unhook_log() {
        String url = apiPath + "?type=37";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 转发消息
     *
     * @param wxid
     * @param msgid
     * @return
     */
    public static JSONObject forward(String wxid, String msgid) {
        String url = apiPath + "?type=40";
        Map<String, Object> map = new HashMap<>();
        map.put("wxid", wxid);
        map.put("msgid", msgid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 退出登录
     *
     * @return
     */
    public static JSONObject logout() {
        String url = apiPath + "?type=44";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 确认收款
     *
     * @param wxid
     * @param transcationId
     * @param transferId
     * @return
     */
    public static JSONObject confirm_receipt(String wxid, String transcationId, String transferId) {
        String url = apiPath + "?type=45";
        Map<String, Object> map = new HashMap<>();
        map.put("wxid", wxid);
        map.put("transcationId", transcationId);
        map.put("transferId", transferId);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 好友列表
     *
     * @return
     */
    public static JSONObject contact_list() {
        String url = apiPath + "?type=46";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 群详情
     *
     * @param chatRoomId
     * @return
     */
    public static JSONObject room_detail(String chatRoomId) {
        String url = apiPath + "?type=47";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * ocr提取文字
     *
     * @param imagePath
     * @return
     */
    public static JSONObject ocr(String imagePath) {
        String url = apiPath + "?type=49";
        Map<String, Object> map = new HashMap<>();
        map.put("imagePath", imagePath);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 拍一拍
     *
     * @param chatRoomId
     * @param wxid
     * @return
     */
    public static JSONObject pat(String chatRoomId, String wxid) {
        String url = apiPath + "?type=50";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("wxid", wxid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 消息置顶
     *
     * @param chatRoomId
     * @param msgid
     * @return
     */
    public static JSONObject top_msg(String chatRoomId, Long msgid) {
        String url = apiPath + "?type=51";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("msgid", msgid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 取消置顶
     *
     * @param chatRoomId
     * @param msgid
     * @return
     */
    public static JSONObject close_top_msg(String chatRoomId, Long msgid) {
        String url = apiPath + "?type=52";
        Map<String, Object> map = new HashMap<>();
        map.put("chatRoomId", chatRoomId);
        map.put("msgid", msgid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 朋友圈首页
     *
     * @return
     */
    public static JSONObject sns_first() {
        String url = apiPath + "?type=53";
        JSONObject response = post(url, null);
        System.out.println(response);
        return response;
    }

    /**
     * 朋友圈下一页
     *
     * @param snsId
     * @return
     */
    public static JSONObject sns_next(String snsId) {
        String url = apiPath + "?type=54";
        Map<String, Object> map = new HashMap<>();
        map.put("snsId", snsId);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 查询联系人或群名称
     *
     * @param wxid 微信id
     * @return
     */
    public static JSONObject query_nickname(String wxid) {
        String url = apiPath + "?type=55";
        Map<String, Object> map = new HashMap<>();
        map.put("id", wxid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 下载消息附件
     *
     * @param msgId
     * @return
     */
    public static JSONObject download_msg_attach(Long msgId) {
        String url = apiPath + "?type=56";
        Map<String, Object> map = new HashMap<>();
        map.put("msgId", msgId);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    /**
     * 获取群/群成员信息
     *
     * @param wxid
     * @return
     */
    public static JSONObject get_member_info(String wxid) {
        String url = apiPath + "?type=57";
        Map<String, String> map = new HashMap<>();
        map.put("wxid", wxid);
        JSONObject response = post(url, JSON.toJSONString(map));
        System.out.println(response);
        return response;
    }

    @SneakyThrows
    public static JSONObject post(String url, String json) {
        String body = Jsoup.connect(url)
                .method(Connection.Method.POST)
                .header("Content-Type", "application/json")
                .requestBody(json)
                .execute().body();
        return JSON.parseObject(body);
    }

    @SneakyThrows
    public static JSONObject hook(String url, String json) {
        String body = Jsoup.connect(url)
                .data("msg",json)
                .method(Connection.Method.POST)
                .timeout(1000)
                .execute().body();
        return JSON.parseObject(body);
    }

}
