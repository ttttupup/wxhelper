import * as wxhelper from '../src/index'
import net from 'net'
import {
    writeMsgStore,
    readMsgStore
} from '../src/utils/messageStore'

// 启动一个TCP服务，接收hook消息
const server = net.createServer((socket: any) => {
    console.log('New client connected')
    let messageStore = readMsgStore()

    let data = Buffer.from('')

    socket.on('data', (data: any, chunk: any) => {
        console.log(`Received data: ${data}`)
        const dataJson = JSON.parse(data)
        messageStore = writeMsgStore(messageStore, dataJson)
        // data = Buffer.concat([data, chunk])
        // console.log(`Received data: ${data}`)
    })

    socket.on('end', () => {
        const decodedData = data.toString('utf8')
        console.log(`Received data: ${decodedData}`)
    })

    socket.on('close', () => {
        console.log('Client disconnected')
    })
})

const wsPort = 19099

const main = async () => {
    // server.listen(wsPort, () => {
    //     console.log(`Server listening on port ${wsPort}`)
    // })
    const testContact0 = 'filehelper'
    const testContact1 = 'ledongmao'
    const testRoom = '1234@chatroom'
    const testRoom1 = '5678@chatroom'

    const checkLoginRes = await wxhelper.checkLogin()
    console.log('登录状态：', checkLoginRes.data)
    if (checkLoginRes.data.msg !== 'success') {
        console.log('请先登录')
        return
    } else {
        const port = String(wsPort)
        const ip = '127.0.0.1'
        const url = ''
        const timeout = '3000'
        const enableHttp = '0'
        const hookSyncMsgRes = await wxhelper.hookSyncMsg(port, ip, url, timeout, enableHttp)
        console.log('开启hook：', hookSyncMsgRes.data)
    }
    const userInfoRes = await wxhelper.userInfo()
    console.log('当前账号信息：', userInfoRes.data)
    const selfWxid = userInfoRes.data.data.wxid
    console.log('当前账号wxid：', selfWxid)
    // const sendTextMsgRes = await wxhelper.sendTextMsg('filehelper', new Date().toLocaleString() + '：你好，我是机器人')
    // console.log('发送文本消息：', sendTextMsgRes.data)

    // const getContactListRes = await wxhelper.getContactList()
    // console.log('获取联系人列表：', getContactListRes.data.data.length)

    // const getDBInfo = await wxhelper.getDBInfo()
    // console.log('获取数据库信息：', getDBInfo.data)

    // const sqliteDB = await wxhelper.execSql(2737761293968, 'select * from MSG where localId =301;')
    // console.log('查询数据库：', sqliteDB.data)

    // const roomInfoRes = await wxhelper.getChatRoomDetailInfo(testRoom1)
    // console.log('获取群聊详情：', roomInfoRes.data)

    // const addMemberToChatRoomRes = await wxhelper.addMemberToChatRoom(testRoom, testContact1)
    // console.log('添加群成员：', addMemberToChatRoomRes.data)

    // const modifyChatRoomNameRes = await wxhelper.modifyNickname(testRoom, selfWxid, '大师')
    // console.log('修改自己的群名片：', modifyChatRoomNameRes.data)

    // const delMemberFromChatRoomRes = await wxhelper.delMemberFromChatRoom(testRoom, testContact1)
    // console.log('删除群成员：', delMemberFromChatRoomRes.data)

    // const getChatRoomMembersRes = await wxhelper.getMemberFromChatRoom(testRoom)
    // console.log('获取群成员：', getChatRoomMembersRes.data)

    // const quitChatRoomRes = await wxhelper.quitChatRoom(testRoom1)
    // console.log('退出群聊：', quitChatRoomRes.data)

    // const getSNSFirstPage = await wxhelper.getSNSFirstPage()
    // console.log('获取朋友圈：', getSNSFirstPage.data)

    // const getSNSNextPage = await wxhelper.getSNSNextPage()
    // console.log('获取朋友圈下一页：', getSNSNextPage.data)

    // const sendAtTextMsgRes = await wxhelper.sendAtText(testContact1, testRoom, '你好，我是机器人')
    // console.log('发送@消息：', sendAtTextMsgRes.data)

    // const getContactProfileRes = await wxhelper.getContactProfile(testContact1)
    // console.log('获取联系人详情：', getContactProfileRes.data)

    // const param = {
    //     appName: '123',
    //     userName: '超哥',
    //     title: '测试转发公众号消息',
    //     url: 'https://mp.weixin.qq.com/s?__biz=MzIwMzYwMTk1NA==&mid=2247483663&idx=1&sn=',
    //     thumbUrl: 'https://mmbiz.qpic.cn/mmbiz_jpg/3ic3Zz3',
    //     digest: '测试',
    //     wxid: testContact1,
    // }

    // const forwardPublicMsgRes = await wxhelper.forwardPublicMsg(param)
    // console.log('转发公众号消息：', forwardPublicMsgRes.data)

    // const payload = {
    //     "wxid": testContact1,
    //     "waidConcat": "wxaf35009675aa0b2a_118",
    //     "waid": "wxaf35009675aa0b2a",
    //     "appletWxid": "gh_7a5c4141778f@app",
    //     "jsonParam": "{\"current_path\":\"home/pages/index.html\",\"current_title\":\"\",\"image_url\":\"https://ut-static.udache.com/webx/mini-pics/U7mDFxU2yh-2-r1BJ-J0X.png\",\"scene\":1001,\"scene_note\":\"\",\"sessionId\":\"SessionId@1672284921_1#1692848476899\"}",
    //     "headImgUrl": "http://mmbiz.qpic.cn/sz_mmbiz_png/9n47wQlh4dH8afD9dQ9uQicibRm5mYz3lawXCLMjmnzFicribH51qsFYxjzPEcTGHGmgX4lkAkQ3jznia8UDEtqsX1w/640?wx_fmt=png&wxfrom=200",
    //     "mainImg": "C:\\wxid_123123\\Applet\\wxaf35009675aa0b2a\\temp\\2.png",
    //     "indexPage": "pages/index/index.html"
    // }
    // const sendAppMsgRes = await wxhelper.sendApplet(payload)
    // console.log('发送小程序消息：', sendAppMsgRes.data)

    // const sendPatMsgRes = await wxhelper.sendPatMsg(testRoom1, testContact1)
    // console.log('拍一拍：', sendPatMsgRes.data)

    // const ocrRes = await wxhelper.ocr('https://ut-static.udache.com/webx/mini-pics/U7mDFxU2yh-2-r1BJ-J0X.png')
    // console.log('OCR识别：', ocrRes.data)
}

void main()