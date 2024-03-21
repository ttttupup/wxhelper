// 封装get和post请求
import axios from 'axios'

// 创建 axios 实例
const request = axios.create({
  // API 请求的默认前缀
  baseURL: 'http://127.0.0.1:19088',

  // 请求超时时间
  timeout: 120000,
})

/**
* 异常拦截处理器
*
* @param {*} error
*/
const errorHandler = (error: { response?: { status: number, config: any } }) => {
  // 判断是否是响应错误信息
  if (error.response) {
    if (error.response.status === 401) {
      return request(error.response.config)
    }
  }
  return Promise.reject(error)
}

/**
 * GET 请求
 *
 * @param {String} url
 * @param {Object} data
 * @param {Object} options
 * @returns {Promise<any>}
 */
export const get = (url: string, data = {}, options = {}) => {

  // request.interceptors.response.use((response) => response.data, errorHandler)
  return request({
    url,
    params: data,
    method: 'get',
    ...options,
  })
}

/**
 * POST 请求
 *
 * @param {String} url
 * @param {Object} data
 * @param {Object} options
 * @returns {Promise<any>}
 */
export const post = (url: string, data = {}, options = {}) => {
  // request.interceptors.response.use((response) => response.data, errorHandler)
  return request({
    url,
    method: 'post',
    data,
    ...options,
  })
}