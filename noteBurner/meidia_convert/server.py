import asyncio
import websockets
import json

from netFlixParser import getTrackInfo

PSSH = ""
licenseRequest=""
sessionId=""
url = "https://www.netflix.com/watch/80142058?trackId=255824129&tctx=0%2C0%2CNAPA%40%40%7C7d73b5e8-cd7b-4737-988e-1c504b531ab4-98736352_titles%2F1%2F%2F%E5%B0%8F%E5%A7%90%E5%A5%BD%E7%99%BD%2F0%2F0%2CNAPA%40%40%7C7d73b5e8-cd7b-4737-988e-1c504b531ab4-98736352_titles%2F1%2F%2F%E5%B0%8F%E5%A7%90%E5%A5%BD%E7%99%BD%2F0%2F0%2Cunknown%2C%2C7d73b5e8-cd7b-4737-988e-1c504b531ab4-98736352%7C1%2CtitlesResults%2C80142058%2CVideo%3A80142058%2CminiDpPlayButton"
license=""
track_info=""
KEEP_ID = None
id = None
def packMessage(fro, to, message):
    data = {
        "content": json.dumps(message),
        "header": {
            "from": fro,
            "to": to
        }
    }
    return json.dumps(data)


async def handle_client(websocket, path):
    global PSSH
    global licenseRequest
    global  sessionId
    global  KEEP_ID
    global id
    # 这是处理每个客户端连接的函数
    # 在这里编写服务器与客户端交互的逻辑

    while True:
        try:
            # 接收客户端消息
            message = await websocket.recv()
            print("Received message:", message)

            # 处理收到的消息
            if message.startswith("shell_"):
                id = message
                print("connect %s succsess" % id)
                # 发送响应消息给客户端
                response = "mediaconvert_NoteBurner-netflix"
                await websocket.send(response)
                message = {
                    "opData": {
                        "appIdentify": "com.noteburner.netflix"
                    },
                    "opType": "Initialize",
                    "token": "4224_1"
                }
                response = packMessage("mediaconvert_NoteBurner-netflix", id, message)
                await websocket.send(response)
            else:
                data = json.loads(message)
                content = json.loads(data['content'])
                header = data['header']
                opType = content['opType']
                opData = content['opData']
                if opType == "Echo":
                    KEEP_ID=header['to'].split("_")[1]
                    print("get connnect to keeper_%s" % KEEP_ID)
                    message = {
                        "opData": {
                            "cert": "Cr0CCAMSEOVEukALwQ8307Y2+LVP+0MYh/HPkwUijgIwggEKAoIBAQDm875btoWUbGqQD8eAGuBlGY+Pxo8YF1LQR+Ex0pDONMet8EHslcZRBKNQ/09RZFTP0vrYimyYiBmk9GG+S0wB3CRITgweNE15cD33MQYyS3zpBd4z+sCJam2+jj1ZA4uijE2dxGC+gRBRnw9WoPyw7D8RuhGSJ95OEtzg3Ho+mEsxuE5xg9LM4+Zuro/9msz2bFgJUjQUVHo5j+k4qLWu4ObugFmc9DLIAohL58UR5k0XnvizulOHbMMxdzna9lwTw/4SALadEV/CZXBmswUtBgATDKNqjXwokohncpdsWSauH6vfS6FXwizQoZJ9TdjSGC60rUB2t+aYDm74cIuxAgMBAAE6EHRlc3QubmV0ZmxpeC5jb20SgAOE0y8yWw2Win6M2/bw7+aqVuQPwzS/YG5ySYvwCGQd0Dltr3hpik98WijUODUr6PxMn1ZYXOLo3eED6xYGM7Riza8XskRdCfF8xjj7L7/THPbixyn4mULsttSmWFhexzXnSeKqQHuoKmerqu0nu39iW3pcxDV/K7E6aaSr5ID0SCi7KRcL9BCUCz1g9c43sNj46BhMCWJSm0mx1XFDcoKZWhpj5FAgU4Q4e6f+S8eX39nf6D6SJRb4ap7Znzn7preIvmS93xWjm75I6UBVQGo6pn4qWNCgLYlGGCQCUm5tg566j+/g5jvYZkTJvbiZFwtjMW5njbSRwB3W4CrKoyxw4qsJNSaZRTKAvSjTKdqVDXV/U5HK7SaBA6iJ981/aforXbd2vZlRXO/2S+Maa2mHULzsD+S5l4/YGpSt7PnkCe25F+nAovtl/ogZgjMeEdFyd/9YMYjOS4krYmwp3yJ7m9ZzYCQ6I8RQN4x/yLlHG5RH/+WNLNUs6JAZ0fFdCmw=",
                            "certMode": "Set",
                            "pssh": PSSH
                        },
                        "opType": "LicenseRequest",
                        "token": KEEP_ID+"_1"
                    }
                    response = packMessage("keeper_"+KEEP_ID, id, message)
                    await websocket.send(response)
                elif opType == "LicenseRequestResult":
                    licenseRequest = opData['data']['licenseRequest']
                    sessionId = opData['data']['sessionId']
                    request_license()
                    message = {
                        "opData": {
                            "license": license,
                            "sessionId": sessionId
                        },
                        "opType": "License",
                        "token":  KEEP_ID+"_1"
                    }
                    response = packMessage("keeper_" + KEEP_ID, id, message)
                    await websocket.send(response)

                elif opType == "LicenseResult":
                    message = tanstoDownloadInfo()
                    response = packMessage("keeper_" + KEEP_ID, id, message)
                    await websocket.send(response)
                elif opType == "GetGlobalInfo":
                    message = {
    "opData":{
        "data":{
            "convertMode":"C",
            "deviceInfo":{
                "ap":"com.noteburner.netflix",
                "apv":"2.0.3",
                "guid":"ae2d05e8-b1eb-4345-998b-e59df5b1be19",
                "uid":"00:0c:29:22:b0:e6",
                "website":"netflix"
            },
            "hwaccel":0,
            "nodePath":"\"C:\\Program Files (x86)\\NoteBurner\\NoteBurner Netflix Video Downloader\\NoteBurner Netflix Video Downloader.exe\""
        },
        "error":False,
        "errorCode":0
    },
    "opType":"GetGlobalInfoResult",
    "token":KEEP_ID+"_1"
}
                    response = packMessage("mediaconvert_NoteBurner-netflix", id, message)
                    await websocket.send(response)
        except websockets.exceptions.ConnectionClosedOK:
            # 客户端断开连接
            print("Client disconnected")
            break
def tanstoDownloadInfo():
    global  track_info
    global PSSH
    result = track_info['result']
    audio_tracks = result['audio_tracks']
    audio_track = audio_tracks[0]
    stream = audio_track['streams'][-1]
    audio_url = stream['urls'][0]['url']

    video_track = result['video_tracks'][0]
    video_stream = video_track['streams'][-1]
    video_url =  video_stream['urls'][0]['url']

    timedtexttrack= result['timedtexttracks'][0]

    download_info = {
    "opData":{
        "input":{
            "tracks":[
                {
                    "bitRate":stream['bitrate'] * 1000,
                    "bitrate":stream['bitrate'] * 1000,
                    "channels":stream['channels'],
                    "codec":audio_track['codecName']+" HQ",
                    "content_profile":audio_track['profile'],
                    "desc":audio_track['languageDescription'],
                    "headUrl":{
                        "length":stream['size'],
                        "offset":0,
                        "url":audio_url
                    },
                    "is5_1": True if stream['channels'] == "5.1" else False,
                    "isAD":False,
                    "isDefault":audio_track['isNative'],
                    "isDrm":stream['isDrm'],
                    "isOriginal":False,
                    "language":"en",
                    "languageDescription":audio_track['languageDescription'],
                    "languageLabel":"English (Original)",
                    "new_track_id":audio_track['new_track_id'],
                    "oriLanguage":audio_track["language"],
                    "size":stream['size'],
                    "trackId":"A:2:1;2;en;1;0;-128-107280467",
                    "type":0,
                    "uri":"A:2:1;2;en;1;0;-128-107280467"
                },
                {
                    "bitRate":video_stream['bitrate'] * 1000,
                    "bitrate":video_stream['bitrate'] * 1000,
                    "codec":"",
                    "desc":"1920x1080_1080P_6773_undefined_24000/1001",
                    "frameRate":str(video_stream['framerate_value'])+"/"+str(video_stream['framerate_scale']),
                    "headUrl":{
                        "length":video_stream['size'],
                        "offset":0,
                        "url":video_url
                    },
                    "isDrm":video_stream['isDrm'],
                    "pssh":PSSH,
                    "qualityIcon":"1080P",
                    "sar":"1:1",
                    "size":video_stream['size'],
                    "trackId":"V:2:1;2;;default;-1;none;-1;-6773",
                    "type":1,
                    "uri":"V:2:1;2;;default;-1;none;-1;-6773"
                },
                {
                    "cdnlist":timedtexttrack['cdnlist'],
                    "codec":"ttml-image",
                    "desc":"Chinese (Simplified)_zh-Hans_cc_false_forced_false_uriT:2:0;1;zh-Hans;0;0;0;_ttml-image",
                    "downloadableIds":{
                        "nflx-cmisc":"2070723521",
                        "webvtt-lssdh-ios8":"2070720789"
                    },
                    "encodingProfileNames":[
                        "nflx-cmisc",
                        "webvtt-lssdh-ios8"
                    ],
                    "headUrl":{
                        "length":0,
                        "offset":0,
                        "url":"http://23.246.55.165/?o=1&v=22&e=1686522996&t=sG76IZhq0JRQNt7wKstWU_E8mm0fWfCeLhC64_2PzKLnh1MqxATeiZBwhs_9tFcsNC5Ru5JwjIOhzan-ADdtlwB-cEsXUwP-DA8iEoHnnN_lf8M3OtdzpC96nhjnLbzZEkx8J53CdY2mV_-S3v_xsQ3Pszl7NPcHieJL9OE5HRN_dAWSFFJgyELUT6rLWe1p0MNYoDCKNfEcKF-xJ5FggbucnCo5TRlWQTup34J88_kCot4jfMLShQ"
                    },
                    "hydrated":True,
                    "id":"T:2:0;1;zh-Hans;0;0;0;",
                    "isCC":True,
                    "isDefault":True,
                    "isForced":False,
                    "isForcedNarrative":False,
                    "isImage":False,
                    "isLanguageLeftToRight":True,
                    "isNoneTrack":False,
                    "language":"zh",
                    "languageDescription":"Chinese (Simplified)",
                    "languageLabel":"Chinese (Simplified)",
                    "new_track_id":"T:2:0;1;zh-Hans;0;0;0;",
                    "oriLanguage":"zh-Hans",
                    "rank":1,
                    "rawTrackType":"subtitles",
                    "size":31209667,
                    "trackId":"T:2:0;1;zh-Hans;0;0;0;_ttml-image",
                    "trackType":"PRIMARY",
                    "ttDownloadables":timedtexttrack['ttDownloadables'],
                    "type":2,
                    "uri":"T:2:0;1;zh-Hans;0;0;0;_ttml-image"
                }
            ]
        },
        "isTrial":True,
        "mediaId":str(result['movieId']),
        "metaData":{
            "description":"两名黑人 FBI 警探受命保护一对头脑简单的上流社会姐妹，他们把自己化妆成这对白人姐妹，出入各种派对，试图搜捕想要绑架她们的绑匪。",
            "thumbnail":r"C:\Users\Administrator\Desktop\meidia_convert\81004276_1.jpg",
            "thumbnailType":"url",
            "title":"小姐好白",
            "year":2004
        },
        "output":{
            "folder":"C:\\Users\\Administrator\\Documents\\NoteBurner Netflix Video Downloader",
            "format":"mp4",
            "hwaccel":True,
            "ignorPartialFail":True,
            "path":"C:\\Users\\Administrator\\Documents\\NoteBurner Netflix Video Downloader\\小姐好白_9.mp4",
            "subtitleForm":2,
            "subtitleFormat":0,
            "videoCodec":"h264"
        }
    },
    "opType":"Convert",
    "token":KEEP_ID+"_1"
}
    return download_info
def request_license():
    global url
    global sessionId
    global licenseRequest
    global license
    track_info = getTrackInfo(url,sessionId,licenseRequest)
    license = track_info['result']['video_tracks'][0]['license']['licenseResponseBase64']
    print("license get susscess %s " % license)
def initConfig():
    global PSSH
    global url
    global  track_info

    track_info = getTrackInfo(url)
    PSSH = track_info['result']['video_tracks'][0]['drmHeader']['bytes']
    print("pssh get susscess %s " % PSSH)


if __name__ == "__main__":
    initConfig()


    # WebSocket服务器的地址和端口
    host = "localhost"
    port = 8012

    # 启动WebSocket服务器
    start_server = websockets.serve(handle_client, host, port)

    # 运行事件循环
    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()