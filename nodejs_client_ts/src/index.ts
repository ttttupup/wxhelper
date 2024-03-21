import { get, post } from './utils/mod'

// def checkLogin():
//     url = "127.0.0.1:19088/api/checkLogin"
//     payload = {}
//     headers = {}
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const checkLogin = () => {
    return post('/api/checkLogin')
}

// def userInfo():
// url = "127.0.0.1:19088/api/userInfo"
// payload = {}
// headers = {}
// response = requests.request("POST", url, headers=headers, data=payload)
// print(response.text)
export const userInfo = () => {
    return post('/api/userInfo')
}

// def sendTextMsg():
//     url = "127.0.0.1:19088/api/sendTextMsg"
//     payload = json.dumps({
//         "wxid": "filehelper",
//         "msg": "12www"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const sendTextMsg = (wxid: string, msg: string) => {
    return post('/api/sendTextMsg', { wxid, msg })
}

// def sendImagesMsg():
//     url = "127.0.0.1:19088/api/sendImagesMsg"
//     print("modify imagePath")
//     raise RuntimeError("modify imagePath then deleted me")
//     payload = json.dumps({
//         "wxid": "filehelper",
//         "imagePath": "C:\\pic.png"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)

export const sendImagesMsg = (wxid: string, imagePath: string) => {
    return post('/api/sendImagesMsg', { wxid, imagePath })
}

// def sendFileMsg():
//     url = "127.0.0.1:19088/api/sendFileMsg"
//     print("modify filePath")
//     raise RuntimeError("modify filePath then deleted me")
//     payload = json.dumps({
//         "wxid": "filehelper",
//         "filePath": "C:\\test.zip"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)

export const sendFileMsg = (wxid: string, filePath: string) => {
    return post('/api/sendFileMsg', { wxid, filePath })
}

// def hookSyncMsg():
//     url = "127.0.0.1:19088/api/hookSyncMsg"
//     print("modify ip port url ")
//     raise RuntimeError("modify ip port url then deleted me")
//     payload = json.dumps({
//         "port": "19099",
//         "ip": "127.0.0.1",
//         "url": "http://localhost:8080",
//         "timeout": "3000",
//         "enableHttp": "0"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const hookSyncMsg = (port: string, ip: string, url: string, timeout: string, enableHttp: string) => {
    return post('/api/hookSyncMsg', { port, ip, url, timeout, enableHttp })
}


// def unhookSyncMsg():
//     url = host + "/api/unhookSyncMsg"
//     payload = {}
//     headers = {}
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const unhookSyncMsg = () => {
    return post('/api/unhookSyncMsg')
}


// def getContactList():
//     url = host + "/api/getContactList"
//     payload = {}
//     headers = {}
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const getContactList = () => {
    return post('/api/getContactList')
}


// def getDBInfo():
//     url = host + "/api/getDBInfo"
//     payload = {}
//     headers = {}
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const getDBInfo = () => {
    return post('/api/getDBInfo')
}


// def execSql():
//     url = host + "/api/execSql"
//     print("modify dbHandle ")
//     raise RuntimeError("modify dbHandle then deleted me")
//     payload = json.dumps({
//         "dbHandle": 1713425147584,
//         "sql": "select * from MSG where localId =100;"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const execSql = (dbHandle: number, sql: string) => {
    return post('/api/execSql', { dbHandle, sql })
}


// def getChatRoomDetailInfo():
//     url = host + "/api/getChatRoomDetailInfo"
//     print("modify chatRoomId ")
//     raise RuntimeError("modify chatRoomId then deleted me")
//     payload = json.dumps({
//         "chatRoomId": "123333@chatroom"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const getChatRoomDetailInfo = (chatRoomId: string) => {
    return post('/api/getChatRoomDetailInfo', { chatRoomId })
}

// def addMemberToChatRoom():
//     url = host + "/api/addMemberToChatRoom"
//     print("modify chatRoomId  memberIds ")
//     raise RuntimeError("modify chatRoomId memberIds then deleted me")
//     payload = json.dumps({
//         "chatRoomId": "123@chatroom",
//         "memberIds": "wxid_123"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const addMemberToChatRoom = (chatRoomId: string, memberIds: string) => {
    return post('/api/addMemberToChatRoom', { chatRoomId, memberIds })
}


// def delMemberFromChatRoom():
//     url = host + "/api/delMemberFromChatRoom"
//     print("modify chatRoomId  memberIds ")
//     raise RuntimeError("modify chatRoomId memberIds then deleted me")
//     payload = json.dumps({
//         "chatRoomId": "21363231004@chatroom",
//         "memberIds": "wxid_123"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const delMemberFromChatRoom = (chatRoomId: string, memberIds: string) => {
    return post('/api/delMemberFromChatRoom', { chatRoomId, memberIds })
}


// def modifyNickname():
//     url = host + "/api/modifyNickname"
//     print("modify chatRoomId  wxid  nickName")
//     raise RuntimeError("modify chatRoomId  wxid  nickName then deleted me")
//     payload = json.dumps({
//         "chatRoomId": "123@chatroom",
//         "wxid": "wxid_123",
//         "nickName": "test"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const modifyNickname = (chatRoomId: string, wxid: string, nickName: string) => {
    return post('/api/modifyNickname', { chatRoomId, wxid, nickName })
}


// def getMemberFromChatRoom():
//     print("modify chatRoomId  ")
//     raise RuntimeError("modify chatRoomId then deleted me")
//     url = host + "/api/getMemberFromChatRoom"
//     payload = json.dumps({
//         "chatRoomId": "123@chatroom"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const getMemberFromChatRoom = (chatRoomId: string) => {
    return post('/api/getMemberFromChatRoom', { chatRoomId })
}


// def topMsg():
//     print("modify msgId  ")
//     raise RuntimeError("modify msgId then deleted me")
//     url = host + "/api/topMsg"
//     payload = json.dumps({
//         "msgId": 1222222
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const topMsg = (msgId: number) => {
    return post('/api/topMsg', { msgId })
}


// def removeTopMsg():
//     print("modify msgId chatRoomId ")
//     raise RuntimeError("modify msgId chatRoomId then deleted me")

//     url = host + "/api/removeTopMsg"

//     payload = json.dumps({
//         "chatRoomId": "123@chatroom",
//         "msgId": 123
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const removeTopMsg = (chatRoomId: string, msgId: number) => {
    return post('/api/removeTopMsg', { chatRoomId, msgId })
}


// def InviteMemberToChatRoom():
//     print("modify memberIds chatRoomId ")
//     raise RuntimeError("modify memberIds chatRoomId then deleted me")

//     url = host + "/api/InviteMemberToChatRoom"

//     payload = json.dumps({
//         "chatRoomId": "123@chatroom",
//         "memberIds": "wxid_123"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const InviteMemberToChatRoom = (chatRoomId: string, memberIds: string) => {
    return post('/api/InviteMemberToChatRoom', { chatRoomId, memberIds })
}


// def hookLog():
//     url = host + "/api/hookLog"
//     payload = {}
//     headers = {}
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const hookLog = () => {
    return post('/api/hookLog')
}


// def unhookLog():
//     url = host + "/api/unhookLog"
//     payload = {}
//     headers = {}
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const unhookLog = () => {
    return post('/api/unhookLog')
}


// def createChatRoom():
//     print("modify memberIds  ")
//     raise RuntimeError("modify memberIds then deleted me")
//     url = host + "/api/createChatRoom"

//     payload = json.dumps({
//         "memberIds": "wxid_8yn4k908tdqp22,wxid_oyb662qhop4422"
//     })
//     headers = {
//         'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const createChatRoom = (memberIds: string) => {
    return post('/api/createChatRoom', { memberIds })
}

// def quitChatRoom():
//     print("modify chatRoomId  ")
//     raise RuntimeError("modify chatRoomId then deleted me")
//     url = host + "/api/quitChatRoom"

//     payload = json.dumps({
//     "chatRoomId": "123@chatroom"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const quitChatRoom = (chatRoomId: string) => {
    return post('/api/quitChatRoom', { chatRoomId })
}

// def forwardMsg():
//     print("modify msgId  ")
//     raise RuntimeError("modify msgId then deleted me")
//     url = host + "/api/forwardMsg"

//     payload = json.dumps({
//     "wxid": "filehelper",
//     "msgId": "12331"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const forwardMsg = (wxid: string, msgId: string) => {
    return post('/api/forwardMsg', { wxid, msgId })
}

// def getSNSFirstPage():
//     url = host + "/api/getSNSFirstPage"

//     payload = {}
//     headers = {}
//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const getSNSFirstPage = () => {
    return post('/api/getSNSFirstPage')
}

// def getSNSNextPage():
//     print("modify snsId  ")
//     raise RuntimeError("modify snsId then deleted me")
//     url = host + "/api/getSNSNextPage"

//     payload = json.dumps({
//     "snsId": ""
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const getSNSNextPage = (snsId: string) => {
    return post('/api/getSNSNextPage', { snsId })
}

// def addFavFromMsg():
//     print("modify msgId  ")
//     raise RuntimeError("modify msgId then deleted me")
//     url = host + "/api/addFavFromMsg"

//     payload = json.dumps({
//     "msgId": "1222222"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const addFavFromMsg = (msgId: string) => {
    return post('/api/addFavFromMsg', { msgId })
}
    
// def addFavFromImage():
//     print("modify wxid imagePath ")
//     raise RuntimeError("modify wxid  imagePath then deleted me")
//     url = host + "/api/addFavFromImage"

//     payload = json.dumps({
//     "wxid": "",
//     "imagePath": ""
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const addFavFromImage = (wxid: string, imagePath: string) => {
    return post('/api/addFavFromImage', { wxid, imagePath })
}

// def getContactProfile():
//     print("modify wxid  ")
//     raise RuntimeError("modify wxid   then deleted me")
//     url = host + "/api/getContactProfile"

//     payload = json.dumps({
//     "wxid": ""
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)
//     print(response.text)
export const getContactProfile = (wxid: string) => {
    return post('/api/getContactProfile', { wxid })
}


// def sendAtText():
//     print("modify wxids  chatRoomId")
//     raise RuntimeError("modify wxids   chatRoomId then deleted me")
//     url = host + "/api/sendAtText"

//     payload = json.dumps({
//     "wxids": "notify@all",
//     "chatRoomId": "123@chatroom",
//     "msg": "你好啊"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const sendAtText = (wxids: string, chatRoomId: string, msg: string) => {
    return post('/api/sendAtText', { wxids, chatRoomId, msg })
}

// def forwardPublicMsg():
//     print("modify param ")
//     raise RuntimeError("modify param then deleted me")
//     url = host + "/api/forwardPublicMsg"

//     payload = json.dumps({
//     "appName": "",
//     "userName": "",
//     "title": "",
//     "url": "",
//     "thumbUrl": "",
//     "digest": "",
//     "wxid": "filehelper"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const forwardPublicMsg = (appName: string, userName: string, title: string, url: string, thumbUrl: string, digest: string, wxid: string) => {
    return post('/api/forwardPublicMsg', { appName, userName, title, url, thumbUrl, digest, wxid })
}

// def forwardPublicMsgByMsgId():
//     print("modify param ")
//     raise RuntimeError("modify param then deleted me")
//     url = host + "/api/forwardPublicMsgByMsgId"

//     payload = json.dumps({
//     "msgId": 123,
//     "wxid": "filehelper"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const forwardPublicMsgByMsgId = (msgId: number, wxid: string) => {
    return post('/api/forwardPublicMsgByMsgId', { msgId, wxid })
}

// def downloadAttach():
//     print("modify param ")
//     raise RuntimeError("modify param then deleted me")
//     url = host + "/api/downloadAttach"

//     payload = json.dumps({
//     "msgId": 123
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const downloadAttach = (msgId: number) => {
    return post('/api/downloadAttach', { msgId })
}


// def decodeImage():
//     print("modify param ")
//     raise RuntimeError("modify param then deleted me")
//     url = host + "/api/decodeImage"

//     payload = json.dumps({
//     "filePath": "C:\\66664816980131.dat",
//     "storeDir": "C:\\test"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const decodeImage = (filePath: string, storeDir: string) => {
    return post('/api/decodeImage', { filePath, storeDir })
}


// def getVoiceByMsgId():
//     print("modify param ")
//     raise RuntimeError("modify param then deleted me")
//     url = host + "/api/getVoiceByMsgId"

//     payload = json.dumps({
//     "msgId": 7880439644200,
//     "storeDir": "c:\\test"
//     })
//     headers = {
//     'Content-Type': 'application/json'
//     }

//     response = requests.request("POST", url, headers=headers, data=payload)

//     print(response.text)
export const getVoiceByMsgId = (msgId: number, storeDir: string) => {
    return post('/api/getVoiceByMsgId', { msgId, storeDir })
}
