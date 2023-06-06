package com.example.wxhk.msg;

import com.example.wxhk.constant.WxMsgType;
import com.example.wxhk.model.PrivateChatMsg;
import com.example.wxhk.model.dto.PayoutInformation;
import com.example.wxhk.server.WxSmgServer;
import com.example.wxhk.tcp.vertx.InitWeChat;
import jakarta.annotation.PostConstruct;
import org.dromara.hutool.core.util.XmlUtil;
import org.dromara.hutool.log.Log;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import java.math.BigDecimal;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.ReentrantReadWriteLock;

@Component
public class WxMsgHandle {
    public static final ConcurrentHashMap<Integer, Handle> map = new ConcurrentHashMap<>(32);
    protected static final Log log = Log.get();
    /**
     * 文件传输助手
     */
    public static final String FILEHELPER = "filehelper";
    /**
     * 收款码缓存 因为有2段信息,一段是交易id,里面可以解析出来源方,二段解析出金额
     */
    public static ConcurrentHashMap<String, String> collection_code_caching = new ConcurrentHashMap<>();


    public static WxSmgServer wxSmgServer;
    /**
     * 看
     */
    public static final ReentrantReadWriteLock LOOK = new ReentrantReadWriteLock();

    @Autowired
    public void setWxSmgServer(WxSmgServer wxSmgServer) {
        WxMsgHandle.wxSmgServer = wxSmgServer;
    }

    @PostConstruct
    public void init() {
        add(chatMsg -> {
            wxSmgServer.私聊(chatMsg);
            return null;
        }, WxMsgType.私聊信息);
        add(chatMsg -> {
            if (FILEHELPER.equals(chatMsg.getFromUser())) {
                wxSmgServer.文件助手(chatMsg);
            }
            return 1;
        }, WxMsgType.收到转账之后或者文件助手等信息);
        add(chatMsg -> {
            wxSmgServer.收到名片(chatMsg);
            return 1;
        }, WxMsgType.收到名片);
        add(chatMsg -> {
            wxSmgServer.收到好友请求(chatMsg);
            return 1;
        }, WxMsgType.好友请求);// 好友请求
        add(chatMsg -> {
            boolean f = 解析扫码支付第二段(chatMsg);
            if (f) {
                f = 解析收款信息1段(chatMsg);
                if (f) {
                    解析收款信息2段(chatMsg);
                }
            }
            return null;
        }, WxMsgType.转账和收款);
        add(chatMsg -> {
            boolean f = 解析扫码支付第一段(chatMsg);
            return null;
        }, WxMsgType.扫码触发);
    }

    /**
     * 解析扫码支付第一段,得到交易id和微信id
     *
     * @param chatMsg
     * @return boolean 返回true 则继续解析,否则解析成功,不需要解析了
     */
    public static boolean 解析扫码支付第一段(PrivateChatMsg chatMsg) {
        try {
            Document document = XmlUtil.parseXml(chatMsg.getContent());
            Element documentElement = document.getDocumentElement();
            String localName = documentElement.getLocalName();
            if ("sysmsg".equals(localName)) {
                String type = documentElement.getAttribute("type");
                if ("paymsg".equals(type)) {
                    NodeList outtradeno = documentElement.getElementsByTagName("outtradeno");
                    if (outtradeno.getLength() > 0) {
                        String textContent = outtradeno.item(0).getTextContent();
                        String textContent1 = documentElement.getElementsByTagName("username").item(0).getTextContent();
                        collection_code_caching.put(textContent, textContent1);
                        return false;
                    }
                }
            }
        } catch (Exception e) {
            log.error(e);
        }
        return true;
    }


    /**
     * 解析扫码支付第二段
     *
     * @param chatMsg 聊天味精
     * @return boolean true 则 继续解析, false则解析成功,不需要再解析了
     */
    public static boolean 解析扫码支付第二段(PrivateChatMsg chatMsg) {
        try {
            Document document = XmlUtil.parseXml(chatMsg.getContent());
            Element documentElement = document.getDocumentElement();
            String localName = documentElement.getLocalName();
            if ("msg".equals(localName)) {
                NodeList outtradeno = documentElement.getElementsByTagName("weapp_path");
                if (outtradeno.getLength() > 1) {
                    String textContent = outtradeno.item(1).getTextContent();
                    Set<Map.Entry<String, String>> entries = collection_code_caching.entrySet();
                    Iterator<Map.Entry<String, String>> iterator = entries.iterator();
                    while (iterator.hasNext()) {
                        Map.Entry<String, String> next = iterator.next();
                        if (textContent.contains(next.getKey())) {
                            // 得到了交易信息
                            NodeList word = documentElement.getElementsByTagName("word");
                            String monery = word.item(1).getTextContent();
                            String remark = word.item(3).getTextContent();
                            if (monery.startsWith("￥")) {
                                String substring = monery.substring(1);
                                BigDecimal decimal = new BigDecimal(substring);
                                log.info("扫码收款:{},付款人:{},付款备注:{}", decimal.stripTrailingZeros().toPlainString(), next.getValue(), remark);
                                wxSmgServer.扫码收款(new PayoutInformation(next.getValue(),decimal,remark));
                                iterator.remove();
                                return false;
                            }
                        }
                    }


                }
            }
        } catch (Exception e) {
            log.error(e);
        }
        return true;
    }

    public static boolean 解析收款信息2段(PrivateChatMsg chatMsg) {
        try {
            Document document = XmlUtil.parseXml(chatMsg.getContent());
            Element documentElement = document.getDocumentElement();
            String localName = documentElement.getLocalName();
            if ("msg".equals(localName)) {
                if (documentElement.getElementsByTagName("transcationid").getLength() > 0) {
                    String remark = documentElement.getElementsByTagName("pay_memo").item(0).getTextContent();
                    String monery = documentElement.getElementsByTagName("feedesc").item(0).getTextContent();
                    String receiver_username = documentElement.getElementsByTagName("receiver_username").item(0).getTextContent();
                    // 如果是机器人发出的,则跳过解析
                    if (InitWeChat.WXID_MAP.contains(receiver_username) ) {
                       return false;
                    }
                    if (monery.startsWith("￥")) {
                        String substring = monery.substring(1);
                        BigDecimal decimal = new BigDecimal(substring);
                        log.info("收款:{},付款人:{},付款备注:{}", decimal.stripTrailingZeros().toPlainString(), chatMsg.getFromUser(), remark);
                        wxSmgServer.收款之后(new PayoutInformation(chatMsg.getFromUser(), decimal, remark));
                        return false;
                    };

                }
            }
        } catch (Exception e) {
            log.error(e);
        }
        return true;
    }


    /**
     * 解析收款信息1段
     * <b>会自动进行收款</b>
     *
     * @param chatMsg
     * @return boolean true则 继续解析,false则不需要解析了
     */
    public static boolean 解析收款信息1段(PrivateChatMsg chatMsg) {
        try {
            String content = chatMsg.getContent();
            Document document = XmlUtil.parseXml(content);
            NodeList paysubtype1 = document.getElementsByTagName("paysubtype");
            if (paysubtype1.getLength() == 0) {
                return true;
            }
            Node paysubtype = paysubtype1.item(0);
            if ("1".equals(paysubtype.getTextContent().trim())) {
                // 手机发出去的
                String textContent = document.getElementsByTagName("receiver_username").item(0).getTextContent();
                if (!InitWeChat.WXID_MAP.contains(textContent)) {
                    // 如果不是机器人收款,则认为不需要解析了,大概率是机器人自己发出去的
                    return false;
                }

                String remark = document.getElementsByTagName("pay_memo").item(0).getTextContent();
                String monery = document.getElementsByTagName("feedesc").item(0).getTextContent();
                String receiver_username = document.getElementsByTagName("receiver_username").item(0).getTextContent();
                if (monery.startsWith("￥")) {
                    String substring = monery.substring(1);
                    BigDecimal decimal = new BigDecimal(substring);
                    Node transcationid = document.getDocumentElement().getElementsByTagName("transcationid").item(0);
                    Node transferid = document.getDocumentElement().getElementsByTagName("transferid").item(0);
                    wxSmgServer.接到收款(new PayoutInformation(chatMsg.getFromUser(), decimal, remark, transcationid.getTextContent(), transferid.getTextContent()));
                    return false;
                }
            }

        } catch (Exception e) {
            log.error(e);
        }
        return true;
    }

    public static void exec(PrivateChatMsg chatMsg) {
        Handle handle = map.get(chatMsg.getType());
        if (handle != null) {
            handle.handle(chatMsg);
        }
    }

    public void add(Handle handle, WxMsgType... type) {
        for (WxMsgType integer : type) {
            map.put(integer.getType(), handle);
        }
    }

    public interface Handle {
        Object handle(PrivateChatMsg chatMsg);
    }
}
