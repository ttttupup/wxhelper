import net from 'net'
import {
  readMsgStore,
  writeMsgStore,
} from './utils/messageStore'
export const server = net.createServer((socket:any) => {
  console.log('New client connected')
  let messageStore = readMsgStore()
  let data = Buffer.from('')

  socket.on('data', (data:any, chunk:any) => {
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

const port = 19099

server.listen(port, () => {
  console.log(`Server listening on port ${port}`)
})
