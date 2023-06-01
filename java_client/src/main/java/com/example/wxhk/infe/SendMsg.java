package com.example.wxhk.infe;

import io.vertx.core.json.JsonObject;

/**
 * http接口请求的基础接口
 *
 * @author wt
 * @date 2023/06/01
 */
public interface SendMsg<T> extends java.io.Serializable{

    default JsonObject toJson(){
        return JsonObject.mapFrom(this);
    }

}
