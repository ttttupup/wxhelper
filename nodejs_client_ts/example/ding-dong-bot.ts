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
    const sendTextMsgRes = await wxhelper.sendTextMsg('filehelper', new Date().toLocaleString() + '：你好，我是机器人')
    console.log('发送文本消息：', sendTextMsgRes.data)
}

void main()