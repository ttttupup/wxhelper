import fs from 'fs'

export const writeMsgStore = (messageStore: { [key: string]: any }, msg: { type: number, content?:any }) => {
    // 检测根目录下是否有msgStore.json文件，如果没有，则创建一个，内容为{}
    if (!fs.existsSync('msgStore.json')) {
        console.log('msgStore.json not exist')
        fs.writeFileSync('msgStore.json', '{}')
    }
    let type = String(msg.type)
    console.info('ws message hook:', type)
    // log.info(JSON.stringify(j, undefined, 2))

    if (msg.content) {
        try {
          const content = msg.content
          // 从content中判断是否存在类似<type>6</type>的格式，并从其中取出type的值
          const m = content.match(/<type>(\d+)<\/type>/)
          if (m != null) {
            type = type + '_' + m[1]
          }
        } catch (e) {
          console.error('ws message hook error:', e)
        }
      }

    if (type === '10000') {
        const list10000 = messageStore['10000'] || []
        list10000.push(msg)
        messageStore[type] = list10000
    } else {
        messageStore[type] = msg
    }

    // 将that.messageTypeTest保存到文件'/msgStore.json'
    fs.writeFileSync('msgStore.json', JSON.stringify(messageStore, undefined, 2))
    return messageStore
}

export const readMsgStore = () => {
    // 读取'/msgStore.json'文件
    // 检测根目录下是否有msgStore.json文件，如果没有，则创建一个，内容为{}
    if (!fs.existsSync('msgStore.json')) {
        console.log('msgStore.json not exist')
        fs.writeFileSync('msgStore.json', '{}')
        return {}
    }
    const data = fs.readFileSync('msgStore.json', 'utf8')
    const json = JSON.parse(data)
    return json
}
