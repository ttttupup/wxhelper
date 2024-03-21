import requests
import json

host = "http://127.0.0.1:19088"

def checkLogin():
    url = host + "/api/checkLogin"
    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def userInfo():
    url = host + "/api/userInfo"
    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def sendTextMsg():
    url = host + "/api/sendTextMsg"
    payload = json.dumps({
        "wxid": "filehelper",
        "msg": "12www"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def sendImagesMsg():
    url = host + "/api/sendImagesMsg"
    print("modify imagePath")
    raise RuntimeError("modify imagePath then deleted me")
    payload = json.dumps({
        "wxid": "filehelper",
        "imagePath": "C:\\pic.png"
    })
    headers = {
        'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)


def sendFileMsg():
    url = host + "/api/sendFileMsg"
    print("modify filePath")
    raise RuntimeError("modify filePath then deleted me")
    payload = json.dumps({
        "wxid": "filehelper",
        "filePath": "C:\\test.zip"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def hookSyncMsg():
    url = host + "/api/hookSyncMsg"
    print("modify ip port url ")
    raise RuntimeError("modify ip port url then deleted me")
    payload = json.dumps({
        "port": "19099",
        "ip": "127.0.0.1",
        "url": "http://localhost:8080",
        "timeout": "3000",
        "enableHttp": "0"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def unhookSyncMsg():
    url = host + "/api/unhookSyncMsg"
    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def getContactList():
    url = host + "/api/getContactList"
    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def getDBInfo():
    url = host + "/api/getDBInfo"
    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def execSql():
    url = host + "/api/execSql"
    print("modify dbHandle ")
    raise RuntimeError("modify dbHandle then deleted me")
    payload = json.dumps({
        "dbHandle": 1713425147584,
        "sql": "select * from MSG where localId =100;"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def getChatRoomDetailInfo():
    url = host + "/api/getChatRoomDetailInfo"
    print("modify chatRoomId ")
    raise RuntimeError("modify chatRoomId then deleted me")
    payload = json.dumps({
        "chatRoomId": "123333@chatroom"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def addMemberToChatRoom():
    url = host + "/api/addMemberToChatRoom"
    print("modify chatRoomId  memberIds ")
    raise RuntimeError("modify chatRoomId memberIds then deleted me")
    payload = json.dumps({
        "chatRoomId": "123@chatroom",
        "memberIds": "wxid_123"
    })
    headers = {
        'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)


def delMemberFromChatRoom():
    url = host + "/api/delMemberFromChatRoom"
    print("modify chatRoomId  memberIds ")
    raise RuntimeError("modify chatRoomId memberIds then deleted me")
    payload = json.dumps({
        "chatRoomId": "21363231004@chatroom",
        "memberIds": "wxid_123"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def modifyNickname():
    url = host + "/api/modifyNickname"
    print("modify chatRoomId  wxid  nickName")
    raise RuntimeError("modify chatRoomId  wxid  nickName then deleted me")
    payload = json.dumps({
        "chatRoomId": "123@chatroom",
        "wxid": "wxid_123",
        "nickName": "test"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def getMemberFromChatRoom():
    print("modify chatRoomId  ")
    raise RuntimeError("modify chatRoomId then deleted me")
    url = host + "/api/getMemberFromChatRoom"
    payload = json.dumps({
        "chatRoomId": "123@chatroom"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def topMsg():
    print("modify msgId  ")
    raise RuntimeError("modify msgId then deleted me")
    url = host + "/api/topMsg"
    payload = json.dumps({
        "msgId": 1222222
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def removeTopMsg():
    print("modify msgId chatRoomId ")
    raise RuntimeError("modify msgId chatRoomId then deleted me")

    url = host + "/api/removeTopMsg"

    payload = json.dumps({
        "chatRoomId": "123@chatroom",
        "msgId": 123
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def InviteMemberToChatRoom():
    print("modify memberIds chatRoomId ")
    raise RuntimeError("modify memberIds chatRoomId then deleted me")

    url = host + "/api/InviteMemberToChatRoom"

    payload = json.dumps({
        "chatRoomId": "123@chatroom",
        "memberIds": "wxid_123"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def hookLog():
    url = host + "/api/hookLog"
    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def unhookLog():
    url = host + "/api/unhookLog"
    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def createChatRoom():
    print("modify memberIds  ")
    raise RuntimeError("modify memberIds then deleted me")
    url = host + "/api/createChatRoom"

    payload = json.dumps({
        "memberIds": "wxid_8yn4k908tdqp22,wxid_oyb662qhop4422"
    })
    headers = {
        'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)

def quitChatRoom():
    print("modify chatRoomId  ")
    raise RuntimeError("modify chatRoomId then deleted me")
    url = host + "/api/quitChatRoom"

    payload = json.dumps({
    "chatRoomId": "123@chatroom"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)

def forwardMsg():
    print("modify msgId  ")
    raise RuntimeError("modify msgId then deleted me")
    url = host + "/api/forwardMsg"

    payload = json.dumps({
    "wxid": "filehelper",
    "msgId": "12331"
    })
    headers = {
    'Content-Type': 'application/json'
    }
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)

def getSNSFirstPage():
    url = host + "/api/getSNSFirstPage"

    payload = {}
    headers = {}
    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)

def getSNSNextPage():
    print("modify snsId  ")
    raise RuntimeError("modify snsId then deleted me")
    url = host + "/api/getSNSNextPage"

    payload = json.dumps({
    "snsId": ""
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)

def addFavFromMsg():
    print("modify msgId  ")
    raise RuntimeError("modify msgId then deleted me")
    url = host + "/api/addFavFromMsg"

    payload = json.dumps({
    "msgId": "1222222"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)
    
def addFavFromImage():
    print("modify wxid imagePath ")
    raise RuntimeError("modify wxid  imagePath then deleted me")
    url = host + "/api/addFavFromImage"

    payload = json.dumps({
    "wxid": "",
    "imagePath": ""
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)

def getContactProfile():
    print("modify wxid  ")
    raise RuntimeError("modify wxid   then deleted me")
    url = host + "/api/getContactProfile"

    payload = json.dumps({
    "wxid": ""
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)
    print(response.text)


def sendAtText():
    print("modify wxids  chatRoomId")
    raise RuntimeError("modify wxids   chatRoomId then deleted me")
    url = host + "/api/sendAtText"

    payload = json.dumps({
    "wxids": "notify@all",
    "chatRoomId": "123@chatroom",
    "msg": "你好啊"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)

def forwardPublicMsg():
    print("modify param ")
    raise RuntimeError("modify param then deleted me")
    url = host + "/api/forwardPublicMsg"

    payload = json.dumps({
    "appName": "",
    "userName": "",
    "title": "",
    "url": "",
    "thumbUrl": "",
    "digest": "",
    "wxid": "filehelper"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)

def forwardPublicMsgByMsgId():
    print("modify param ")
    raise RuntimeError("modify param then deleted me")
    url = host + "/api/forwardPublicMsgByMsgId"

    payload = json.dumps({
    "msgId": 123,
    "wxid": "filehelper"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)

def downloadAttach():
    print("modify param ")
    raise RuntimeError("modify param then deleted me")
    url = host + "/api/downloadAttach"

    payload = json.dumps({
    "msgId": 123
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)


def decodeImage():
    print("modify param ")
    raise RuntimeError("modify param then deleted me")
    url = host + "/api/decodeImage"

    payload = json.dumps({
    "filePath": "C:\\66664816980131.dat",
    "storeDir": "C:\\test"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)


def getVoiceByMsgId():
    print("modify param ")
    raise RuntimeError("modify param then deleted me")
    url = host + "/api/getVoiceByMsgId"

    payload = json.dumps({
    "msgId": 7880439644200,
    "storeDir": "c:\\test"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    print(response.text)

    

if __name__ == '__main__':
    checkLogin()
    userInfo()
    sendTextMsg()
