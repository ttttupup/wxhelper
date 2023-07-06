package com.example.wxhk.model.response;

import com.example.wxhk.infe.Resp;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import lombok.Data;
import lombok.experimental.Accessors;

import java.io.Serializable;
import java.util.List;

/**
 * 联系人列表
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
@JsonIgnoreProperties(ignoreUnknown = true)
public class ContactList implements Resp {

    /**
     * code : 1
     * data : [{"customAccount":"","delFlag":0,"type":1,"userName":"朋友推荐消息","verifyFlag":0,"wxid":"fmessage"},{"customAccount":"tencent_cloud","delFlag":0,"type":3,"userName":"腾讯云助手","verifyFlag":24,"wxid":"gh_a73e2407e0f8"},{"customAccount":"","delFlag":0,"type":1,"userName":"语音记事本","verifyFlag":0,"wxid":"medianote"},{"customAccount":"","delFlag":0,"type":1,"userName":"漂流瓶","verifyFlag":0,"wxid":"floatbottle"},{"customAccount":"jys-wt","delFlag":0,"type":8651011,"userName":"时光似水戏流年","verifyFlag":0,"wxid":"wxid_gf1fogt5a0pq22"},{"customAccount":"wxzhifu","delFlag":0,"type":3,"userName":"微信支付","verifyFlag":24,"wxid":"gh_3dfda90e39d6"},{"customAccount":"dhkzfr","delFlag":0,"type":3,"userName":"阿芙（代发）","verifyFlag":0,"wxid":"wxid_kh16lri40gzj22"},{"customAccount":"","delFlag":0,"type":3,"userName":"文件传输助手","verifyFlag":0,"wxid":"filehelper"},{"customAccount":"","delFlag":0,"type":3,"userName":"fff","verifyFlag":0,"wxid":"24964676359@chatroom"},{"customAccount":"","delFlag":0,"type":2,"userName":"最美阿芙","verifyFlag":0,"wxid":"23793178249@chatroom"},{"customAccount":"afu943344","delFlag":0,"type":2,"userName":"A-阿芙4号-LOL永劫云顶出租-代发","verifyFlag":0,"wxid":"wxid_1gxthknqbmwv22"},{"customAccount":"","delFlag":0,"type":3,"userName":"微信收款助手","verifyFlag":24,"wxid":"gh_f0a92aa7146c"},{"customAccount":"","delFlag":0,"type":0,"userName":"","verifyFlag":0,"wxid":"25984984710827869@openim"}]
     * result : OK
     */

    private Integer code;
    private String result;
    private List<DataBean> data;

    @Data
    @Accessors(chain = true)
    public static class DataBean implements Serializable {
        /**
         * customAccount :
         * delFlag : 0
         * type : 1
         * userName : 朋友推荐消息
         * verifyFlag : 0
         * wxid : fmessage
         */

        private String customAccount;
        private Integer delFlag;
        private Integer type;
        private String userName;
        private Integer verifyFlag;
        private String wxid;
    }
}
