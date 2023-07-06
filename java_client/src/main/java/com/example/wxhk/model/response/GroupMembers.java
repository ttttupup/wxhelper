package com.example.wxhk.model.response;

import com.example.wxhk.infe.Resp;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import lombok.Data;
import lombok.experimental.Accessors;

import java.io.Serializable;

@Data
@Accessors(chain = true)
@JsonIgnoreProperties(ignoreUnknown = true)
public class GroupMembers implements Resp {

    /**
     * code : 1
     * data : {"admin":"wxid_gf1fogt5a0pq22","chatRoomId":"24964676359@chatroom","members":"wxid_gf1fogt5a0pq22^Gwxid_4yr8erik0uho22"}
     * result : OK
     */

    private Integer code;
    private DataBean data;
    private String result;

    @Data
    public static class DataBean implements Serializable {
        /**
         * admin : wxid_gf1fogt5a0pq22
         * chatRoomId : 24964676359@chatroom
         * members : wxid_gf1fogt5a0pq22^Gwxid_4yr8erik0uho22
         */

        private String admin;
        private String chatRoomId;
        private String members;
    }
}
